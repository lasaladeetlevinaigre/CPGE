#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

// ces deux lignes sont des macros: pour des fonctions très simples, comme MIN et MAX de deux entiers
#define MAX(x,y) ( ( (x) > (y) )  ? (x):(y) )
#define MIN(x,y) ( ( (x) < (y) )  ? (x):(y) )


#define elt_type int
#define elt_fmt  "%d"


// on a choisi de garder les fonctions de manipulation dans leur version itérative, pour limiter l'empilement de blocs d'activation étant donné qu'on va gérer des arbres ayant beaucoup de noeuds.



struct node
{
  elt_type val;
  struct node *left;
  struct node *right;
  struct node *parent;
};

typedef struct node node;
typedef struct node bst;

bst *bst_create(bst *l, elt_type k, bst *r)
{

  node *nod = malloc(sizeof(node));
  assert(nod != NULL);

  nod->parent = NULL;
  nod->val = k;
  nod->left = l;
  
  if (l != NULL)
    l->parent = nod;
  
  
  nod->right = r;

  if (r != NULL)
    r->parent = nod;
   
  return nod;
}

void bst_free(bst **addr_t)
{
  assert(addr_t != NULL);
  bst *t = *addr_t;
  if (t == NULL)
    return;
  bst_free(&(t->left));
  bst_free(&(t->right));
  free(t);
  *addr_t = NULL;

  return;
}




void bst_print_preorder(bst *t)
{
  if (t == NULL)
    return;

  printf("("); printf(elt_fmt, t->val); printf(";");
  if (t->parent != NULL)
  {
    printf("parent=");printf(elt_fmt, t->parent->val);printf(") ");
  }
  else
    printf(" parent=NULL) ");
  bst_print_preorder(t->left);
  bst_print_preorder(t->right);

  return;
				      
}

void bst_print_inorder(bst *t)
{
  if (t == NULL)
    return;
  
  bst_print_inorder(t->left);
  printf("("); printf(elt_fmt, t->val); printf(";");
  if (t->parent != NULL)
  {
    printf("parent=");printf(elt_fmt, t->parent->val);printf(") ");
  }
  else
    printf("parent=NULL) ");
  bst_print_inorder(t->right);

  return;
				      
}

void bst_print_postorder(bst *t)
{
  if (t == NULL)
    return;
  
  bst_print_postorder(t->left);  
  bst_print_postorder(t->right);
  printf("("); printf(elt_fmt, t->val); printf(";");
  if (t->parent != NULL)
  {
    printf("parent=");printf(elt_fmt, t->parent->val);printf(") ");
  }
  else
    printf("parent=NULL) ");
  return;
				      
}


bst *bst_insert(elt_type k, bst *t)
{
 
  if (t == NULL)
    return bst_create(NULL, k, NULL);
  
  node * n = NULL;

  if (k < t->val)
  {
    n = bst_insert(k, t->left);
    t->left = n;
    n->parent = t;
  }
  else if (k > t->val)
  {
    n = bst_insert(k, t->right);
    t->right = n;
    n->parent = t;
  }

  // cas d'egalite: on ne fait rien

  return t;
}





node *bst_min(bst *t)
{

  if (t == NULL) // cas d'erreur
  {
    fprintf(stderr, "Impossible de renvoyer le min: l'arbre est vide!\n");
    return NULL; 
  }

  
  node *n = t;
  while (n->left != NULL)
    n = n->left;
  

  return n;
}


bool bst_find(elt_type k, bst *t, node **addr_node)
{
  *addr_node = NULL;
  
  node *n = t; 
  while (n != NULL)
  {
    if (k == n->val)
    {
      *addr_node = n;
      return true;
    }
    
    if (k < n-> val)
      n = n->left;
    else 
      n = n->right;
  }
  
  return false;
}

void rotate_right(bst *n)
{

  node *u = n; // noeud autour duquel s'effectue la rotation
  if (n == NULL)
    return;
  
  node *v = n->left;
  if (v == NULL)
    return;

  node *t1 = v->left;
  node *t2 = v->right;
  node *t3 = u->right;
 
  elt_type tmp = u->val;
  u->val = v->val;
  v->val = tmp;

  u->left = t1;
  u->right = v;
  v->left = t2;
  v->right = t3;

  if (t1 != NULL)
    t1->parent = u;
  
  if (t3 != NULL)
    t3->parent = v;

  
  return;
}

void rotate_left(bst *n)
{
  node *u = n; // noeud autour duquel s'effectue la rotation
  if (n == NULL)
    return;
  
  node *v = n->right;
  if (v == NULL)
    return;
  
  node *t1 = u->left;
  node *t2 = v->left;
  node *t3 = v->right;
 
  elt_type tmp = u->val;
  u->val = v->val;
  v->val = tmp;

  u->left = v;
  u->right = t3;
  v->left = t1;
  v->right = t2;

  if (t1 != NULL)
    t1->parent = v;
  if (t3 != NULL)
    t3->parent = u;

  
  return;
}


void test_rot_reciproc(void)
{
  bst *t = bst_insert(4, NULL);
  t = bst_insert(5, t);
  t = bst_insert(2, t);
  t = bst_insert(3, t);
  t = bst_insert(1, t);

  printf("\n***** TEST RECIPROCITE ***********\n");
  printf("DEBUT:\n");bst_print_inorder(t); printf("\n\n");
  rotate_right(t);

  printf("ROTATION DROITE:\n");bst_print_inorder(t); printf("\n\n");
  
  rotate_left(t); // on teste si rot left et right sont bien réciproques l'un de l'autre
  
  printf("ROTATION GAUCHE:\n");bst_print_inorder(t); printf("\n\n");
  printf("\n*******************************\n");
  bst_free(&t);
  return;
}


// test de l'enonce
void test1(void)
{
  bst *t = bst_insert(14, NULL);
  t = bst_insert(17, t);
  t = bst_insert(20, t);
  t = bst_insert(18, t);
  t = bst_insert(23, t);
  t = bst_insert(5, t);
  t = bst_insert(12, t);
  t = bst_insert(13, t);
  t = bst_insert(10, t);
  t = bst_insert(6, t);
  t = bst_insert(7, t);
  t = bst_insert(3, t);
  t = bst_insert(1, t);
  t = bst_insert(-2, t);

  printf("\n***** TEST 1 ***********\n");
  printf("DEBUT:\n");bst_print_inorder(t); printf("\n\n");
  node *n = NULL;

  // rotation droite autour de 10
  if (bst_find(10, t, &n) == true)
  {
    rotate_right(n);
    printf("ROTATION DROITE AUTOUR DE 10:\n");bst_print_inorder(t); printf("\n\n");
  }

  // rotation gauche autour de 17
  if (bst_find(17, t, &n) == true)
  {
    rotate_left(n);
    printf("ROTATION GAUCHE AUTOUR DE 17:\n");bst_print_inorder(t); printf("\n\n");
  }

  // rotation droite autour de 14
  if (bst_find(14, t, &n) == true)
  {
    rotate_right(n);
    printf("ROTATION DROITE AUTOUR DE 14:\n");bst_print_inorder(t); printf("\n\n");
  }
  printf("\n*************************\n");
  // on n'oublie pas de libérer l'espace alloué dans le tas
  bst_free(&t);
  return;
}


int main(void)
{
  test_rot_reciproc();
  test1();
  
  return 0;
}

