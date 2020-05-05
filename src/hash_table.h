/*
 * Hash table for storing Module's information
 */
#ifndef HASH_TABLE_H
    #define HASH_TABLE_H
    /* typedefs */
    typedef void (*value_free_fn)(void *);

    /* structs */
    // Item of Hash table
    typedef struct {
        char *key;   /* key is represented only by string */
        void *value; /* Module's struct */
    } HT_Item;

    // Hash table
    typedef struct {
        int  size;             /* maximum number of elements (can be extended) */
        int  count;            /* actual number of elements */
        value_free_fn handler; /* handles stored values */
        HT_Item **items;       /* actual items */
    } Hash_table;

    /* funcs */
    Hash_table* new_hash_table(int size, value_free_fn handler);              /* Returns new hash table with given size*/
    void*       ht_search_item(Hash_table *ht, const char *key);              /* Get value by key */
    void        ht_insert_item(Hash_table *ht, const char *key, void *value); /* Insert value by key */
    void        ht_delete_item(Hash_table *ht, const char *key);              /* Deletes value by key */
    void        delete_hash_table(Hash_table *ht);                            /* Deletes Hash table*/
#endif
