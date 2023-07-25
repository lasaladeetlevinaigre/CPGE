#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define elt_type int
#define elt_fmt  "%d "


struct node
{
  elt_type val;
  struct node *left;
  struct node *right;
};

typedef struct node node;
typedef struct node bintree;

bintree *bintree_create(node *l, elt_type val, node *r)
{

  node *nod = malloc(sizeof(node));
  assert(nod != NULL);

  nod->val = val;
  nod->left = l;
  nod->right = r;

  return nod;
}
void bintree_free(bintree **addr_t)
{
  assert(addr_t != NULL);
  bintree *t = *addr_t;
  if (t == NULL)
    return;
  bintree_free(&(t->left));
  bintree_free(&(t->right));
  free(t);
  *addr_t = NULL;

  return;
}

int bintree_number_of_nodes(bintree *t)
{
  if (t == NULL)
    return 0;

  return 1 + bintree_number_of_nodes(t->left) + bintree_number_of_nodes(t->right);				      
}

elt_type max(elt_type x, elt_type y)
{
  if (x > y)
    return x;
  else
    return y;
}

int bintree_height(bintree *t)
{
  if (t == NULL)
    return -1;

  return 1 + max(bintree_height(t->left), bintree_height(t->right));				      
}

int bintree_leaves(bintree *t)
{
  if (t == NULL)
    return 0;

  if (t->left == NULL && t->right == NULL)
    return 1;
  
  return bintree_leaves(t->left) + bintree_leaves(t->right);				      
}

void bintree_print_preorder(bintree *t)
{
  if (t == NULL)
    return;
  
  printf(elt_fmt, t->val);
  bintree_print_preorder(t->left);
  bintree_print_preorder(t->right);

  return;
				      
}

void bintree_print_inorder(bintree *t)
{
  if (t == NULL)
    return;
  
  bintree_print_inorder(t->left);
  printf(elt_fmt, t->val);
  bintree_print_inorder(t->right);

  return;
				      
}

void bintree_print_postorder(bintree *t)
{
  if (t == NULL)
    return;
  
  bintree_print_postorder(t->left);  
  bintree_print_postorder(t->right);
  printf(elt_fmt, t->val);
  return;
				      
}



int main(int argc, char **argv)
{
  // test 1: ex1 q5
  node *n1 = bintree_create(NULL, 2, NULL);
  node *n2 = bintree_create(NULL, 5, NULL);
  node *n3 = bintree_create(NULL, 8, NULL);
  node *n4 = bintree_create(n1, 3, n2);
  node *n5 = bintree_create(NULL, 7, n3); 
  node *n6 = bintree_create(n4, 5, n5);

  bintree *t = n6;

  printf("Test 1:\n");
  printf("Prefixe : "); bintree_print_preorder(t); printf("\n");
  printf("Infixe  : "); bintree_print_inorder(t); printf("\n");
  printf("Postfixe: "); bintree_print_postorder(t); printf("\n");

  bintree_free(&t);


  // test 2: ex1 q5

  // 
  node *n7 = bintree_create(NULL, 7, NULL);
  n6 = bintree_create(NULL, 6, n7);
  node *n10 = bintree_create(n6, 10, NULL);
  node *n12 = bintree_create(n10, 12, NULL);
  n3 = bintree_create(NULL, 3, NULL);
  n5 = bintree_create(n3, 5, n12);
  
  node *n18 = bintree_create(NULL, 18, NULL);
  node *n23 = bintree_create(NULL, 23, NULL);
  node *n20 = bintree_create(n18, 20, n23);
  node *n16 = bintree_create(NULL, 16, n20);
  
  t = bintree_create(n5, 15, n16);
  
  printf("\n Test 2:\n");
  printf("Prefixe : "); bintree_print_preorder(t); printf("\n");
  printf("Infixe  : "); bintree_print_inorder(t); printf("\n");
  printf("Postfixe: "); bintree_print_postorder(t); printf("\n");
  
  bintree_free(&t);
  return 0;
}
