/*
 * Implementation of hash table
 */
// -------------------------------- Headers -------------------------------------
#include "hash_table.h" /* Hash table declarations */
#include <stdlib.h>     /* Working with heap */ 

// ------------------------------- Prototypes -----------------------------------
Item* new_item(const char *, void *);

// ------------------------------- Main funcs -----------------------------------
/*
 * Returns new Hash table
 * Returns NULL if error is occured
 */
Hash_table*
new_hash_table() {
    return NULL;
}

/*
 * Get Item from Hash table
 * If key doesn't exist, return NULL
 */
void*
get_item_value(Hash_table* ht, const char *key) {
    return NULL;
}

/*
 * Updates Item by provided key in provided Hash table
 * If key doesn't exist, create new Item with given value in hash table
 * Returns -1 if was an error
 */
int
update_item(Hash_table* ht, const char *key, void *value) {
    return 0;
}

/*
 * Deletes Item and returns stored value by it
 * If key doesn't exist, returns NULL
 */
void*
delete_item(Hash_table* ht, const char *key) {
    return NULL;
}

/*
 * Deletes Hash table
 */
void
delete_hash_table(Hash_table* ht) {
}
// ---------------------------- Additional funcs --------------------------------
/*
 * New item for hash table
 */
Item*
new_item(const char *key, void *value) {
    return NULL;
}
