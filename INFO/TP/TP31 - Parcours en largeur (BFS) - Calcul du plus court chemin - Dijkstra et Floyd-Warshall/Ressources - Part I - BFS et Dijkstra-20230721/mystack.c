#ifndef __MYSTACK__
#define __MYSTACK__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mystack.h"
#include <string.h>

stack *stack_create()
{
  stack *s  = malloc(sizeof(stack));
  assert(s != NULL);
  s->siz = 0;
  s->addr_first = NULL;
  return s;
}

int stack_length(stack *s)
{
  assert(s != NULL);
  return s->siz;
}

bool stack_is_empty(stack *s)
{
  assert(s != NULL);
  return (s->addr_first == NULL);
}

vtype stack_top(stack *s)
{
  assert(s != NULL);
  stack_cell *c = s->addr_first;
  return c->key;
}
//********************************


void stack_push(stack *s, vtype k)
{
  assert(s != NULL);
  stack_cell *c = s->addr_first;

  stack_cell *c_new = malloc(sizeof(stack_cell));
  c_new->key = k; 
  c_new->addr_next = c;
  s->addr_first = c_new;

  s->siz = s->siz + 1;
  return;
}


void stack_print(stack *s)
{
  if (s == NULL)
  {
      printf("Pile non allouée\n");
      return;
  }
  
  stack_cell *c = s->addr_first;

  printf("%d element(s): ", s->siz);
  while (c != NULL)
  {
    printf(fmt, c->key);
    c = c->addr_next;
  }
  printf("\n");

  return;
}



vtype stack_pop(stack *s)
{
  assert(s != NULL);

  stack_cell *c = s->addr_first;

  c = s->addr_first;
  assert(c != NULL);
  s->addr_first = c->addr_next;
  s->siz = s->siz - 1;
  vtype v = c->key;
  free(c);

  return v;
}



// le passage par adresse est nécessaire pour être certain
// de bien remettre l'adresse de la liste NULL
void stack_free(stack **addr_s)
{
  stack *s = *addr_s;
  stack_cell *c = s->addr_first;
  stack_cell *tmp = NULL;
  
  while (c != NULL)
  {
    tmp = c;
    c = c->addr_next;
    //free(tmp->key);
    free(tmp);
  }
  free(s);
  *addr_s = NULL;

  return;
}

void stack_reverse(stack *s)
{
  stack_cell *c = s->addr_first;
  stack_cell *prvc = NULL;
  stack_cell *tmpc = NULL;

  while (c != NULL)
  {    
    tmpc = c->addr_next;
    c->addr_next = prvc;
    prvc = c;
    c = tmpc;
  }

  s->addr_first = prvc;
  
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
