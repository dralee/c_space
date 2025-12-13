/**
 * Robin Hood Hashing（开放寻址）
线程安全、无锁（lock-free）风格的 Robin Hood HashMap 实现（C11）。它满足多线程并发插入/查询/删除的基本无锁语义，
使用原子操作（stdatomic.h）对槽位进行 CAS 替换来实现 Robin Hood 的“抢位”逻辑。
重要限制与说明（必读）
在并发、无锁哈希表里，有几件事特别难做到并且实现成本高（需要高级回收方案）：
安全回收（memory reclamation）：无锁结构中不能直接释放被替换/删除的节点，否则可能被其他线程仍访问。这里的实现不做延迟
回收 —— 被替换/删除的 Entry 会泄漏（不 free），这是为了示例代码的简单性。工程级实现必须配合 epoch-based reclamation
 / hazard pointers / RCU 等回收机制来避免内存错误/悬挂指针。
不支持动态扩容（resize）：无锁 resize 非常复杂。此实现采用固定容量（power-of-two），不做扩容。若需扩容，请使用全局锁或更复杂的迁移方案。
性能：本实现示范了思路并可用于并发读写的原型或教学/实验用途。生产需要加入回收、resize、指纹优化、批量分配等。

无锁 Robin Hood HashMap 扩展为 带有简单 Epoch-Based Reclamation (EBR) 的版本。这个实现仍然保留之前的设计
（固定容量、无锁插入/查找/删除、使用原子 CAS），但在“替换/删除”时不再直接泄漏内存，而是把被替换/删除的 Entry 放入线程本地的回收（retire）列表，
并在安全时机（所有线程的最小活跃 epoch > 被回收对象的退役 epoch）释放内存。

说明：这个 EBR 实现是教学 / 工程原型级别，适合在你自己的项目中作为可靠回收方案的起点。要用于大规模生产还可继续优化（更高效的线程注册、动态线程表、批量合并、与扩容配合等）。
 关键特性与限制（先看要点）

✅ 支持多线程并发 insert/get/delete（C11 原子 + CAS）
✅ 增加Epoch-Based Reclamation：被替换/删除的 Entry 会安全延迟释放
✅ 仍为固定容量（不做扩容）——因为无锁安全扩容复杂
✅ 每个线程必须在使用 map 前 注册 (ebr_thread_register)，并在退出时注销 (ebr_thread_unregister)（实现用 pthread TLS）
⚠️ 目前回收策略为简单阈值触发（每个线程的 retire 列表长度到达阈值时会触发一次 global epoch 增加并尝试回收）
⚠️ 对于 key/value 的释放，map 会在真正释放 Entry 时调用用户提供的 free 函数（如果有）

Epoch-Based Reclamation (EBR) 的版本

-std=gnu11 -pthread 编译

gcc -std=gnu11 -pthread hash-robinhood5.c -O2 -o hash-robinhood5
 *
 * 2025.12.12 by dralee
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <stdint.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

/* --------------------------- config ------------------------- */
#define MAX_THREADS 128
#define RETIRE_THRESHOLD 64 // per-thread retire list length to trigger scan
#define MAX_CAPACITY (1<<20) // sanity


typedef unsigned (*hash_func)(const void*);
typedef int (*key_eq_func)(const void*, const void*);
typedef void (*free_func)(void*);

typedef struct {
        void* key;
        _Atomic(void*) value;
        int dist;         // probe distance
} Entry;

/* map stores atomic uintptr_t which is (uintptr_t)Entry* or special TOMBSTONE value */
typedef struct {
        atomic_uintptr_t *table;  // size = capacity
        atomic_int size;         // approximate
        size_t capacity;         // power-of-two

        hash_func hash;
        key_eq_func key_eq;

        free_func free_key;
        free_func free_value;
} RobinHoodHash;

/** RobinHoodHash retire node */
typedef struct RetireNode {
    struct RetireNode *next;
    Entry *entry;
    unsigned retire_epoch;
} RetireNode;

