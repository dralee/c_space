/**
 * Robin Hood Hashing（开放寻址）
特点：
开放寻址（线性 probing）+ Robin Hood 规则
key/value 均为 void*（泛型）
使用“距离（probe distance）”来决定抢位置
使用 TOMBSTONE（墓碑）标记删除
自动扩容（load factor > 0.7）
 *
 * 2025.12.12 by dralee
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 8
#define LOAD_FACTOR_LIMIT 0.7

typedef unsigned (*hash_func)(const void*);
typedef int (*key_eq_func)(const void*, const void*);
typedef void (*free_func)(void*);

typedef enum { EMPTY, FILLED, TOMBSTONE } EntryState;

typedef struct {
        void* key;
        void* value;
        int dist;         // probe distance
        EntryState state; // EMPTY FILLED TOMBSTONE
} Entry;

typedef struct {
        Entry *table;
        int size;
        int capacity;

        hash_func hash;
        key_eq_func key_eq;

        free_func free_key;
        free_func free_value;
} RobinHoodHash;

static void rh_resize(RobinHoodHash *rh);
static void entry_free(RobinHoodHash *rh, Entry *entry);

RobinHoodHash *rh_create(hash_func hash, key_eq_func key_eq, 
    free_func free_key, free_func free_value) {
    RobinHoodHash *rh = malloc(sizeof(RobinHoodHash));
    rh->size = 0;
    rh->capacity = INITIAL_CAPACITY;
    rh->table = calloc(rh->capacity, sizeof(Entry));

    rh->hash = hash;
    rh->key_eq = key_eq;
    rh->free_key = free_key;
    rh->free_value = free_value;

    return rh;
}


// insert / update
void rh_insert(RobinHoodHash *rh, void* key, void* value) {
    if ((float)rh->size / rh->capacity > LOAD_FACTOR_LIMIT) {
        rh_resize(rh);
    }

    unsigned hash = rh->hash(key);
    int index = hash % rh->capacity;
    
    Entry new_entry = {key, value, 0, FILLED};

    while (1) {
        Entry *slot = &rh->table[index];

        if (slot->state == EMPTY || slot->state == TOMBSTONE) {
            // place new entry
            *slot = new_entry;
            rh->size++;
            return;
        }

        if (rh->key_eq(slot->key, key) && slot->state == FILLED) {
            // update value
            if(rh->free_value) {
                rh->free_value(slot->value);
            }
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

const void* rh_get(RobinHoodHash *rh, const void* key) {
    unsigned hash = rh->hash(key);
    int index = hash % rh->capacity;
    int dist = 0;

    while (1) {
        Entry *slot = &rh->table[index];

        if (slot->state == EMPTY) {
            return NULL;
        }
        if (slot->state == FILLED && rh->key_eq(slot->key, key)) {
            return slot->value;
        }

        if (dist > slot->dist) {
            // can't be further
            return NULL;
        }

        index = (index + 1) % rh->capacity;
        dist++;
    }
}

int rh_delete(RobinHoodHash *rh, const void* key) {
    unsigned hash = rh->hash(key);
    int index = hash % rh->capacity;
    int dist = 0;

    while (1) {
        Entry *slot = &rh->table[index];

        if (slot->state == EMPTY) {
            return 0; // not found
        }

        if (slot->state == FILLED && rh->key_eq(slot->key, key)) {
            entry_free(rh, slot);
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
    for(int i = 0; i < rh->capacity; i++){
        if(rh->table[i].state != FILLED) continue;
        if(rh->free_key){
            rh->free_key(rh->table[i].key);
        }
        if(rh->free_value){
            rh->free_value(rh->table[i].value);
        }
    }
    free(rh->table);
    free(rh);
}

static inline void entry_free(RobinHoodHash *rh, Entry *entry) {
    if (rh->free_key) {
        rh->free_key(entry->key);
    }
    if (rh->free_value) {
        rh->free_value(entry->value);
    }
}

/*
* example for key=string, value=int*
*/
unsigned str_hash(const void *p){
    const char *s = p;
    unsigned h = 5381;
    while (*s)
    {
        h = ((h << 5) + h) + *s; /* hash * 33 + c */
        s++;
    }
    return h;
}

int str_eq(const void *a, const void *b){
    return strcmp((const char*)a, (const char*)b) == 0;
}

void free_int(void *p){ free(p); }
void free_str(void *p){ free(p); }

int main() {
    RobinHoodHash *rh = rh_create(str_hash, str_eq, free_str, free_int);
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
    rh_free(rh);
    return 0;
}