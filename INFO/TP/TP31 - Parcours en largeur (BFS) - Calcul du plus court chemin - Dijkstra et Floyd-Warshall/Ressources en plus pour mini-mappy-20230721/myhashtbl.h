#ifndef __MYHASHTBLH__
#define __MYHASHTBLH__
#include <stdint.h>

#include "myhashtbl-lst.h"

typedef struct hashtbl hashtbl;

hashtbl *hashtbl_create(uint64_t n_buckets);
void hashtbl_add(hashtbl *ht, char *k, myhashtbl_elt_type v);
bool hashtbl_find(hashtbl *ht, char *k, myhashtbl_elt_type *addr_v);
bool hashtbl_replace(hashtbl *ht, char *k, myhashtbl_elt_type v);
bool hashtbl_remove(hashtbl *ht, char *k);
void hashtbl_print(hashtbl *h);
void hashtbl_free(hashtbl **addr_ht);
uint64_t hashtbl_number_of_bindings(hashtbl *ht);
uint64_t hashtbl_stats(hashtbl *ht);

#endif