/* thread record for RobinHoodHash (EBR) */
typedef struct {
    atomic_uintptr_t active; // 0:not active, otherwise stores epoch | 1(encode active)
    atomic_uint_fast32_t local_epoch; // copy of global epoch when entered
    RetireNode *retire_list;
    size_t retire_count;
    pthread_t tid;
    int in_use;
} EBRThreadRecord;

/* global s for EBR */
static atomic_uint_fast32_t global_epoch = 0;
static EBRThreadRecord ebr_table[MAX_THREADS];
static pthread_mutex_t ebr_table_lock = PTHREAD_MUTEX_INITIALIZER;

/* TLS for thread id index */
static __thread int ebr_tid = -1;

/* TOMBSTONE sentinel */
static const uintptr_t TOMBSTONE = (uintptr_t)1; // sentinel for deleted slot

/* helpsers */
static inline Entry* entry_from_ptr(uintptr_t ptr) {
    return (Entry*)ptr;
}

static inline uintptr_t ptr_from_entry(Entry *entry) {
    return (uintptr_t)entry;
}

static Entry* make_entry(void* key, void* value, int dist) {
    Entry *entry = malloc(sizeof(Entry));
    if(!entry){
        return NULL;
    }
    entry->key = key;
    atomic_init(&entry->value, value);
    entry->dist = dist;
    return entry;
}

// duplicate entry with increased dist (used when an entry is displaced)
static Entry* duplicate_entry_with_dist(Entry *entry, int new_dist) {
    Entry *new_entry = malloc(sizeof(Entry));
    if(!new_entry){
        return NULL;
    }
    new_entry->key = entry->key;
    void *v = atomic_load(&entry->value);
    atomic_init(&new_entry->value, v);
    new_entry->dist = new_dist;
    return new_entry;
}

/* --------------------------- EBR API ------------------------- */
int ebr_thread_register(void) {
    if(ebr_tid != -1) { // aready registered
        return ebr_tid;
    }
    
    pthread_t self = pthread_self();
    pthread_mutex_lock(&ebr_table_lock);
    for(int i = 0; i < MAX_THREADS; ++i){
        if(!ebr_table[i].in_use){
            ebr_table[i].in_use = 1;
            ebr_table[i].tid = self;
            atomic_store(&ebr_table[i].active, 0); // not active yet
            atomic_store(&ebr_table[i].local_epoch, 0);
            ebr_table[i].retire_list = NULL;
            ebr_table[i].retire_count = 0;
            ebr_tid = i;
            pthread_mutex_unlock(&ebr_table_lock);
            return ebr_tid;
        }
    }
    pthread_mutex_unlock(&ebr_table_lock);
    return -1; // no slot
}

/* unregister the current thread - attempt to reclaim its leftover retire nodes */
void ebr_thread_unregister(void) {
    if(ebr_tid == -1) {
        return;
    }
    int id = ebr_tid;
    // mark not active
    atomic_store(&ebr_table[id].active, 0);
    // attempt a final global epoch advance and scan
    atomic_fetch_add(&global_epoch, 1);
    // perform scan to free retire list nodes if possible
    // we'll call ebr_scan() below which is safe
    // Note: ebr_scan operates globally; here we do a quick local free for remaining safe nodes
    // We'll reuse ebr_scan (but need to unlock if help)
    // call scan
    // for simplicity, allow other threads to participate: call ebr_scan from this thread
    // but must make sure to set active=0 first (done)
    // run scan once
    // Note: ebr_scan is defined later; to avoid implicit decl, ensure prototype already declared
    extern void ebr_scan(void);
    ebr_scan();
    // now free any still-retired nodes by waiting until safe (we'll spin a bit)
    // Simpler: leave them for others; but we try a few attempts to free them
    for(int attempt = 0; attempt < 3; ++attempt){
        ebr_scan();
        if(ebr_table[id].retire_list == NULL){
            break;
        }
        usleep(1000);
    }

    // mark record free (but don't free retire nodes here forcibly)
    pthread_mutex_lock(&ebr_table_lock);
    ebr_table[id].in_use = 0;
    ebr_table[id].tid = 0;
    ebr_table[id].retire_list = NULL;
    ebr_table[id].retire_count = 0;
    pthread_mutex_unlock(&ebr_table_lock);
    ebr_tid = -1;
}

