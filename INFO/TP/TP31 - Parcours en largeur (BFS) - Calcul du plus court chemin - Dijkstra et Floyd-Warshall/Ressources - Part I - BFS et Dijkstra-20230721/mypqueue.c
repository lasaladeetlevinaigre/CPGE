#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "mypqueue.h"

struct pqueue
{
  bool reversed_order;
  int size;
  int capacity;
  pqueue_key_type *tab;
  pqueue_elt_type *val;
};

typedef struct pqueue pqueue;
typedef pqueue heap;

// exercice 2: creation d'un tas et insertion dans un tas

// On donne en entrée de la fonction la capacité maximale du tableau qui servira à stocker les étiquettes du tas
heap *heap_create(int capacity, bool reversed_order)
{
  heap *t = malloc(sizeof(heap));
  t->reversed_order = reversed_order;
  t->capacity = capacity;
  t->size = 0;
  t->tab = calloc(capacity, sizeof(pqueue_key_type));
  t->val = calloc(capacity, sizeof(pqueue_elt_type));
  return t;
}

// libération de l'espace alloué dans le tas
void heap_free(heap **addr_t)
{
  assert(addr_t != NULL);
  heap *t = *addr_t;

  if (t != NULL)
  {
    if (t->tab != NULL)
      free(t->tab);
    if (t->val != NULL)
      free(t->val);
    free(t);
  }
  *addr_t = NULL;
  return;
}



void aux_heap_print_infixe(heap *t, int l)
{
  if (l >= t->size)
    return;
  aux_heap_print_infixe(t, 2*l+1);
  printf("(");printf(pqueue_key_fmt, t->tab[l]);printf(","); printf(pqueue_elt_fmt, t->val[l]);printf(") ");
  aux_heap_print_infixe(t, 2*l+2);
  return;
}

void heap_print_infixe(heap *t)
{
  printf("Tas (parcours infixe):\n\t");
  aux_heap_print_infixe(t, 0);printf("\n");
  return;
}

bool heap_is_full(heap *t)
{
  return (t->size >= t->capacity);
}

void move_up(heap *t, int i, pqueue_key_type k, pqueue_elt_type v) // reverse_order: cf exo heap_sort
{

  assert(i >= 0);
  
  int j;
  //printf("Inserer (%d %lf)\n", k, v);
  while (i > 0)
  {
   
    j = (i-1)/2; // indice du père
    //printf("i = %d j = %d pere=%lf v= %lf\n", i, j, t->tab[j], k);
    if (    (t->reversed_order == true && t->tab[j] < k) // si le père et le fils sont rangés dans le bon ordre, on stope la remontée
	 || (t->reversed_order == false && t->tab[j] > k) )
      break;
    t->val[i] = t->val[j];
    t->tab[i] = t->tab[j]; // descente fils <-> du père dans le fils
    i = j;
  }

  t->tab[i] = k; // on place la clé dans le noeud où l'on s'est arrêté
  t->val[i] = v; // et la valeur associee dans le tableau associatif
  return;
}

void heap_insert(heap *t, pqueue_key_type k, pqueue_elt_type v)
{
  if (heap_is_full(t) == true)
  {
    fprintf(stderr, "Impossible d'inserer: le tas a atteint sa capacite maximale\n");
    printf("Capacité max: %d\n", t->capacity);
    printf("Insertion ");
    printf(pqueue_key_fmt, k);
    printf(" ");
    printf(pqueue_elt_fmt, v);
    printf("\n");
    exit(1);
  }
  
  move_up(t, t->size, k, v); // on insère v en bas à droite, dans la case d'indice t->size, et on le fait remonter jusqu'à rétablir la propriété de tas

  t->size = t->size + 1; // mise à jour du nombre de noeuds du tas
 
  return;
}


// cette opération est parfois appelée "tamisage" en français
pqueue_elt_type move_down(heap *t, int i, pqueue_key_type k, pqueue_elt_type v, pqueue_key_type *addr_key)
{

  //printf("Tamisage de %d\n", v);

  pqueue_key_type sav_ki = t->tab[i];
  pqueue_elt_type sav_vi = t->val[i];
  
  int j = 2*i+1;
  
  while (j < t->size)
  {
    if (j+1 < t->size && ( (t->reversed_order == true && t->tab[j+1] < t->tab[j]) || (t->reversed_order == false && t->tab[j+1] > t->tab[j])))
      j = j + 1;

    if ( (t->reversed_order == true && k < t->tab[j]) || ( t->reversed_order == false && k > t->tab[j] ) ) // i père, j fils
      break;
    
 
    t->tab[i] = t->tab[j];
    t->val[i] = t->val[j];
    i = j;
    
    j = 2*i + 1;
  }
  
  t->tab[i] = k;
  t->val[i] = v;

  *addr_key = sav_ki;
  return sav_vi;
}


pqueue_elt_type heap_remove(heap *t, int i, pqueue_key_type *addr_key)
{
  assert(i < t->size);

  // on réinsere le dernier élément à la place de l'élément d'indice i, qui est renvoyé par la fonction
  pqueue_key_type last_key = t->tab[t->size-1];
  pqueue_elt_type last_elt = t->val[t->size-1];
  t->size = t->size-1;
  return move_down(t, i, last_key, last_elt, addr_key);
}

pqueue *pqueue_create(int capacity, bool reversed_order)
{
  return heap_create(capacity, reversed_order);
}

void pqueue_free(pqueue **addr_q)
{
  return heap_free(addr_q);
}

void pqueue_enqueue(pqueue *q, pqueue_key_type k, pqueue_elt_type v)
{
  heap_insert(q, k, v);
  /*for (int i = 0; i < q->size; i++)
    printf("(%lf, %d) ", q->tab[i], q->val[i]);
    printf("\n");*/
  return;
}


pqueue_elt_type pqueue_dequeue(pqueue *q, pqueue_key_type *addr_key)
{
  return heap_remove(q, 0, addr_key);
}

int pqueue_size(pqueue *q)
{
  return q->size;
}

bool pqueue_is_empty(pqueue *q)
{
  return (q->size == 0);
}

void pqueue_get_arrays(pqueue *q, pqueue_key_type **addr_tab_key, pqueue_elt_type **addr_tab_val, int *qsize)
{
  *addr_tab_key = q->tab;
  *addr_tab_val = q->val;
  *qsize = q->size;
}








