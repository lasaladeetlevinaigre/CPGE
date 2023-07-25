#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct s_cell
{
  double         val; // valeur contenue dans ce maillon
  struct s_cell *addr_next; // pointeur contenant l'adresse
                            // du maillon suivant
};

typedef struct s_cell cell;

// *******************************//
// amelioration: creation d'une structure de donnée list
// qui encapsule les 2 informations: l'adresse du premier
// maillon + la taille de la liste
// la taille est ainis mise à jour des que la structure
// est modifiée (suppression ou ajour d'un element)
// ce qui evite de recalculer trop souvent la taille
struct s_list
{
  int  siz;
  cell         *addr_first;
};

typedef struct s_list list;

list *create_list()
{
  list *l = NULL;
  l = malloc(sizeof(list));
  assert(l != NULL);
  l->siz = 0;
  l->addr_first = NULL;
  return l;
}

int get_length(list *l)
{
  assert(l != NULL);
  return l->siz;
}

//********************************


void push(double v, list *l)
{
  assert(l != NULL);
  cell *c = l->addr_first;

  cell *c_new = malloc(sizeof(cell));
  c_new->val = v;
  c_new->addr_next = c;
  l->addr_first = c_new;

  l->siz = l->siz + 1;
  return;
}


void print_list(list *l)
{
  cell *c = l->addr_first;

  printf("%d elements: ", l->siz);
  while (c != NULL)
  {
    printf("%lf ", c->val);
    c = c->addr_next;
  }
  printf("\n");
  return;
}

double get_ith(unsigned int i, list *l)
{
  assert(l != NULL);
  cell *c = l->addr_first;
  for (unsigned int j = 0; j < i; j++)
  {
    assert(c != NULL);
    c = c->addr_next;
  }
  assert(c != NULL);

  return c->val;
}

void delete_ith(unsigned int i, list *l)
{
  assert(l != NULL);
  assert(i < l->siz);
  
  cell *c = NULL;
  
  if (i == 0)
  {
    c = l->addr_first;
    assert(c != NULL);
    l->addr_first = c->addr_next;
    l->siz = l->siz - 1;
    free(c);
    return;
  }

  
  c   = l->addr_first;
  cell *prv = NULL;
  cell *nxt = NULL;

 
  for (unsigned int j = 0; j < i; j++)
  {
    assert(c != NULL);
    prv = c;
    c = c->addr_next;
  }

  assert(c != NULL);  
  nxt = c->addr_next;
  prv->addr_next = nxt;

  free(c);
  l->siz = l->siz -1;

  return;
}

// calcul de la longueur courante de la liste
int length(list *l)
{
  assert(l != NULL);
  cell *c = l->addr_first;
  int cpt = 0;
  
  while (c != NULL)
  {
    cpt = cpt + 1;
    c = c->addr_next;
  }

  l->siz = cpt;
  return cpt;
}

void insert_ith(unsigned int i, double v, list *l)
{
  assert(l != NULL);
  assert(i <= l->siz);
  
  cell *c_new = NULL;
  
  if (i == 0)
  {
    c_new = malloc(sizeof(cell));
    c_new->val = v;
    c_new->addr_next = l->addr_first;
    l->addr_first = c_new;
    l->siz = l->siz + 1;
    return;
  }

  cell *c = l->addr_first;
  cell *prv = NULL;
  for (unsigned int j = 0; j < i; j++)
  {
    assert(c != NULL);
    prv = c;
    c = c->addr_next;
  }

  c_new = malloc(sizeof(cell));
  c_new->val = v;
  c_new->addr_next = c;
  prv->addr_next = c_new;
  l->siz = l->siz + 1;
  return;
}

// le passage par adresse est nécessaire pour être certain
// de bien remettre l'adresse de la liste NULL
void free_list(list **addr_l)
{
  list *l = *addr_l;
  cell *c = l->addr_first;
  cell *tmp = NULL;
  
  while (c != NULL)
  {
    tmp = c;
    c = c->addr_next;
    free(tmp);
  }
  free(l);
  *addr_l = NULL;

  return;
}


// attention: cette fonction a un effet de bord: l1 est modifiée
// en sortie de la fonction!!!
list *concatenate(list *l1, list *l2)
{
  if (l2 == NULL)
    return l1;
  if (l1 == NULL)
    return l2;
  
  cell *c = l1->addr_first;
  if (c == NULL)
    return l2;

  cell *prv = NULL;
  
  while (c != NULL)
  {
    prv = c;
    c = c->addr_next;
  }
  prv->addr_next = l2->addr_first;

  l1->siz = l1->siz + l2->siz;
  return l1;
}

int main(void)
{
  list *l1 = create_list();

  push(1.5, l1);
  push(3.1, l1);
  
  print_list(l1);
  printf("get=%lf\n", get_ith(0, l1));
  print_list(l1);
  push(2.25, l1);
  push(1.33, l1);
  push(2.17, l1);
  print_list(l1);

  printf("Valeur %d: %lf\n", 2, get_ith(2, l1));
  insert_ith(3, 31.47, l1);
  print_list(l1);
  printf("get=%lf\n", get_ith(0, l1));
  delete_ith(0, l1);
  print_list(l1);
  delete_ith(2, l1);
  printf("Liste l1: \n");
  print_list(l1);

  
  list *l2 = create_list();
  push(25.1, l2);
  push(36, l2);
  printf("Liste l2: \n");
  print_list(l2);

  printf("Concatenation: \n");
  print_list(concatenate(l1, l2));
  printf("La liste l1 a été modifiée: \n");
  print_list(l1);
  
  printf("get=%lf\n", get_ith(0, l1));
  delete_ith(0, l1);
  printf("get=%lf\n", get_ith(0, l1));
  
  free_list(&l1);
  free(l2);
  
  return 0;

}