/* thread enters a read/write critical section: record local epoch and mark active */
static inline void ebr_enter(void) {
    if(ebr_tid == -1){
        // auto register if not registered
        if(ebr_thread_register() == -1){
            fprintf(stderr, "EBR: ebr_thread_register failed\n");
            return;
        }
    }
    int id = ebr_tid;
    uint_fast32_t g = atomic_load(&global_epoch);
    atomic_store(&ebr_table[id].local_epoch, g);
    // encode active: set low bit to 1 in active (we store epoch in active variable as an uinptr for simpler compare)
    // we'll store epoch << 1 | 1 in active to mark active and epoch copy
    uintptr_t enc = ((uintptr_t)g << 1) | 1;
    atomic_store(&ebr_table[id].active, enc);
}

/* thread exists critical section: mark not active */
static inline void ebr_exit(void) {
    if(ebr_tid == -1){
        return;
    }
    int id = ebr_tid;
    // clear active
    atomic_store(&ebr_table[id].active, 0);
}

/* retire an entry: add to thread-local retire list with retire_epoch = global_epoch */
static void ebr_retire_entry(Entry *entry) {
    if(ebr_tid == -1){
        if(ebr_thread_register() == -1){
            // cannot register -> free immediately (last resort)
            // but this is dangerous; safer to leak; here we free to avoid leak in test env
            free(entry);
            return;
        }
    }

    int id = ebr_tid;
    RetireNode *rn = malloc(sizeof(RetireNode));
    rn->entry = entry;
    rn->retire_epoch = (unsigned)atomic_load(&global_epoch);
    rn->next = ebr_table[id].retire_list;
    ebr_table[id].retire_list = rn;
    ebr_table[id].retire_count++;

    // trigger a scan occasionally
    if(ebr_table[id].retire_count > RETIRE_THRESHOLD){
        // advance global epoch
        atomic_fetch_add(&global_epoch, 1);
        // run a scan to try reclaiming
        extern void ebr_scan(void);
        ebr_scan();
    }
}

/* compute minimum epoch among active threads (consider only active records) */
static unsigned ebr_min_active_epoch(void) {
    unsigned min_epoch = (unsigned)atomic_load(&global_epoch);
    for(int i = 0; i < MAX_THREADS; ++i){
        if(!ebr_table[i].in_use) {
            continue;
        }
        uintptr_t act = atomic_load(&ebr_table[i].active);
        if(act == 0){
            continue; // not active
        }
        unsigned local_epoch = (unsigned)((act >> 1) & 0xffffffffu);
        if(local_epoch < min_epoch){
            min_epoch = local_epoch;
        }
    }
    return min_epoch;
}

/* scan all threads' retire lists and free nodes with retire_epoch < min_active_epoch */
void ebr_scan(void) {
    unsigned min_epoch = ebr_min_active_epoch();
    // for each thread retire list, try to free nodes older than min_epoch
    for(int i = 0; i < MAX_THREADS; ++i){
        if(!ebr_table[i].in_use) {
            continue;
        }
        RetireNode **pp = &ebr_table[i].retire_list;
        while(*pp){
            RetireNode *rn = *pp;
            if(rn->retire_epoch < min_epoch){
                // safe to reclaim
                *pp = rn->next;
                ebr_table[i].retire_count--;
                // free actual entry resources (we need access to map free functions)
                // we'll attach a simple callback stored globally for freeing: but map may differ.
                // To keep it simple, each Entry should contain key/value that need freeing by caller:
                // We'll provide a global default free function pointer that the map sets before using ebr.
                extern void ebr_free_entry_resources(Entry *e);
                ebr_free_entry_resources(rn->entry);
                free(rn->entry);
                free(rn);
            } else {
                pp = &(*pp)->next;
            }
        }
    }
}

