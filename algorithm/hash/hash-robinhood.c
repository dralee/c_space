/**
 * Robin Hood Hashing（开放寻址）
特点：
开放寻址（线性 probing）+ Robin Hood 规则
key/value 均为 int（你可以替换成泛型）
使用“距离（probe distance）”来决定抢位置
使用 TOMBSTONE（墓碑）标记删除
自动扩容（load factor > 0.7）
 *
 * 2025.12.11 by dralee
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 8
#define LOAD_FACTOR_LIMIT 0.7

typedef enum { EMPTY, FILLED, TOMBSTONE } EntryState;

typedef struct {
        int key;
        int value;
        int dist;         // probe distance
        EntryState state; // EMPTY FILLED TOMBSTONE
} Entry;

typedef struct {
        Entry *table;
        int size;
        int capacity;
} RobinHoodHash;

// simple hash
static inline unsigned int hash_int(int x) {
    // mix bits
    x ^= x >> 16;
    x *= 0x7feb352d;
    x ^= x >> 15;
    x *= 0x96e6a2a7;
    x ^= x >> 16;
    return x;
}

RobinHoodHash *rh_create() {
    RobinHoodHash *rh = malloc(sizeof(RobinHoodHash));
    rh->size = 0;
    rh->capacity = INITIAL_CAPACITY;
    rh->table = calloc(rh->capacity, sizeof(Entry));
    return rh;
}

static void rh_resize(RobinHoodHash *rh);

// insert / update
void rh_insert(RobinHoodHash *rh, int key, int value) {
    if ((float)rh->size / rh->capacity > LOAD_FACTOR_LIMIT) {
        rh_resize(rh);
    }

    unsigned hash = hash_int(key);
    int index = hash % rh->capacity;
    int dist = 0;

    Entry new_entry = {key, value, dist, FILLED};

    while (1) {
        Entry *slot = &rh->table[index];

        if (slot->state == EMPTY || slot->state == TOMBSTONE) {
            // place new entry
            *slot = new_entry;
            rh->size++;
            return;
        }

        if (slot->key == key && slot->state == FILLED) {
            // update value
            slot->value = value;
            return;
        }

        // robin hood step: if incoming entry's distance is greater, swap
        if (new_entry.dist > slot->dist) {
            // swap
            Entry tmp = *slot;
            *slot = new_entry;
            new_entry = tmp;
        }

        // move to next slot
        index = (index + 1) % rh->capacity;
        new_entry.dist++;
    }
}

int *rh_get(RobinHoodHash *rh, int key) {
    unsigned hash = hash_int(key);
    int index = hash % rh->capacity;
    int dist = 0;

    while (1) {
        Entry *slot = &rh->table[index];

        if (slot->state == EMPTY) {
            return NULL;
        }
        if (slot->state == FILLED && slot->key == key) {
            return &slot->value;
        }

        if (dist > slot->dist) {
            // can't be further
            return NULL;
        }

        index = (index + 1) % rh->capacity;
        dist++;
    }
}

int rh_delete(RobinHoodHash *rh, int key) {
    unsigned hash = hash_int(key);
    int index = hash % rh->capacity;
    int dist = 0;

    while (1) {
        Entry *slot = &rh->table[index];

        if (slot->state == EMPTY) {
            return 0; // not found
        }

        if (slot->state == FILLED && slot->key == key) {
            slot->state = TOMBSTONE;
            rh->size--;
            return 1;
        }

        if (dist > slot->dist) {
            return 0;
        }

        index = (index + 1) % rh->capacity;
        dist++;
    }
}

static void rh_resize(RobinHoodHash *rh) {
    int old_capacity = rh->capacity;
    Entry *old_table = rh->table;

    rh->capacity *= 2;
    rh->size = 0;
    rh->table = calloc(rh->capacity, sizeof(Entry));

    // reinsert all filled entries
    for (int i = 0; i < old_capacity; ++i) {
        if (old_table[i].state == FILLED) {
            rh_insert(rh, old_table[i].key, old_table[i].value);
        }
    }

    free(old_table);
}

void rh_free(RobinHoodHash *rh) {
    free(rh->table);
    free(rh);
}

int main() {
    RobinHoodHash *rh = rh_create();
    for (int i = 0; i < 20; ++i) {
        rh_insert(rh, i, i * 10);
    }

    int *val = rh_get(rh, 10);
    if (val) {
        printf("key 10 -> %d\n", *val);
    }

    rh_delete(rh, 10);
    val = rh_get(rh, 10);
    printf("after delete: key 10 -> %s\n", val ? "found" : "not found");

    rh_free(rh);
    return 0;
}