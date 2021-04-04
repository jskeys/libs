#include "hash_table.h"

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

const uint64_t k_mask = ~0ul >> 60;

// www.cse.yorku.ca/~oz/hash.html
uint64_t hash(const char *str)
{
    uint64_t hash = 5381;
    while (*str++)
    {
        hash = ((hash << 5) + hash) + (uint64_t)*str;
    }
    return hash;
}

// Traverse a linked list starting at `item` until `key` is found or the end of
// the list is reached. If `key` is found, 1 is returned and `item` points to
// the corresponding item. If `0` is returned, `item` points to the last item
// in the list. Passing a NULL ptr is allowed, but is probably a waste of time.
uint8_t traverse(LL_Item_t **const item, const char *key)
{
    while (*item)
    {
        if (!strcmp((*item)->key, key))
        {
            return 1;
        }
        else if (!(*item)->next)
        {
            return 0;
        }
        else
        {
            *item = (*item)->next;
        }
    }
    return 0;
}

// Remove item from table
Status_t HT_Remove(LL_Item_t *table[], char *key)
{
    LL_Item_t *item, *list;

    int index = (int)(hash(key) & k_mask);
    list = table[index];
    item = list;

    // If item is found update the `next` field of the preceding item and the
    // `prev` field of the subsequent element. Additionally, if the item is the
    // first one in the list, then update the hash table.
    if (traverse(&item, key))
    {
        if (item->prev)
        {
            (item->prev)->next = item->next;
        }
        if (item->next)
        {
            (item->next)->prev = item->prev;
        }
        if (item == list)
        {
            table[index] = item->next;
        }
        free(item);
    }
    return 0;
}

// Insert a key-val pair into the table or update the val of an existing
// key-val pair. Returns `STATUS_NULL_PTR` if either `key` or `val` is NULL.
Status_t HT_Insert(LL_Item_t *table[], char *key, char *val)
{
    if (!key || !val)
    {
        return STATUS_NULL_PTR;
    }

    LL_Item_t *item, *list;

    int index = (int)(hash(key) & k_mask);
    list = table[index];

    if (!list)
    {
        item = (LL_Item_t *)malloc(sizeof(LL_Item_t));
        item->prev = NULL;
        item->next = NULL;
        item->key = key;
        item->val = val;
        table[index] = item;
    }
    else
    {
        if (traverse(&list, key))
        {
            list->val = val;
        }
        else
        {
            item = (LL_Item_t *)malloc(sizeof(LL_Item_t));
            list->next = item;
            item->prev = list;
            item->next = NULL;
            item->key = key;
            item->val = val;
        }
    }

    return STATUS_OKAY;
}
