#ifndef __MYLIST__
#define __MYLIST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mylist.h"
#include <string.h>

list *list_create()
{
  list *l = NULL;
  l = malloc(sizeof(list));
  assert(l != NULL);
  l->siz = 0;
  l->addr_first = NULL;
  return l;
}

int list_length(list *l)
{
  assert(l != NULL);
  return l->siz;
}

bool list_is_empty(list *l)
{
  assert(l != NULL);
  return (l->addr_first == NULL);
}

//********************************


void list_add(list *l, key_type k, elt_type v)
{
  assert(l != NULL);
  cell *c = l->addr_first;

  cell *c_new = malloc(sizeof(cell));
  c_new->key = k; //strdup(k);
  c_new->val = v;
  c_new->addr_next = c;
  l->addr_first = c_new;

  l->siz = l->siz + 1;
  return;
}

cell *list_first_cell(list *l)
{
  assert(l!= NULL);
  return l->addr_first;
}
  
cell *list_next_cell(cell *c)
{
  assert(c!= NULL);
  return c->addr_next;
}

void list_print(list *l)
{
  if (l == NULL)
  {
      printf("Liste vide\n");
      return;
  }
  
  cell *c = l->addr_first;

  printf("%d element(s): ", l->siz);
  while (c != NULL)
  {
    printf("(");
    printf(key_fmt, c->key);
    printf(",");
    printf(elt_fmt, c->val);
    printf(") ");
    
    c = c->addr_next;
  }
  printf("\n");

  return;
}

bool list_find(list *l, key_type k, elt_type *v)
{
  assert(l != NULL);
  
  cell *c = l->addr_first;
  bool found = false;

  for (unsigned int j = 0; j < l->siz; j++)
  {
    assert(c != NULL);
    if (c->key == k) //(strcmp(c->key, k) ==  0)
    {
      found = true;
      *v = c->val;
      break;
    }
    
    c = c->addr_next;
  }

  return found;
}

bool list_delete(list *l, key_type k)
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
    
    
    if (c->key == k) //(strcmp(c->key, k) == 0)
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
    //free(c->key);
    free(c);
    return true;
  }


  cell *nxt = c->addr_next;
  prv->addr_next = nxt;

  //free(c->key);
  free(c);
  l->siz = l->siz -1;

  return true;
}



// le passage par adresse est nécessaire pour être certain
// de bien remettre l'adresse de la liste NULL
void list_free(list **addr_l)
{
  list *l = *addr_l;
  cell *c = l->addr_first;
  cell *tmp = NULL;
  
  while (c != NULL)
  {
    tmp = c;
    c = c->addr_next;
    //free(tmp->key);
    free(tmp);
  }
  free(l);
  *addr_l = NULL;

  return;
}



/*
int main(void)
{
  list *l = create_list();

  push(5, l);
  push(6, l);
  
  print_list(l);
  printf("get=%d\n", get_ith(0, l));
  print_list(l);
  push(7, l);
  push(-1, l);
  push(3, l);
  print_list(l);

  printf("Valeur %d: %d\n", 2, get_ith(2, l));
  insert_ith(3, 31, l);
  print_list(l);
  printf("get=%d\n", get_ith(0, l));
  delete_ith(0, l);
  print_list(l);
  delete_ith(2, l);
  printf("Liste l: \n");
  print_list(l);

  
  list *l2 = create_list();
  push(25, l2);
  push(36, l2);
  printf("Liste l2: \n");
  print_list(l2);

  printf("Concatenation: \n");
  print_list(concatenate(l, l2));
  printf("La liste l a été modifiée: \n");
  print_list(l);
  
  printf("get=%d\n", get_ith(0, l));
  delete_ith(0, l);
  printf("get=%d\n", get_ith(0, l));
  
  free_list(&l);
  free(l2);
  
  return 0;

  }*/

#endif
