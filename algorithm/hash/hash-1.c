/** 哈希冲突：通过链表解决
 * 2025.12.11 by dralee
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _node {
    char *key;
    char *value;
    struct _node *next;
} node;

#define HASH_SIZE 101
static node *hash_table[HASH_SIZE];

void init_hash_table() {
    for (int i = 0; i < HASH_SIZE; i++) {
        hash_table[i] = NULL;
    }
}

unsigned int hash(char *key) {
    unsigned int hash_value = 0;
    for (;*key; key++) {        
        hash_value = *key + hash_value * 31;
    }
    return hash_value % HASH_SIZE;
}

node* lookup(char *key) {
    unsigned int hash_value = hash(key);
    node *p = hash_table[hash_value];
    while (p != NULL) {
        if (strcmp(p->key, key) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

char* m_strdup(const char *s) {
    char *p = malloc(strlen(s) + 1);
    strcpy(p, s);
    return p;
}

char* get(char *key) {
    node* n = lookup(key);
    if(n == NULL) {
        return NULL;
    }
    return n->value;
}

int install(char *key, char *value) {
    unsigned int hash_value;
    node* np;
    if((np = lookup(key)) == NULL) {
        hash_value = hash(key);
        np = (node*)malloc(sizeof(node));
        if(np == NULL)
            return 0;
        np->key = m_strdup(key);
        if(np->key == NULL){
            return 0;
        }
        np->next = hash_table[hash_value];
        hash_table[hash_value] = np;
    }else{
        free(np->value);
    }
    
    np->value = m_strdup(value);
    if(np->value == NULL){
        return 0;
    }
    
    return 1;
}

void display() {
    printf("=============== hash table ===============\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        node *p = hash_table[i];
        if(p == NULL) {
            printf("()");
            continue;
        }
        printf("(");
        while (p != NULL) {
            printf("(%s.%s)", p->key, p->value);
            p = p->next;
        }
        printf(".)\r\n");
    }
    printf("\r\n");
}

void clean(){
    node* t;
    for (int i = 0; i < HASH_SIZE; i++) {
        node *p = hash_table[i];
        while (p != NULL) {
            t = p->next;
            free(p->key);
            free(p->value);
            free(p);
            p = t;
        }
    }
}

int main(){
    int i;
    char* names[] = {"hello","world","this","k1230","g31489x"};
    char* values[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday"};
    init_hash_table();
    for(i=0;i<5;i++){
        install(names[i],values[i]);
    }
    
    display();
    printf("the value of hello is %s\n",get("hello"));
    printf("the value of world is %s\n",get("world"));
    printf("the value of this is %s\n",get("this"));
    printf("the value of k1230 is %s\n",get("k1230"));
    printf("the value of g31489x is %s\n",get("g31489x"));

    install("world", "20251211-hash");
    printf("the value of world is %s\n",get("world"));

    clean();
    return 0;
}