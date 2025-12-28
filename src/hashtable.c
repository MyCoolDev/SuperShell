#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

unsigned long djb2_hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash;
}

hashtable_t *ht_create(size_t size)
{
    hashtable_t *ht;
    size_t i;

    ht = (hashtable_t *)malloc(sizeof(hashtable_t) + sizeof(entry_t *) * size);
    if (ht == NULL)
        return NULL;

    ht->size = size;

    for (i = 0; i < size; i++)
        ht->values[i] = NULL;

    return ht;
}

int ht_set(hashtable_t *ht, const char *key, char *value)
{
    unsigned long hash;
    int index;
    entry_t *curr;

    hash = djb2_hash((unsigned char *)key);
    index = hash % ht->size;

    curr = ht->values[index];

    while (curr != NULL)
    {
        if (strcmp(curr->key, key) == 0)
        {
            free(curr->value);
            curr->value = value;
            return 0;
        }

        curr = curr->next;
    }

    entry_t *new_entry;

    new_entry = (entry_t *)malloc(sizeof(entry_t));
    if (new_entry == NULL)
        return -1;

    /* add to start */
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = ht->values[index];

    ht->values[index] = new_entry;

    return 0;
}

entry_t *ht_get_entry(hashtable_t *ht, char *key)
{
    unsigned long hash;
    int index;
    entry_t *curr;

    hash = djb2_hash((unsigned char *)key);
    index = hash % ht->size;

    curr = ht->values[index];

    while (curr != NULL)
    {
        if (strcmp(curr->key, key) == 0)
        {
            return curr;
        }

        curr = curr->next;
    }

    return NULL;
}

void delete_entry(hashtable_t *ht, char *key)
{
    unsigned long hash;
    int index;
    entry_t *curr;

    hash = djb2_hash((unsigned char *)key);
    index = hash % ht->size;
    curr = ht->values[index];

    if (curr == NULL)
        return;

    if (strcmp(curr->key, key) == 0)
    {
        ht->values[index] = curr->next;
        free(curr->key);
        free(curr->value);
        free(curr);
        return;
    }
    
    entry_t *temp;

    while (curr->next != NULL)
    {
        if (strcmp(curr->next->key, key) == 0)
        {
            temp = curr->next;
            curr->next = temp->next;
            free(temp->key);
            free(temp->value);
            free(temp);
            return;
        }
    }
}

void clear_ht(hashtable_t *ht)
{
    if (ht == NULL)
        return;

    hashtable_t *ptr;
    entry_t *curr;
    size_t i;

    ptr = ht;
    for (i = 0; i < ht->size; ++i)
    {
        entry_t *temp;

        curr = ht->values[i];
        while (curr != NULL)
        {
            temp = curr;
            curr = curr->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }

    free(ht);
}