/* global callback pointers used by ebr_free_entry_resources to free key/value */
static free_func ebr_global_free_key = NULL;
static free_func ebr_global_free_value = NULL;
static pthread_mutex_t ebr_global_free_lock = PTHREAD_MUTEX_INITIALIZER;

/* set the global free callbacks (map should call this at create) */
void ebr_set_free_callbacks(free_func free_key, free_func free_value) {
    pthread_mutex_lock(&ebr_global_free_lock);
    ebr_global_free_key = free_key;
    ebr_global_free_value = free_value;
    pthread_mutex_unlock(&ebr_global_free_lock);
}

/* called by EBR to free an entry's key/value (map-level free funcs) */
void ebr_free_entry_resources(Entry *e) {
    if(!e){
        return;
    }
    // free key/value using callbacks
    if(ebr_global_free_key && e->key){
        ebr_global_free_key(e->key);
    }
    if(ebr_global_free_value){
        void *v = atomic_load(&e->value);
        if(v){
            ebr_global_free_value(v);
        }
    }
}

/* -------------------------------------------- map functions (lock-free Robin Hood) -------------------------------------------- */

// create map with fixed capacity (must be >= 2 and power of two will be chosen)
RobinHoodHash *rh_create(size_t capacity, hash_func hash, key_eq_func key_eq, 
    free_func free_key, free_func free_value) {
    if(capacity < 2) {
        capacity = 2;
    }

    // round up to power of two for nicer modulo (optional)
    size_t cap = 1;
    while (cap < capacity) {
        cap <<= 1;
    }
    if(cap > MAX_CAPACITY){
        cap = MAX_CAPACITY;
    }

    RobinHoodHash *rh = malloc(sizeof(RobinHoodHash));
    if(!rh){
        return NULL;
    }
    rh->capacity = cap;
    rh->table = calloc(cap, sizeof(atomic_uintptr_t));
    if(!rh->table){
        free(rh);
        return NULL;
    }
    for(size_t i = 0; i < cap; i++){
        atomic_init(&rh->table[i], (uintptr_t)0);
    }
    atomic_init(&rh->size, 0);

    rh->hash = hash;
    rh->key_eq = key_eq;
    rh->free_key = free_key;
    rh->free_value = free_value;

    // register free callbacks for EBR to use when reclaiming
    ebr_set_free_callbacks(free_key, free_value);

    return rh;
}

// destroy map: attempt to free live entries and run scans; any outstanding retired entires may be freed too
void rh_destroy(RobinHoodHash *rh) {
    if(!rh) return;
    // free live entires
    for(size_t i = 0; i < rh->capacity; ++i){
        uintptr_t p = atomic_load(&rh->table[i]);
        if(p != 0 && p != TOMBSTONE){
            Entry *entry = entry_from_ptr(p);
            if(rh->free_key && entry->key){
                rh->free_key(entry->key);
            }
            void *v = atomic_load(&entry->value);
            if(rh->free_value && v){
                rh->free_value(v);
            }
            free(entry);
        }
    }
    free(rh->table);
    free(rh);

    // try to advance epoch and collect retired nodes
    atomic_fetch_add(&global_epoch, 1);
    ebr_scan();
}

// internal: compute index
static inline size_t index_for_hash(RobinHoodHash *rh, unsigned hash) {
    return (size_t)(hash & (rh->capacity - 1)); // capacity is power-of-two
}

