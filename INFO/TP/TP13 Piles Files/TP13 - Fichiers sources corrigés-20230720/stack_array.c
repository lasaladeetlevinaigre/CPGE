#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define elt_type int
#define elt_fmt "%d\n"

struct s_stack
{
  int siz; // taille courante de la stacke
  int siz_max; // taille maximale possible
  elt_type *tab; // tableau contenant les valeurs (séquentielles) de la stacke
};

typedef struct s_stack stack;

stack *stack_create(int s_max)
{
  stack *s = malloc(sizeof(stack));
  s->siz = 0;
  s->siz_max = s_max;
  s->tab = malloc(s_max*sizeof(elt_type));

  return s;
}

bool stack_is_empty(stack *s)
{
  assert(s != NULL);
  return (s->siz == 0);
}

int stack_length(stack *s)
{
  assert(s != NULL);
  return s->siz;
}

void stack_push(elt_type v, stack *s)
{
  assert(s != NULL);
  assert(s->siz < s->siz_max-1);
  
  s->tab[s->siz] = v;
  s->siz = s->siz+1;

  return;
}

elt_type stack_top(stack *s)
{
  assert(s != NULL);
  assert(s->siz > 0);
  return s->tab[s->siz-1];
}

elt_type stack_pop(stack *s)
{
  assert(s != NULL);
  assert(s->siz > 0);
  elt_type v = s->tab[s->siz-1];
  s->siz = s->siz-1;

  return v;
}

void stack_print(stack *s)
{
  assert(s != NULL);
  printf("Hauteur de la pile %d\n", stack_length(s));
  printf("-------\n");
  for (int i = s->siz-1; i >= 0; i--)
    printf(elt_fmt, s->tab[i]);
  printf("-------\n");
  printf("\n");
}

void stack_delete(stack *s)
{
  assert(s != NULL);
  s->siz = 0;
}

void stack_free(stack **addr_s)
{
  assert(addr_s != NULL);
  stack *s = *addr_s;
  if (s->tab != NULL)
    free(s->tab);
  free(s);
  *addr_s = NULL;
}


int main(void)
{
  stack *s = stack_create(100);

  stack_push(2, s);
  stack_push(6, s);
  stack_push(-8, s);
  stack_print(s);
  printf("top=%d\n", stack_top(s)); fflush(stdout);
  stack_print(s);
  stack_pop(s);
  stack_print(s);
  stack_delete(s);
  stack_print(s);
  stack_push(-3, s);
  stack_push(7, s);
  stack_print(s);
  stack_free(&s);
  
  return 0;
}
