#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct s_list
{
  int siz; // taille courante de la liste
  int siz_max; // taille maximale possible
  int *tab; // tableau contenant les valeurs (séquentielles) de la liste
};

typedef struct s_list list;

list *create_list(int s_max)
{
  list *l = malloc(sizeof(list));
  l->siz = 0;
  l->siz_max = s_max;
  l->tab = malloc(s_max*sizeof(int));

  return l;
}

int get_length(list *l)
{
  return l->siz;
}

void push(int v, list *l)
{
  assert(l->siz < l->siz_max);
  
  for (int j = l->siz; j > 0; j--)
    l->tab[j] = l->tab[j-1];

  l->tab[0] = v;
  l->siz = l->siz+1;

  return;
}

void print_list(list *l)
{
  printf("Longueur %d: ", get_length(l));
  for (unsigned int i = 0; i < l->siz; i++)
    printf("%d ", l->tab[i]);
  printf("\n");
}

int get_ith(int i, list *l)
{
  assert(i < l->siz);
  return l->tab[i];
}


int delete_ith(unsigned int i, list *l)
{
  assert(i < l->siz);
  
  int v = l->tab[i];
  for (unsigned int j = i; j < l->siz-1; j++)
    l->tab[j] = l->tab[j+1];
  l->siz = l->siz - 1;

  return v;
}

void insert_ith(int i, int v, list *l)
{
  assert(l->siz < l->siz_max);
  assert(i >= 0 && i <= l->siz);
  
  for (int j = l->siz; j > i; j--)
    l->tab[j] = l->tab[j-1];

  l->tab[i] = v;
  l->siz = l->siz+1;

  return;
}


void delete_list(list *l)
{
  l->siz = 0;
}

void free_list(list **addr_l)
{
  list *l = *addr_l;
  if (l->tab != NULL)
    free(l->tab);
  free(l);
  *addr_l = NULL;
}



list *concatenate(list *l1, list *l2)
{
  assert(l1->siz + l2->siz < l1->siz_max);
  
  for (unsigned int j = 0; j < l2->siz; j++)
    l1->tab[l1->siz+j] = l2->tab[j];
  
  l1->siz = l1->siz + l2->siz;
  return l1;
}

int main(void)
{
  list *l = create_list(100);

  push(2, l);
  push(6, l);
  push(-8, l);
  print_list(l);
  printf("pop=%d\n", get_ith(0, l));
  print_list(l);

  insert_ith(0, 5, l);
  insert_ith(3, 11, l);
  printf("Liste l: \n");
  print_list(l);

  list *l2 = create_list(100);
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
  free_list(&l2);
  
  return 0;
}