// insert / update (lock-free). returns 1 on insert, 2 on update, 0 on failure(should not happen)
int rh_insert(RobinHoodHash *rh, void* key, void* value) {
    ebr_enter();

    unsigned hash = rh->hash(key);
    size_t index = index_for_hash(rh, hash);
    Entry *new_entry = make_entry(key, value, 0);
    if(!new_entry){
        return 0;
    }

    size_t cap = rh->capacity;
    for(size_t probes = 0; probes < cap; ++probes) {
        size_t i = (index + probes) & (cap - 1);
        uintptr_t cur = atomic_load(&rh->table[i]);

        // empty slot: try to CAS in
        if(cur == 0 || cur == TOMBSTONE){
            uintptr_t expected = cur;
            if(atomic_compare_exchange_strong(&rh->table[i], &expected, ptr_from_entry(new_entry))){
                atomic_fetch_add(&rh->size, 1);
                ebr_exit();
                return 1; // inserted
            }else{
                // CAS failed, retry same slot by reloading
                probes--; // try same probe again
                continue;
            }
        }

        // slot occupied
        Entry *slot_e = entry_from_ptr(cur);
        // check key equality => update value
        if(rh->key_eq(slot_e->key, key)){
            // update in-place the value (atomic)
            void *oldv = atomic_exchange(&slot_e->value, value);
            // free new entry we allocated
            if(rh->free_key){
                rh->free_key(new_entry->key); // free the key wee created (caller my have passed ownership)
            }
            free(new_entry);
            if (rh->free_value && oldv) {
                rh->free_value(oldv);
                ebr_exit();
                return 2; // updated
            }            
        }

        // robin hood: if current slot's dist < new entry's dist, swap(steal)
        if(slot_e->dist < new_entry->dist){
            // attempt to CAS place new_entry here(only if unchanged)
            uintptr_t expected = cur;
            if(atomic_compare_exchange_strong(&rh->table[i], &expected, ptr_from_entry(new_entry))){
                // success: slot now holds new_e (owned by table), displaced 'slot_e' must be reinserted with dist+1
                Entry *displaced = duplicate_entry_with_dist(slot_e, new_entry->dist + 1);
                // retire original slot_e (since it's still present in memory but not referenced by table)
                // but we must keep slot_e memory alive until reclaimed: retire it
                ebr_retire_entry(slot_e);
                new_entry = displaced;
                // continue probing with new_entry
                continue;
            }
            // CAS failed, reload and retry this slot
            probes--;
            continue;
        }

        // otherwise, advance: increment new_entry's dist and continue
        new_entry->dist++;
        continue;
    }
    
    // table full or couldn't insert 
    // drop new_entry (retire or free)
    if(rh->free_key){
        rh->free_key(new_entry->key); // free the key wee created (caller my have passed ownership)
    }
    if(rh->free_value){
        void *v = atomic_load(&new_entry->value);
        if(v){
            rh->free_value(v);
        }
    }
    free(new_entry);
    ebr_exit();
    return 0;
}

// get value pointer for key (returns stored value pointer or NULL)
void* rh_get(RobinHoodHash *rh, const void* key) {
    ebr_enter();
    unsigned hash = rh->hash(key);
    size_t index = index_for_hash(rh, hash);
    size_t cap = rh->capacity;
    int dist = 0;

    for(size_t probes = 0; probes < cap; ++probes) {
        size_t i = (index + probes) & (cap - 1);
        uintptr_t cur = atomic_load(&rh->table[i]);

        if(cur == 0) {
            ebr_exit();
            return NULL; // empty means not found
        }
        if(cur == TOMBSTONE){
            dist++;
            continue;
        }

        Entry *slot_e = entry_from_ptr(cur);
        if(rh->key_eq(slot_e->key, key)){
            void *v = atomic_load(&slot_e->value);
            ebr_exit();
            return v;
        }
        // robin hood stop condition
        if(dist > slot_e->dist){
            ebr_exit();
            return NULL;
        }

        dist++;
    }
    ebr_exit();
    return NULL;
}

