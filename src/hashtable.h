#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stddef.h>

typedef struct entry_s {
    char *key;
    char *value; /* the path to the file for exec next */
    struct entry_s *next;
} entry_t;

typedef struct {
    size_t size;
    entry_t *values[];
} hashtable_t;

unsigned long djb2_hash(unsigned char *str);
int ht_set(hashtable_t *ht, const char *key, char *value);
entry_t *ht_get_entry(hashtable_t *ht, char *key);
void delete_entry(hashtable_t *ht, char *key);
void clear_ht(hashtable_t *ht);

#endif
