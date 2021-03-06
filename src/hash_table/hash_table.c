/*
 * Implementation of Hash table
 */
// -------------------------------- Headers -------------------------------------
#include "hash_table.h" /* Hash table declarations */

#include <stdlib.h>     /* Working with heap */ 
#include <string.h>     /* Working with strings */ 
#include <math.h>       /* Working with math functions */ 

// ------------------------------- Prototypes -----------------------------------
HT_Item* new_item(const char *, void *);
void* free_item(HT_Item *);
int   get_hash(const char *, int , int );
int   get_index_ht(const char *, int, int);
void  resize_hash_table(Hash_table *, int );
void  resize_up(Hash_table *);
void  resize_down(Hash_table *);

// ----------------------------- Global values ----------------------------------
static HT_Item HT_DELETED_ITEM = {NULL, NULL};

// ------------------------------- Main funcs -----------------------------------
/*
 * Returns new Hash table
 * Returns NULL if error is occured
 */
Hash_table*
new_hash_table(int size, value_free_fn handler) {
    Hash_table *ht = (Hash_table *) malloc(sizeof(Hash_table));

    ht->size = size;                                         /* initial size */
    ht->count = 0;                                           /* initial number of elements*/
    ht->items = calloc((size_t) ht->size, sizeof(HT_Item*)); /* initialize array */
    ht->handler = handler;                                   /* value handler */

    return ht;
}

/*
 * Get HT_Item's value from Hash table
 * If key doesn't exist, return NULL
 */
void*
ht_search_item(Hash_table *ht, const char *key) {
    // Get index in Hash table
    int index = get_index_ht(key, ht->size, 0);

    // Get HT_Item by index
    HT_Item *it = ht->items[index];

    // Search HT_Item
    int i = 1;
    while (it != NULL) {
        if (it != &HT_DELETED_ITEM) {
            if (strcmp(it->key, key) == 0) {
                return it->value;
            }
        }

        index = get_index_ht(key, ht->size, i);
        it = ht->items[index];
        i++;
    }

    // HT_Item doesn't exist
    return NULL;
}

/*
 * Inserts HT_Item by provided key in provided Hash table
 */
void
ht_insert_item(Hash_table *ht, const char *key, void *value) {
    // Check for load
    int load = ht->count * 100 / ht->size;

    // If more than 70 percent is occupied
    // Increase size of Hash table
    if (load > 70) {
        resize_up(ht);
    }

    // Create new HT_Item
    HT_Item *it = new_item(key, value);

    // Get index by provided key
    int index = get_index_ht(it->key, ht->size, 0);

    // Get HT_Item in provided key
    HT_Item *cur_it = ht->items[index];
    int i = 1;
    // If current HT_Item is not NULL
    // Iterate to find new place where to put new item
    while ((cur_it != NULL) && (cur_it != &HT_DELETED_ITEM)) {
        index = get_index_ht(it->key, ht->size, i);
        cur_it = ht->items[index];
        i++;
    }

    ht->items[index] = it;
    ht->count++;
}

/*
 * Deletes HT_Item
 */
void
ht_delete_item(Hash_table *ht, const char *key) {
    // Check for load
    int load = ht->count * 100 / ht->size;
    
    // If less than 10 percent is occupied
    // Decrease size of Hash table
    if (load < 10) {
        resize_down(ht);
    }

    // Get HT_Item
    int index = get_index_ht(key, ht->size, 0);
    HT_Item *it = ht->items[index];

    void *val; /* here will be stored value */
    int i = 1;
    // If there was a collision
    // Try again!
    while (it != NULL) {
        if (it != &HT_DELETED_ITEM) {
            if (strcmp(it->key, key) == 0) {
                val = free_item(it);

                if (ht->handler != NULL) {
                    ht->handler(val);
                }

                ht->items[index] = &HT_DELETED_ITEM;
                ht->count--; 
            }
        }

        index = get_index_ht(key, ht->size, i);
        it = ht->items[index];
        i++;
    }
}

/*
 * Deletes Hash table
 * Free all HT_Items without returning their value
 * Then free Hash table
 */
void
delete_hash_table(Hash_table *ht) {
    void *val; /* here will be stored values */
    // Delete all HT_Items 
    for(int i = 0; i < ht->size; i++) {
        // Get next item
        HT_Item *it = ht->items[i];
        // If HT_Item exist, delete it
        if (it != NULL) {
            val = free_item(it);

            if (ht->handler != NULL) {
                ht->handler(val);
            }
        }
    }

    free(ht->items); /* Free array */
    free(ht);        /* Free Hash table */
}

// ---------------------------- Additional funcs --------------------------------
/*
 * New HT_Item for hash table
 */
HT_Item*
new_item(const char *key, void *value) {
    HT_Item *it = (HT_Item *) malloc(sizeof(HT_Item));
    it->key = strdup(key); /* copy key */
    it->value = value;

    return it;
}

/*
 * Free HT_Item for hash table
 */
void*
free_item(HT_Item *it) {
    void *value = it->value; /* store value */

    // Free copied key and memory
    free(it->key);
    free(it);

    return value;
}

/*
 * Returns hash of string
 * Reduce the size of the result hash to index in hash table
 * Prime number here must be larger than number of characters in used alphabet (ASCII -> prime > 128)
 */
int
get_hash(const char *str, int prime, int ht_size) {
    long hash = 0; /* result hash */

    int str_len = strlen(str);
    for (int i = 0; i < str_len; i++) {
        // Calculate hash
        hash += (long) pow(prime, str_len - (i+1)) * str[i];

        // Reduce to index in hash table
        hash %= ht_size;
    }

    return (int) hash;
}

/*
 * Get index in hash table
 * To deal with collisions using open address method with double hashing
 */
int
get_index_ht(const char *str, int ht_size, int attempt) {
    int hash_a = get_hash(str, 487, ht_size);
    int hash_b = get_hash(str, 439, ht_size);
    return (hash_a + (attempt * (hash_b + 1))) % ht_size;
}

/*
 * Resize Hash table to given size
 */
void
resize_hash_table(Hash_table *ht, int size) {
    // Creating new Hash table
    Hash_table *new_ht = new_hash_table(size, ht->handler);

    // Copying HT_Items to new Hash table
    // If not NULLs and deleted
    for(int i = 0; i < ht->size; i++) {
        HT_Item *it = ht->items[i];
        if (it != NULL && it != &HT_DELETED_ITEM) {
            ht_insert_item(new_ht, it->key, it->value);
        }
    }
    
    // Change pointer to new Hash table
    Hash_table *tmp_ht = ht;
    ht = new_ht;

    // Delete old Hash table
    delete_hash_table(tmp_ht);
}

/*
 * Resize Hash table up
 */
void
resize_up(Hash_table *ht) {
    int new_size = ht->size * 2;
    resize_hash_table(ht, new_size);
}

/*
 * Resize Hash table down
 */
void
resize_down(Hash_table *ht) {
    int new_size = ht->size / 2;
    resize_hash_table(ht, new_size);
}
