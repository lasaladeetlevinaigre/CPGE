#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

enum TypeElt {Int, Float, Double, Char, Bool};

struct s_cell
{
  void          *addr_elt; // pointeur vers l'élément de type void (non def)
  struct s_cell *addr_next; // pointeur contenant l'adresse
                            // du maillon suivant
};

typedef struct s_cell cell;

struct s_list
{
  unsigned int  siz;
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

unsigned int get_length(list *l)
{
  assert(l != NULL);
  return l->siz;
}


/************************************/

void push(void *v, list *l)
{
  assert(l != NULL);
  cell *c = l->addr_first;

  cell *c_new = malloc(sizeof(cell));
  c_new->addr_elt = v;
  c_new->addr_next = c;
  l->addr_first = c_new;

  l->siz = l->siz + 1;
  return;
}

void print_list(list *l, char fmt)
{
  cell *c = l->addr_first;
  bool stop = false;
  
    
  int    *pi;
  float  *pf;
  double *plf;
  char   *pc;
  bool   *pb;
  
  printf("%d elements: ", l->siz);
  while (c != NULL && stop == false)
  {
    if (c->addr_elt != NULL)
    {
      switch(fmt)
      {	  
	case Int:
	  pi = (int *)c->addr_elt;
	  printf("%d", *pi );
	  break;
        case Float:
	  pf = (float *)c->addr_elt;
	  printf("%f", *pf);
	  break;
	case Double:
	  plf = (double *)c->addr_elt;
	  printf("%lf", *plf);
	  break;
        case Char:
	  pc = (char *)c->addr_elt;
	  printf("%c", *pc);
	  break;
	  
       	case Bool:
	  pb = (bool *)c->addr_elt;
	  printf("%d", *pb );
	  break;
      default:
          printf("Liste d'éléments de type inconnu, impossible d'afficher");
	  stop = true;
	  break;
      }
      
	
    }
    else
      printf("NULL");
    
    printf(" ");
    c = c->addr_next;
  }
  printf("\n");

  return;
}




void *get_ith(unsigned int i, list *l)
{
  assert(l != NULL);
  cell *c = l->addr_first;
  for (unsigned int j = 0; j < i; j++)
  {
    assert(c != NULL);
    c = c->addr_next;
  }
  assert(c != NULL);

  return c->addr_elt;
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
  
}

// calcul de la longueur courante de la liste
unsigned int length(list *l)
{
  assert(l != NULL);
  cell *c = l->addr_first;
  unsigned int cpt = 0;
  
  while (c != NULL)
  {
    cpt = cpt + 1;
    c = c->addr_next;
  }

  l->siz = cpt;
  return cpt;
}

void insert_ith(unsigned int i, void *addr_v, list *l)
{
  assert(l != NULL);
  assert(i <= l->siz);
  
  cell *c_new = NULL;
  
  if (i == 0)
  {
    c_new = malloc(sizeof(cell));
    c_new->addr_elt = addr_v;
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
  c_new->addr_elt = addr_v;
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
  list *l = create_list();


  int t[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  insert_ith(0, &(t[4]), l);
  insert_ith(0, &(t[6]), l);
  
  print_list(l, Int);
  int *v = get_ith(0, l);
  printf("get=%d\n", *v);
  print_list(l, Int);
  insert_ith(0, &(t[7]), l);
  insert_ith(0, &(t[9]), l);
  insert_ith(0, &(t[0]), l);

  
  
  print_list(l, Int);
  v = get_ith(2, l);
  printf("Valeur %d: %d\n", 2, *v);
  int a = 31;
  insert_ith(0, &a, l);
  print_list(l, Int);
  v  = get_ith(0, l);
  printf("get=%d\n", *v );
  delete_ith(0, l);
  print_list(l, Int);
  delete_ith(2, l);

  

  list *l2 = create_list();
  push(&(t[7]), l2);
  push(&(t[3]), l2);
  printf("Liste l:\n");
  print_list(l, Int);
  printf("Liste l2:\n");
  print_list(l2, Int);
  printf("Concatenation:\n");
  print_list(concatenate(l, l2), Int);
  // liste de flottants simple précision
  list *l3 = create_list();
  float tf[3] = {1.5, -3, 2.1};
  
  insert_ith(0, &(tf[0]), l3);
  insert_ith(0, &(tf[2]), l3);
  
  print_list(l3, Float);
  float *vf  = get_ith(0, l3);
  printf("get=%f\n", *vf);
  delete_ith(0, l3);
  vf  = get_ith(0, l3);
  printf("get=%f\n", *vf);
  
  free_list(&l3);  
  free_list(&l);
  free(l2);
  return 0;

}
