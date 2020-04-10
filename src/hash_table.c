/*
 * Implementation of hash table
 */
// -------------------------------- Headers -------------------------------------
#include "hash_table.h" /* Hash table declarations */
#include <stdlib.h>     /* Working with heap */ 

// ------------------------------- Prototypes -----------------------------------
Item* new_item(const char *, void *);
void* free_item(Item *);

// ------------------------------- Main funcs -----------------------------------
/*
 * Returns new Hash table
 * Returns NULL if error is occured
 */
Hash_table*
new_hash_table() {
    Hash_table *ht = (Hash_table *) malloc(sizeof(Hash_table));

    ht->size = 10;                                        /* initial size */
    ht->count = 0;                                        /* initial number of elements*/
    ht->items = calloc((size_t) ht->size, sizeof(Item*)); /* initialize array */
    return ht;
}

/*
 * Get Item's value from Hash table
 * If key doesn't exist, return NULL
 */
void*
get_item_value(Hash_table *ht, const char *key) {
    return NULL;
}

/*
 * Updates Item by provided key in provided Hash table
 * If key doesn't exist, create new Item with given value in hash table
 * Returns -1 if was an error
 */
int
update_item(Hash_table *ht, const char *key, void *value) {
    return 0;
}

/*
 * Deletes Item and returns stored value 
 * If key doesn't exist, returns NULL
 */
void*
delete_item(Hash_table *ht, const char *key) {
    return NULL;
}

/*
 * Deletes Hash table
 * Free all Items without returning their value
 * Then free Hash table
 */
void
delete_hash_table(Hash_table *ht) {
    // Delete all Items 
    for(int i = 0; i < ht->size; i++) {
        // Get next item
        Item *it = ht->items[i];
        // If Item exist, delete it
        if (item != NULL) {
            delete_item(it);
        }
    }

    free(ht->items); /* Free array */
    free(ht);        /* Free Hash table */
}
// ---------------------------- Additional funcs --------------------------------
/*
 * New Item for hash table
 */
Item*
new_item(const char *key, void *value) {
    Item *it = (Item *) malloc(sizeof(Item));
    it->key = strdup(key); /* copy key */
    it->value = value;
    return it;
}

/*
 * Free Item for hash table
 */
void*
free_item(Item *it) {
    void *value = it->value; /* store value */

    // Free copied key and memory
    free(it->key);
    free(it);

    return value;
}
