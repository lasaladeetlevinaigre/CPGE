#ifndef __MYLIST__
#define __MYLIST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "myhashtbl-lst.h"

// implémentation d'une structure de données de type liste
// sous forme de maillons (cell) chaînés
struct s_cell
{
  char               *key;
  myhashtbl_elt_type  val; // valeur contenue dans ce maillon
  struct s_cell *addr_next; // pointeur contenant l'adresse
                            // du maillon suivant
};

typedef struct s_cell cell;

// structure encapsulante, avec stockage de la taille courante
struct s_myhashtbl_list
{
  int  siz;
  cell *addr_first;
};

typedef struct s_myhashtbl_list myhashtbl_list;

myhashtbl_list *myhashtbl_list_create()
{
  myhashtbl_list *l = NULL;
  l = malloc(sizeof(myhashtbl_list));
  assert(l != NULL);
  l->siz = 0;
  l->addr_first = NULL;
  return l;
}

int myhashtbl_list_length(myhashtbl_list *l)
{
  assert(l != NULL);
  return l->siz;
}

//********************************


void myhashtbl_list_add(myhashtbl_list *l, char * k, myhashtbl_elt_type v)
{
  assert(l != NULL);
  cell *c = l->addr_first;

  cell *c_new = malloc(sizeof(cell));
  c_new->key = strdup(k);
  c_new->val = v;
  c_new->addr_next = c;
  l->addr_first = c_new;

  l->siz = l->siz + 1;
  return;
}


void myhashtbl_list_print(myhashtbl_list *l)
{
  if (l == NULL)
  {
      printf("Myhashtbl_Liste vide\n");
      return;
  }
  
  cell *c = l->addr_first;

  printf("%d element(s): ", l->siz);
  while (c != NULL)
  {
    printf("(");
    printf("%s ", c->key);
    printf(",");
    printf(myhashtbl_elt_fmt, c->val);
    printf(") ");
    
    c = c->addr_next;
  }
  printf("\n");

  return;
}

bool myhashtbl_list_find(myhashtbl_list *l, char *k, myhashtbl_elt_type *addr_v)
{
  assert(addr_v != NULL);

  if (l == NULL)
    return false;
  
  cell *c = l->addr_first;
  bool found = false;

  for (unsigned int j = 0; j < l->siz; j++)
  {
    assert(c != NULL);
    if (strcmp(c->key, k) ==  0)
    {
      found = true;
      *addr_v = c->val;
      break;
    }
    
    c = c->addr_next;
  }

  return found;
}

bool myhashtbl_list_replace(myhashtbl_list *l, char *k, myhashtbl_elt_type v)
{
  assert(l != NULL);
  
  cell *c = l->addr_first;
  bool found = false;

  for (unsigned int j = 0; j < l->siz; j++)
  {
    assert(c != NULL);
    if (strcmp(c->key, k) ==  0)
    {
      found = true;
      c->val = v; // remplacement de l'ancienne valeur pour cette clé
      break;
    }
    
    c = c->addr_next;
  }

  if (found == false)
  {
    myhashtbl_list_add(l, k, v);
  }
  return found;
}

bool myhashtbl_list_remove(myhashtbl_list *l, char *k)
{
  if (l == NULL)
    return false;

  cell *c = l->addr_first;
  bool found = false;

  cell *prv = NULL;
  unsigned int i;
  
  for (i = 0; i < l->siz; i++)
  {
    assert(c != NULL);
    
    
    if (strcmp(c->key, k) == 0)
    {
      found = true;
      break;
    }
    prv = c;
    c = c->addr_next;
  }
  
  if (found == false)
    return false;
  
  if (prv == NULL)
  {
    c = l->addr_first;
    assert(c != NULL);
    l->addr_first = c->addr_next;
    l->siz = l->siz - 1;
    free(c->key);
    free(c);
    return true;
  }


  cell *nxt = c->addr_next;
  prv->addr_next = nxt;

  free(c->key);
  free(c);
  l->siz = l->siz -1;

  return true;
}



// le passage par adresse est nécessaire pour être certain
// de bien remettre l'adresse de la myhashtbl_liste NULL
void myhashtbl_list_free(myhashtbl_list **addr_l)
{
  myhashtbl_list *l = *addr_l;
  cell *c = l->addr_first;
  cell *tmp = NULL;
  
  while (c != NULL)
  {
    tmp = c;
    c = c->addr_next;
    free(tmp->key);
    free(tmp);
  }
  free(l);
  *addr_l = NULL;

  return;
}



/*
int main(void)
{
  myhashtbl_list *l = create_myhashtbl_list();

  push(5, l);
  push(6, l);
  
  print_myhashtbl_list(l);
  printf("get=%d\n", get_ith(0, l));
  print_myhashtbl_list(l);
  push(7, l);
  push(-1, l);
  push(3, l);
  print_myhashtbl_list(l);

  printf("Valeur %d: %d\n", 2, get_ith(2, l));
  insert_ith(3, 31, l);
  print_myhashtbl_list(l);
  printf("get=%d\n", get_ith(0, l));
  delete_ith(0, l);
  print_myhashtbl_list(l);
  delete_ith(2, l);
  printf("Myhashtbl_Liste l: \n");
  print_myhashtbl_list(l);

  
  myhashtbl_list *l2 = create_myhashtbl_list();
  push(25, l2);
  push(36, l2);
  printf("Myhashtbl_Liste l2: \n");
  print_myhashtbl_list(l2);

  printf("Concatenation: \n");
  print_myhashtbl_list(concatenate(l, l2));
  printf("La myhashtbl_liste l a été modifiée: \n");
  print_myhashtbl_list(l);
  
  printf("get=%d\n", get_ith(0, l));
  delete_ith(0, l);
  printf("get=%d\n", get_ith(0, l));
  
  free_myhashtbl_list(&l);
  free(l2);
  
  return 0;

  }*/

#endif
