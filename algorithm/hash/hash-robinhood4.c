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

 * 简化且教学用途的无锁 Robin Hood HashMap（C11）
 *
 * 限制：
 *  - 固定容量 (no resize)
 *  - 不做安全内存回收（被替换或删除的 Entry 会泄漏）
 *  - 需要用户提供 hash 和 key_eq
 *
 * 生产请使用 epoch/hazard-pointer/RCU 回收，并支持迁移/resize。

-std=gnu11 -pthread 编译

gcc -std=gnu11 -pthread hash-robinhood4.c -DTEST_SINGLE -O2 -o hash-robinhood4
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


typedef unsigned (*hash_func)(const void*);
typedef int (*key_eq_func)(const void*, const void*);
typedef void (*free_func)(void*);

typedef struct {
        void* key;
        _Atomic(void*) value;
        int dist;         // probe distance
} Entry;

typedef struct {
        atomic_uintptr_t *table;
        atomic_int size;
        size_t capacity;

        hash_func hash;
        key_eq_func key_eq;

        free_func free_key;
        free_func free_value;
} RobinHoodHash;

static const uintptr_t TOMBSTONE = (uintptr_t)1; // sentinel for deleted slot

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

// create a fixed capacity map(capacity must be >= 2)
RobinHoodHash *rh_create(size_t capacity, hash_func hash, key_eq_func key_eq, 
    free_func free_key, free_func free_value) {
    // round up to power of two for nicer modulo (optional)
    size_t cap = 1;
    while (cap < capacity) {
        cap <<= 1;
    }
    RobinHoodHash *rh = malloc(sizeof(RobinHoodHash));
    if(!rh){
        return NULL;
    }
    rh->capacity = cap;
    rh->table = calloc(cap, sizeof(atomic_uintptr_t));
    for(size_t i = 0; i < cap; i++){
        atomic_init(&rh->table[i], (uintptr_t)0);
    }
    atomic_init(&rh->size, 0);

    rh->hash = hash;
    rh->key_eq = key_eq;
    rh->free_key = free_key;
    rh->free_value = free_value;

    return rh;
}

// destroy map(note: leaked replaced/deleted Entry not freed here)
void rh_destroy(RobinHoodHash *rh) {
    if(!rh) return;
    for(size_t i = 0; i < rh->capacity; ++i){
        uintptr_t p = atomic_load(&rh->table[i]);
        if(p != 0 && p != TOMBSTONE){
            Entry *entry = entry_from_ptr(p);
            if(rh->free_key){
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
}

// internal: compute index
static inline size_t index_for_hash(RobinHoodHash *rh, unsigned hash) {
    return (size_t)(hash & (rh->capacity - 1)); // capacity is power-of-two
}

// insert / update (lock-free). returns 1 on insert, 2 on update, 0 on failure(should not happen)
int rh_insert(RobinHoodHash *rh, void* key, void* value) {
    unsigned hash = rh->hash(key);
    size_t index = index_for_hash(rh, hash);
    Entry *new_entry = make_entry(key, value, 0);
    if(!new_entry){
        return 0;
    }

    size_t cap = rh->capacity;
    for(size_t probes = 0; probes < cap; ++probes) {
        size_t i = (index + probes) % (cap - 1);
        uintptr_t cur = atomic_load(&rh->table[i]);

        // empty slot: try to CAS in
        if(cur == 0 || cur == TOMBSTONE){
            uintptr_t expected = cur;
            if(atomic_compare_exchange_strong(&rh->table[i], &expected, ptr_from_entry(new_entry))){
                atomic_fetch_add(&rh->size, 1);
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
                return 2; // updated
            }            
        }

        // robin hood: if current slot's dist < new entry's dist, swap(steal)
        if(slot_e->dist < new_entry->dist){
            // attempt to CAS place new_entry here(only if unchanged)
            uintptr_t expected = cur;
            if(atomic_compare_exchange_strong(&rh->table[i], &expected, ptr_from_entry(new_entry))){
                // success: slot now holds new_entry, the displaced is 'slot_e'
                // we must continue inserting the displaced element with increased dist
                // but we cannot modify slot_e in-place (other threads may see it).so create a copy with dist+1
                Entry *displaced = duplicate_entry_with_dist(slot_e, new_entry->dist + 1);
                // note: original slot_e still exists in memory and is leaked (no reclamation here)
                new_entry = displaced;
                // continue probing with new_entry (probes keeps moving)
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
    
    // table full /couldn't insert 
    // leak new_entry (or free key/value if desired)
    return 0;
}

// get value pointer for key (returns stored value pointer or NULL)
void* rh_get(RobinHoodHash *rh, const void* key) {
    unsigned hash = rh->hash(key);
    size_t index = index_for_hash(rh, hash);
    size_t cap = rh->capacity;
    int dist = 0;

    for(size_t probes = 0; probes < cap; ++probes) {
        size_t i = (index + probes) % (cap - 1);
        uintptr_t cur = atomic_load(&rh->table[i]);

        if(cur == 0) {
            return NULL; // empty means not found
        }
        if(cur == TOMBSTONE){
            dist++;
            continue;
        }

        Entry *slot_e = entry_from_ptr(cur);
        if(rh->key_eq(slot_e->key, key)){
            return atomic_load(&slot_e->value);
        }
        // robin hood stop condition
        if(dist > slot_e->dist){
            return NULL;
        }

        dist++;
    }
    return NULL;
}

// delete key (returns 1 on success, 0 not found)
int rh_delete(RobinHoodHash *rh, const void* key) {
    unsigned hash = rh->hash(key);
    size_t index = index_for_hash(rh, hash);
    size_t cap = rh->capacity;
    int dist = 0;

    for (size_t probes = 0; probes < cap; ++probes) {
        size_t i = (index + probes) % (cap - 1);
        uintptr_t cur = atomic_load(&rh->table[i]);

        if (cur == 0) {
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
                // note: we do not free slot_e here(no reclamation)
                return 1;
            }
            // someone changed the slot; retry same index
            probes--;
            continue;
        }

        if(dist > slot_e->dist){
            return 0;
        }
        dist++;
    }
    return 0;
}

/*
* example for key=string, value=int*
*/
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

// simple single-thread test
// -DTEST_SINGLE specify single-thread
#ifdef TEST_SINGLE
int main() {
    RobinHoodHash *rh = rh_create(64, str_hash, str_eq, free_str, free_int);
    for (int i = 0; i < 20; ++i) {
        void* key = malloc(10);  // strdup("alice")
        if(!sprintf(key, "key-%d", i)){
            printf("error for malloc key.");
            return 1;
        }
        int* value = malloc(sizeof(int));
        *value = i*10;
        rh_insert(rh, key, value);
    }

    void* k10 = malloc(10);
    if(!sprintf(k10, "key-%d", 10)){
        printf("error for malloc key-10.");
        return 1;
    }
    const int *val = rh_get(rh, k10);
    if (val) {
        printf("key 10 -> %d\n", *val);
    }

    rh_delete(rh, k10);
    val = rh_get(rh, k10);
    printf("after delete: key key-10 -> %s\n", val ? "found" : "not found");

    free(k10);
    rh_destroy(rh);
    return 0;
}
#endif