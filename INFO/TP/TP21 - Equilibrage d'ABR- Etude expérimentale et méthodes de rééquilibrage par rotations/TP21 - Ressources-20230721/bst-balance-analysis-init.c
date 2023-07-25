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




// on a recodé cette fonction sous orme itérative pour la rendre plus rapide et éviter l'empilement des blocs d'activation
int bst_height(bst *t)
{
  if (t == NULL)
    return -1;

  return 1 + MAX(bst_height(t->left), bst_height(t->right));				      
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
    printf("parent=NULL) ");
  
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


  node *n = t;
  node *new_node = NULL;
  while (n != NULL)
  {
    if (k == n->val) // la valeur est déjà présente, on n'a rien à insérer
      return t;
    
    else if (k < n->val)
    {
      if (n->left == NULL)
      {
	new_node = bst_create(NULL, k, NULL); // on cree le nouveau noeud isole
	n->left = new_node; // on raccorde le parent:  left --> nouveau noeud
	new_node->parent = n; // on dit que le parent du nouveau noeud est bien n
	return t; // on sort directement
      }
      n = n->left;
    }
    else
    {
      if (n->right == NULL)
      {
	new_node = bst_create(NULL, k, NULL);
	n->right = new_node;
	new_node->parent = n;
	return t;
      }
      n = n->right;
    }
  }

  return NULL;
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




int main(int argc, char **argv)
{
  return 0;
}
