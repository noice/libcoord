/*
 * Hash table for storing Module's information
 */
#ifndef HASH_TABLE.H
    #define HASH_TABLE.H
    /* structs */
    // Item of Hash table
    typedef struct hash_table_item {
        char *key;   /* key is represented only by string */
        void *value; /* value can be any type (will be struct Module) */
    } Item;

    // Hash table
    typedef struct hash_table {
        int  size;    /* */
        int  count;   /* */
        Item **items; /* */
    } Hash_table;

    /* funcs */
    Hash_table* new_hash_table();                                          /* Returns new hash table */
    void*       get_item_value(Hash_table *ht, const char *key);           /* Get Item by key */
    int         update_item(Hash_table *ht, const char *key, void *value); /* Updates value by key (add new item if new key provided) */
    void*       delete_item(Hash_table *ht, const char *key);              /* Deletes item and return value */
    void        delete_hash_table(Hash_table* ht);                         /* Deletes Hash table*/
#endif
