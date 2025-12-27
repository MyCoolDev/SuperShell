#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stddef.h>

typedef struct entry_s {
    char *key;
    void* value;
    struct entry_s *next;
} entry_t;

typedef struct {
    size_t size;
    entry_t *values[];
} hashtable_t;

unsigned long djb2_hash(unsigned char *str);
int ht_set(hashtable_t *ht, char *key, void *value);
entry_t *get_value(hashtable_t *ht, char *key);
void delete_entry(hashtable_t *ht, char *key);

#endif
