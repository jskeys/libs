#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        STATUS_OKAY,
        STATUS_KEY_NOT_FOUND,
        STATUS_NULL_PTR,
    } Status_t;

    struct LL_Item
    {
        struct LL_Item *next;
        struct LL_Item *prev;
        char *key;
        void *val;
    };

    typedef struct LL_Item LL_Item_t;

    Status_t ht_get(char *key);
    Status_t ht_insert(LL_Item_t **table, char *key, char *value);
    Status_t ht_remove(LL_Item_t **table, char *key);

#ifdef __cplusplus
}
#endif

#endif
