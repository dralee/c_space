// rh_lockfree_ebr.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <stdint.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

/*
 * Lock-free Robin Hood HashMap + simple Epoch-Based Reclamation (EBR)
 *
 * Limitations:
 *  - Fixed capacity (no resize)
 *  - Threads MUST register via ebr_thread_register() before using the map
 *  - EBR implementation is simple: global epoch increments when retire list reaches threshold
 *  - Suitable as a practical prototype. For production, consider robust optimizations.
 */

/* ---------------------- Config ---------------------- */

#define MAX_THREADS 128
#define RETIRE_THRESHOLD 64  // per-thread retire list length to trigger scan
#define MAX_CAPACITY (1<<20) // sanity

/* ---------------------- Types & function typedefs ---------------------- */

typedef unsigned (*hash_func)(const void *);
typedef int      (*key_eq_func)(const void *, const void *);
typedef void     (*free_func)(void *);

typedef struct Entry {
    void *key;
    _Atomic(void *) value;
    int dist; // probe distance
} Entry;

/* Map stores atomic uintptr_t which is (uintptr_t)Entry* or special TOMBSTONE value */
typedef struct {
    atomic_uintptr_t *table; // size = capacity
    size_t capacity;         // power of two
    atomic_int size;         // approximate
    hash_func hash;
    key_eq_func key_eq;
    free_func free_key;
    free_func free_value;
} LFMap;

/* EBR retire node */
typedef struct RetireNode {
    struct RetireNode *next;
    Entry *entry;
    unsigned retire_epoch;
} RetireNode;

/* Thread record for EBR */
typedef struct {
    atomic_uintptr_t active; // 0 = not active, otherwise stores epoch | 1 (encode active)
    atomic_uint_fast32_t local_epoch; // copy of global epoch when entered
    RetireNode *retire_list;
    size_t retire_count;
    pthread_t tid;
    int in_use;
} EBRThreadRecord;

/* ---------------------- Globals for EBR ---------------------- */

static atomic_uint_fast32_t global_epoch = 0;
static EBRThreadRecord ebr_table[MAX_THREADS];
static pthread_mutex_t ebr_table_lock = PTHREAD_MUTEX_INITIALIZER;

/* TLS for thread id index */
static __thread int ebr_tid = -1;

/* TOMBSTONE sentinel */
static const uintptr_t TOMBSTONE = (uintptr_t)1;

/* ---------------------- Helpers ---------------------- */

static inline Entry *entry_from_ptr(uintptr_t p) {
    return (Entry *)(uintptr_t)p;
}
static inline uintptr_t ptr_from_entry(Entry *e) {
    return (uintptr_t)(uintptr_t)e;
}

/* ---------------------- EBR API ---------------------- */

