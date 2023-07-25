#ifndef __MYPQUEUEH__
#define __MYPQUEUEH__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define pqueue_elt_type int
#define pqueue_elt_fmt "%d"
#define pqueue_key_type double
#define pqueue_key_fmt "%lf"


typedef struct pqueue pqueue;

// constructeur
pqueue  *pqueue_create(int capacity, bool reversed_order);

// destructeur
void     pqueue_free(pqueue **addr_q);

// accesseurs
bool     pqueue_is_empty(pqueue *q);
int      pqueue_size(pqueue *q);
void     pqueue_get_arrays(pqueue *q, pqueue_key_type **addr_tab_key, pqueue_elt_type **addr_tab_val, int *qsize);
  
// transformateurs
void     pqueue_enqueue(pqueue *q, pqueue_key_type k, pqueue_elt_type v);
pqueue_elt_type pqueue_dequeue(pqueue *q, pqueue_key_type *addr_key);

#endif
