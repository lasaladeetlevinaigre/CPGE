#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h> // pour le type uint64_t
#include "myhashtbl-lst.h"

#define B 31

#define hash_type int64_t

struct hashtbl
{
  uint64_t n_buckets;
  myhashtbl_list **tab;
};

typedef struct hashtbl hashtbl;

hashtbl *hashtbl_create(uint64_t n_buckets)
{
  hashtbl *ht = malloc(sizeof(hashtbl));
  assert(ht != NULL); // check allocation
  
  ht->n_buckets = n_buckets;
  ht->tab = malloc(n_buckets*sizeof(myhashtbl_list *));
  for (int i = 0; i < ht->n_buckets; i++)
    ht->tab[i] = NULL;
  
  return ht;
}

hash_type hash_function_pb(int b, int64_t n_buckets, char *k)
{
  assert( k!= NULL);
  
  int64_t sum = 0;
  int n = strlen(k);
  sum = 0;
  for (int i = 0; i < n; i++)
    sum = k[i] + sum*b;

  
  return sum % n_buckets; //(sum & INT_MAX)%n_buckets;
}

hash_type hash_function(int b, int64_t n_buckets, char *k)
{
  assert( k!= NULL);
  
  int64_t sum = 0;
  int n = strlen(k);
  sum = 0;
  for (int i = 0; i < n; i++)
    sum = k[i] + sum*b;

  
  return (sum & INT_MAX)%n_buckets;
}


void hashtbl_add(hashtbl *ht, char * k, myhashtbl_elt_type v)
{
  assert(ht != NULL);
  assert(k != NULL);
  hash_type idx  = hash_function(B, ht->n_buckets, k);
 
  myhashtbl_list *l = ht->tab[idx];
  
  if (l == NULL)
  {
    ht->tab[idx] = myhashtbl_list_create();
    l = ht->tab[idx];
  }
  
  myhashtbl_list_add(l, k, v);
}

bool hashtbl_find(hashtbl *ht, char * k, myhashtbl_elt_type *addr_v)
{
  assert(ht != NULL);
  assert(k != NULL);
  assert(addr_v != NULL);
  
  hash_type idx  = hash_function(B, ht->n_buckets, k);
  myhashtbl_list *l = ht->tab[idx];
  return myhashtbl_list_find(l, k, addr_v);
}

bool hashtbl_replace(hashtbl *ht, char * k, myhashtbl_elt_type v)
{
  assert(ht != NULL);
  assert(k != NULL);
  
  hash_type idx  = hash_function(B, ht->n_buckets, k);
  myhashtbl_list *l = ht->tab[idx];
  
  return myhashtbl_list_replace(l, k, v);
}

bool hashtbl_remove(hashtbl *ht, char * k)
{
  assert(ht != NULL);
  assert(k != NULL);
  
  hash_type idx  = hash_function(B, ht->n_buckets, k);
  myhashtbl_list *l = ht->tab[idx];
  
  return myhashtbl_list_remove(l, k);
}


void hashtbl_print(hashtbl *h)
{
  assert(h != NULL);
  for (int i = 0; i < h->n_buckets; i++)
  {
    printf("Bucket %d: ", i);
    myhashtbl_list_print(h->tab[i]);
  }
  printf("\n");
  return;
}

void hashtbl_free(hashtbl **addr_ht)
{
  assert(addr_ht != NULL);
  
  hashtbl *ht = *addr_ht;

  if (ht == NULL)
    return;
  
  myhashtbl_list *l = NULL;
  
  for (int i = 0; i < ht->n_buckets; i++)
  {
    l = ht->tab[i];
    if (l != NULL)
      myhashtbl_list_free(&l);
  }
  free(ht->tab);
  free(ht);

  *addr_ht = NULL;
  
  return;
}


uint64_t hashtbl_number_of_bindings(hashtbl *ht)
{
  uint64_t sum = 0;
  int max_bucket_size = 0;
  int bucket_size;
  myhashtbl_list *l = NULL;
  
  for (uint64_t i = 0; i < ht->n_buckets; i++)
  {
    l = ht->tab[i];
    if (l != NULL)
    {
      bucket_size = myhashtbl_list_length(l);
      if (bucket_size > max_bucket_size)
	max_bucket_size = bucket_size;
      sum = sum + bucket_size;
    }
  }

  return sum;
}

uint64_t hashtbl_stats(hashtbl *ht)
{
  uint64_t sum = 0;
  int max_bucket_size = 0;
  int bucket_size;
  myhashtbl_list *l = NULL;
  
  for (uint64_t i = 0; i < ht->n_buckets; i++)
  {
    l = ht->tab[i];
    if (l != NULL)
    {
      bucket_size = myhashtbl_list_length(l);
      if (bucket_size > max_bucket_size)
	max_bucket_size = bucket_size;
      sum = sum + bucket_size;
    }
  }

  if (sum == 0)
    return 0;

  
  int *histo = calloc((max_bucket_size+1), sizeof(int));

  for (uint64_t i = 0; i < ht->n_buckets; i++)
  {
    l = ht->tab[i];
    if (l != NULL)
      bucket_size = myhashtbl_list_length(l);
    else
      bucket_size = 0;

    histo[bucket_size] = histo[bucket_size] + 1;
    
  }

  printf("Nombre de données: %ld\n", sum);
  printf("Taille maximale de seau: %ld\n", sum);
  printf("Facteur d'équilibrage: %ld\n", sum/ht->n_buckets);
  printf("Diagramme en bâtons:\n");
  for (int i = 0; i <= max_bucket_size; i++)
    printf("\tTaille %d: %d seaux\n", i, histo[i]);
  


  //printf("clé n': %d\n", hash_function("n'", h->n_buckets));
  //printf("clé le: %d\n", hash_function("le", h->n_buckets));

  free(histo);
  return sum;
}