/* register current thread to EBR; returns index >=0 on success or -1 on failure */
int ebr_thread_register(void) {
    if (ebr_tid != -1) return ebr_tid; // already registered

    pthread_t self = pthread_self();
    pthread_mutex_lock(&ebr_table_lock);
    for (int i = 0; i < MAX_THREADS; ++i) {
        if (!ebr_table[i].in_use) {
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

/* unregister the current thread - attempt to reclaim its leftover retired nodes */
void ebr_thread_unregister(void) {
    if (ebr_tid == -1) return;
    int id = ebr_tid;
    // mark not active
    atomic_store(&ebr_table[id].active, 0);
    // attempt a final global epoch advance and scan
    atomic_fetch_add(&global_epoch, 1);
    // perform scan to free retire list nodes if possible
    // We'll call ebr_scan() below which is safe
    // Note: ebr_scan operates globally; here we do a quick local free for remaining safe nodes
    // We'll reuse ebr_scan (but need to unlock if held)
    // call scan
    // For simplicity, allow other threads to participate: call ebr_scan from this thread
    // but must make sure to set active=0 first (done)
    // run scan once
    // Note: ebr_scan is defined later; to avoid implicit decl, ensure prototype already declared
    extern void ebr_scan(void);
    ebr_scan();
    // now free any still-retired nodes by waiting until safe (we'll spin a bit)
    // Simpler: leave them for others; but we try a few attempts to free them:
    for (int attempt = 0; attempt < 3; ++attempt) {
        ebr_scan();
        if (ebr_table[id].retire_list == NULL) break;
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
    if (ebr_tid == -1) {
        // auto-register if not registered
        if (ebr_thread_register() == -1) {
            fprintf(stderr, "EBR: thread register failed\n");
            return;
        }
    }
    int id = ebr_tid;
    uint_fast32_t g = atomic_load(&global_epoch);
    atomic_store(&ebr_table[id].local_epoch, g);
    // encode active: set low bit to 1 in active (we store epoch in active variable as an uintptr for simpler compare)
    // We'll store epoch<<1 | 1 in active to mark active and epoch copy
    uintptr_t enc = ((uintptr_t)g << 1) | 1;
    atomic_store(&ebr_table[id].active, enc);
}

/* thread exits critical section: mark not active */
static inline void ebr_exit(void) {
    if (ebr_tid == -1) return;
    int id = ebr_tid;
    atomic_store(&ebr_table[id].active, 0);
}

/* retire an entry: add to thread-local retire list with retire_epoch = global_epoch */
static void ebr_retire_entry(Entry *e) {
    if (ebr_tid == -1) {
        if (ebr_thread_register() == -1) {
            // cannot register -> free immediately (last resort)
            // but this is dangerous; safer to leak; here we free to avoid leak in test env
            free(e);
            return;
        }
    }
    int id = ebr_tid;
    RetireNode *rn = malloc(sizeof(RetireNode));
    rn->entry = e;
    rn->retire_epoch = (unsigned)atomic_load(&global_epoch);
    rn->next = ebr_table[id].retire_list;
    ebr_table[id].retire_list = rn;
    ebr_table[id].retire_count++;

    // trigger a scan occasionally
    if (ebr_table[id].retire_count >= RETIRE_THRESHOLD) {
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
    for (int i = 0; i < MAX_THREADS; ++i) {
        if (!ebr_table[i].in_use) continue;
        uintptr_t act = atomic_load(&ebr_table[i].active);
        if (act == 0) continue; // not active
        unsigned local_epoch = (unsigned)( (act >> 1) & 0xffffffffu );
        if (local_epoch < min_epoch) min_epoch = local_epoch;
    }
    return min_epoch;
}

/* scan all threads' retire lists and free nodes with retire_epoch < min_active_epoch */
void ebr_scan(void) {
    unsigned min_epoch = ebr_min_active_epoch();
    // for each thread retire list, try to free nodes older than min_epoch
    for (int i = 0; i < MAX_THREADS; ++i) {
        if (!ebr_table[i].in_use) continue;
        RetireNode **pp = &ebr_table[i].retire_list;
        while (*pp) {
            RetireNode *rn = *pp;
            if (rn->retire_epoch < min_epoch) {
                // safe to reclaim
                *pp = rn->next;
                ebr_table[i].retire_count--;
                // free actual entry resources (we need access to map free functions)
                // We'll attach a simple callback stored globally for freeing: but map may differ.
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
void ebr_set_free_callbacks(free_func fk, free_func fv) {
    pthread_mutex_lock(&ebr_global_free_lock);
    ebr_global_free_key = fk;
    ebr_global_free_value = fv;
    pthread_mutex_unlock(&ebr_global_free_lock);
}

/* called by EBR to free an entry's key/value (map-level free funcs) */
void ebr_free_entry_resources(Entry *e) {
    if (!e) return;
    // free key/value using callbacks
    if (ebr_global_free_key && e->key) ebr_global_free_key(e->key);
    if (ebr_global_free_value) {
        void *v = atomic_load(&e->value);
        if (v) ebr_global_free_value(v);
    }
}

/* ---------------------- Map functions (lock-free Robin Hood) ---------------------- */

/* Create new Entry on heap */
static Entry *make_entry(void *key, void *value, int dist) {
    Entry *e = malloc(sizeof(Entry));
    if (!e) return NULL;
    e->key = key;
    atomic_init(&e->value, value);
    e->dist = dist;
    return e;
}

/* duplicate entry with increased dist (used when an entry is displaced) */
static Entry *dup_entry_with_dist(Entry *src, int new_dist) {
    Entry *e = malloc(sizeof(Entry));
    if (!e) return NULL;
    e->key = src->key;
    void *v = atomic_load(&src->value);
    atomic_init(&e->value, v);
    e->dist = new_dist;
    return e;
}

/* Create map with fixed capacity (must be >= 2 and power of two will be chosen) */
LFMap *lfmap_create(size_t capacity, hash_func h, key_eq_func eq, free_func fk, free_func fv) {
    if (capacity < 2) capacity = 2;
    size_t cap = 1;
    while (cap < capacity) cap <<= 1;
    if (cap > MAX_CAPACITY) cap = MAX_CAPACITY;

    LFMap *m = malloc(sizeof(LFMap));
    if (!m) return NULL;
    m->capacity = cap;
    m->table = calloc(cap, sizeof(atomic_uintptr_t));
    if (!m->table) { free(m); return NULL; }
    for (size_t i = 0; i < cap; ++i) atomic_init(&m->table[i], (uintptr_t)0);
    atomic_init(&m->size, 0);
    m->hash = h;
    m->key_eq = eq;
    m->free_key = fk;
    m->free_value = fv;

    // register free callbacks for EBR to use when reclaiming
    ebr_set_free_callbacks(fk, fv);

    return m;
}

/* Destroy map: attempt to free live entries and run scans; any outstanding retired entries may be freed too */
void lfmap_destroy(LFMap *m) {
    if (!m) return;
    // free live entries
    for (size_t i = 0; i < m->capacity; ++i) {
        uintptr_t p = atomic_load(&m->table[i]);
        if (p != 0 && p != TOMBSTONE) {
            Entry *e = entry_from_ptr(p);
            if (m->free_key && e->key) m->free_key(e->key);
            void *v = atomic_load(&e->value);
            if (m->free_value && v) m->free_value(v);
            free(e);
        }
    }

    free(m->table);
    free(m);

    // try to advance epoch and collect retired nodes
    atomic_fetch_add(&global_epoch, 1);
    ebr_scan();
}

/* internal: compute index */
static inline size_t idx_for_hash(LFMap *m, unsigned h) {
    return (size_t)(h & (m->capacity - 1)); // capacity is power-of-two
}

/* Insert or update (lock-free). Returns 1 on insert, 2 on update, 0 on failure */
int lfmap_insert(LFMap *m, void *key, void *value) {
    ebr_enter();

    unsigned h = m->hash(key);
    size_t idx = idx_for_hash(m, h);
    Entry *new_e = make_entry(key, value, 0);
    if (!new_e) { ebr_exit(); return 0; }

    size_t cap = m->capacity;
    for (size_t probes = 0; probes < cap; ++probes) {
        size_t i = (idx + probes) & (cap - 1);
        uintptr_t cur = atomic_load(&m->table[i]);

        if (cur == 0 || cur == TOMBSTONE) {
            uintptr_t expected = cur;
            if (atomic_compare_exchange_strong(&m->table[i], &expected, ptr_from_entry(new_e))) {
                atomic_fetch_add(&m->size, 1);
                ebr_exit();
                return 1; // inserted
            } else {
                // CAS failed, retry same slot
                probes--;
                continue;
            }
        }

        Entry *slot_e = entry_from_ptr(cur);

        if (m->key_eq(slot_e->key, key)) {
            // update value in-place
            void *oldv = atomic_exchange(&slot_e->value, value);
            // free new entry's key (caller gave ownership to map; we didn't insert new_e)
            if (m->free_key) m->free_key(new_e->key);
            free(new_e);
            if (m->free_value && oldv) m->free_value(oldv);
            ebr_exit();
            return 2; // updated
        }

        // Robin Hood: if current slot's dist < new's dist, swap (steal)
        if (slot_e->dist < new_e->dist) {
            uintptr_t expected = cur;
            if (atomic_compare_exchange_strong(&m->table[i], &expected, ptr_from_entry(new_e))) {
                // success: slot now holds new_e (owned by table), displaced 'slot_e' must be reinserted with dist+1
                Entry *displaced = dup_entry_with_dist(slot_e, slot_e->dist + 1);
                // retire original slot_e (since it's still present in memory but not referenced by table)
                // But we must keep slot_e memory alive until reclaimed: retire it
                ebr_retire_entry(slot_e);
                new_e = displaced;
                // continue with new_e
                continue;
            } else {
                probes--;
                continue;
            }
        }

        // otherwise advance
        new_e->dist++;
        continue;
    }

    // table full or couldn't insert
    // drop new_e (retire or free)
    if (m->free_key) m->free_key(new_e->key);
    if (m->free_value) {
        void *v = atomic_load(&new_e->value);
        if (v) m->free_value(v);
    }
    free(new_e);
    ebr_exit();
    return 0;
}

/* Get value pointer for key (returns stored value pointer or NULL) */
void *lfmap_get(LFMap *m, const void *key) {
    ebr_enter();
    unsigned h = m->hash(key);
    size_t idx = idx_for_hash(m, h);
    size_t cap = m->capacity;
    int dist = 0;

    for (size_t probes = 0; probes < cap; ++probes) {
        size_t i = (idx + probes) & (cap - 1);
        uintptr_t cur = atomic_load(&m->table[i]);

        if (cur == 0) { ebr_exit(); return NULL; }
        if (cur == TOMBSTONE) { dist++; continue; }

        Entry *slot_e = entry_from_ptr(cur);
        if (m->key_eq(slot_e->key, key)) {
            void *v = atomic_load(&slot_e->value);
            ebr_exit();
            return v;
        }
        if (dist > slot_e->dist) { ebr_exit(); return NULL; }
        dist++;
    }
    ebr_exit();
    return NULL;
}

/* Delete key (returns 1 on success, 0 not found) */
int lfmap_delete(LFMap *m, const void *key) {
    ebr_enter();
    unsigned h = m->hash(key);
    size_t idx = idx_for_hash(m, h);
    size_t cap = m->capacity;
    int dist = 0;

    for (size_t probes = 0; probes < cap; ++probes) {
        size_t i = (idx + probes) & (cap - 1);
        uintptr_t cur = atomic_load(&m->table[i]);

        if (cur == 0) { ebr_exit(); return 0; }
        if (cur == TOMBSTONE) { dist++; continue; }

        Entry *slot_e = entry_from_ptr(cur);
        if (m->key_eq(slot_e->key, key)) {
            uintptr_t expected = cur;
            if (atomic_compare_exchange_strong(&m->table[i], &expected, TOMBSTONE)) {
                atomic_fetch_sub(&m->size, 1);
                // retire the removed entry for later reclaim
                ebr_retire_entry(slot_e);
                ebr_exit();
                return 1;
            } else {
                probes--;
                continue;
            }
        }

        if (dist > slot_e->dist) { ebr_exit(); return 0; }
        dist++;
    }
    ebr_exit();
    return 0;
}

/* ---------------------- Example hash/eq/free for const char* -> int* ---------------------- */

unsigned str_hash(const void *p) {
    const char *s = p;
    unsigned h = 2166136261u;
    while (*s) { h ^= (unsigned char)*s++; h *= 16777619u; }
    return h;
}
int str_eq(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b) == 0;
}
void free_str(void *p) { free(p); }
void free_int(void *p) { free(p); }

/* ---------------------- Simple test (multi-thread) ---------------------- */

#ifdef TEST_MULTI
#define THREADS 4
#define OPS_PER_THREAD 10000

typedef struct {
    LFMap *m;
    int id;
} WorkerArg;

void *worker(void *arg) {
    WorkerArg *wa = arg;
    // register thread for EBR
    ebr_thread_register();

    LFMap *m = wa->m;
    int id = wa->id;
    char buf[64];
    for (int i = 0; i < OPS_PER_THREAD; ++i) {
        int k = id * OPS_PER_THREAD + i;
        snprintf(buf, sizeof(buf), "key-%d", k);
        char *ks = strdup(buf);
        int *v = malloc(sizeof(int));
        *v = k;
        lfmap_insert(m, ks, v);

        // occasionally lookup/delete
        if ((i & 7) == 0) {
            void *r = lfmap_get(m, ks);
            // don't free lookup result (map owns)
            (void)r;
        }
        if ((i & 31) == 0) {
            lfmap_delete(m, ks);
        }
    }
    ebr_thread_unregister();
    return NULL;
}

int main() {
    LFMap *m = lfmap_create(1<<16, str_hash, str_eq, free_str, free_int);
    pthread_t th[THREADS];
    WorkerArg args[THREADS];

    for (int i = 0; i < THREADS; ++i) {
        args[i].m = m; args[i].id = i;
        pthread_create(&th[i], NULL, worker, &args[i]);
    }
    for (int i = 0; i < THREADS; ++i) pthread_join(th[i], NULL);

    // final scan & destroy
    atomic_fetch_add(&global_epoch, 1);
    ebr_scan();

    lfmap_destroy(m);
    printf("done\n");
    return 0;
}
#endif

/* For single-thread quick test:
 * compile with -DTEST_SINGLE and run
 */
#ifdef TEST_SINGLE
int main() {
    LFMap *m = lfmap_create(64, str_hash, str_eq, free_str, free_int);

    ebr_thread_register();

    char *k = strdup("alice");
    int *v = malloc(sizeof(int)); *v = 42;
    lfmap_insert(m, k, v);

    int *r = lfmap_get(m, "alice");
    printf("alice -> %d\n", r ? *r : -1);

    lfmap_delete(m, "alice");
    r = lfmap_get(m, "alice");
    printf("after delete: %s\n", r ? "found" : "not found");

    ebr_thread_unregister();

    atomic_fetch_add(&global_epoch, 1);
    ebr_scan();

    lfmap_destroy(m);
    return 0;
}
#endif