// delete key (returns 1 on success, 0 not found)
int rh_delete(RobinHoodHash *rh, const void* key) {
    ebr_enter();
    unsigned hash = rh->hash(key);
    size_t index = index_for_hash(rh, hash);
    size_t cap = rh->capacity;
    int dist = 0;

    for (size_t probes = 0; probes < cap; ++probes) {
        size_t i = (index + probes) & (cap - 1);
        uintptr_t cur = atomic_load(&rh->table[i]);

        if (cur == 0) {
            ebr_exit();
            return 0; // empty means not found
        }
        if (cur == TOMBSTONE) {
            dist++;
            continue;
        }

        Entry *slot_e = entry_from_ptr(cur);
        if(rh->key_eq(slot_e->key, key)){
            // try to replace with tombstone
            uintptr_t expected = cur;
            if(atomic_compare_exchange_strong(&rh->table[i], &expected, TOMBSTONE)){
                atomic_fetch_sub(&rh->size, 1);
                // retire the removed entry for later reclaim
                ebr_retire_entry(slot_e);
                ebr_exit();
                return 1;
            }
            // someone changed the slot; retry same index
            probes--;
            continue;
        }

        if(dist > slot_e->dist){
            ebr_exit();
            return 0;
        }
        dist++;
    }
    ebr_exit();
    return 0;
}

/* -------------------------------------------- example hash/eq/free for const char* -> int* -------------------------------------------- */

unsigned str_hash(const void *p){
    const char *s = p;
    unsigned h = 2166136261u;
    while (*s) {
        h ^= (unsigned char)*s++;
        h *= 16777619u;
    }
    return h;
}

int str_eq(const void *a, const void *b){
    return strcmp((const char*)a, (const char*)b) == 0;
}

void free_int(void *p){ free(p); }
void free_str(void *p){ free(p); }

/* -------------------------------------------- simple test (multi-thread) -------------------------------------------- */
// compile with -DTEST_MULTI and run

#ifdef TEST_MULTI
#define THREADS 4
#define OPS_PER_THREAD 10000

typedef struct {
    RobinHoodHash *rh;
    int id;
} WorkerArg;

void *worker(void *arg) {
    WorkerArg *warg = arg;
    // register thread for EBR
    ebr_thread_register();

    RobinHoodHash *rh = warg->rh;    
    int id = warg->id;
    char buf[64];
    for (int i = 0; i < OPS_PER_THREAD; ++i) {
        int k = id * OPS_PER_THREAD + i;
        snprintf(buf, sizeof(buf), "key-%d", k);
        char *ks = strdup(buf);
        int *v = malloc(sizeof(int));
        *v = k * 10;
        rh_insert(rh, ks, v);

        // occasionally lookup / delete
        if(k & 7 == 0){
            void *r = rh_get(rh, ks);
            // don't free lookup result (map owns)
            (void)r;
            printf("key-%d: %d\n", k, *(int*)r);
        }
        if((i & 31) == 0){
            rh_delete(rh, ks);
            printf("deleted key-%d\n", k);
        }
    }
    ebr_thread_unregister();
    return NULL;
}

int main() {
    RobinHoodHash *rh = rh_create(1<<16, str_hash, str_eq, free_str, free_int);
    pthread_t th[THREADS];
    WorkerArg warg[THREADS];
    for (int i = 0; i < THREADS; ++i) {
        warg[i].rh = rh;
        warg[i].id = i;
        pthread_create(&th[i], NULL, worker, &warg[i]);
    }
    for (int i = 0; i < THREADS; ++i) {
        pthread_join(th[i], NULL);
    }

    // final scan & destroy
    atomic_fetch_and(&global_epoch, 1);
    ebr_scan();

    rh_destroy(rh);
    printf("done\n");
    return 0;
}
#endif


/* -------------------------------------------- simple test (single-thread) -------------------------------------------- */
// compile with -DTEST_SINGLE and run
#ifdef TEST_SINGLE
int main() {
    RobinHoodHash *rh = rh_create(64, str_hash, str_eq, free_str, free_int);

    ebr_thread_register();

    char *k = strdup("alice");
    int *v = malloc(sizeof(int));
    rh_insert(rh, k, v);

    int *r = rh_get(rh, k);
    printf("alice: %d\n", r ? *r : -1);

    rh_delete(rh, k);
    printf("deleted alice\n");
    r = rh_get(rh, k);
    printf("after delete: alice: %s\n", r ? "found" : "not found");

    ebr_thread_unregister();

    atomic_fetch_add(&global_epoch, 1);
    ebr_scan();

    rh_destroy(rh);
    printf("done\n");
    return 0;
}
#endif