/*
 * Hash table for storing Module's information
 */
#ifndef HASH_TABLE_H
    #define HASH_TABLE_H
    /* structs */
    // Item of Hash table
    typedef struct hash_table_item {
        char *key;   /* key is represented only by string */
        void *value; /* Module's struct */
    } Item;

    // Hash table
    typedef struct hash_table {
        int  size;    /* maximum number of elements (can be extended) */
        int  count;   /* actual number of elements */
        Item **items; /* actual items */
    } Hash_table;

    /* funcs */
    Hash_table* new_hash_table(int size);                                  /* Returns new hash table with given size*/
    void*       search_item(Hash_table *ht, const char *key);              /* Get value by key */
    int         insert_item(Hash_table *ht, const char *key, void *value); /* Insert value by key */
    void*       delete_item(Hash_table *ht, const char *key);              /* Deletes value by key and returns it */
    void        delete_hash_table(Hash_table *ht);                         /* Deletes Hash table*/
#endif
