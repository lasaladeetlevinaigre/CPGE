#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include "limits.h" 
#define elt_type int
#define elt_fmt  "%d "


struct node
{
  elt_type val;
  struct node *left;
  struct node *right;
};

typedef struct node node;
typedef struct node bst;

// On a choisi de conserver les fonctions récursives, mais on aurait pu laisser les fonctions iteratives et les adapter également!

bst *bst_create(bst *l, elt_type k, bst *r)
{

  node *nod = malloc(sizeof(node));
  assert(nod != NULL);


  nod->val = k;
  nod->left = l;  
  nod->right = r;
  
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

int bst_number_of_nodes(bst *t)
{
  if (t == NULL)
    return 0;

  return 1 + bst_number_of_nodes(t->left) + bst_number_of_nodes(t->right);				      
}

elt_type max(elt_type x, elt_type y)
{
  if (x > y)
    return x;
  else
    return y;
}
elt_type min(elt_type x, elt_type y)
{
  if (x < y)
    return x;
  else
    return y;
}

int bst_height(bst *t)
{
  if (t == NULL)
    return -1;

  return 1 + max(bst_height(t->left), bst_height(t->right));				      
}

int bst_leaves(bst *t)
{
  if (t == NULL)
    return 0;

  if (t->left == NULL && t->right == NULL)
    return 1;
  
  return bst_leaves(t->left) + bst_leaves(t->right);				      
}


void bst_print_preorder(bst *t)
{
  if (t == NULL)
    return;
  
  printf(elt_fmt, t->val);
  bst_print_preorder(t->left);
  bst_print_preorder(t->right);

  return;
				      
}

void bst_print_inorder(bst *t)
{
  if (t == NULL)
    return;
  
  bst_print_inorder(t->left);
  printf(elt_fmt, t->val);
  bst_print_inorder(t->right);

  return;
				      
}

void bst_print_postorder(bst *t)
{
  if (t == NULL)
    return;
  
  bst_print_postorder(t->left);  
  bst_print_postorder(t->right);
  printf(elt_fmt, t->val);
  return;
				      
}

// exercice 2: fonction de manipulations bsaiques d'un ABR

bst *bst_insert(elt_type k, bst *t)
{
 
  if (t == NULL)
    return bst_create(NULL, k, NULL);
  
  node * n = NULL;
  
  if (k < t->val)
  {
    n = bst_insert(k, t->left);
    t->left = n;
  }
  else if (k > t->val)
  {
    n = bst_insert(k, t->right);
    t->right = n;
  }

  // cas d'egalite: on ne fait rien

  return t;
}


// On adapte la fonction bst_min pour qu'elle renvoie également le noeud parent ==> utile dans la fonction de suppression
node *aux_bst_min(bst *t, bst **addr_parent)
{
  *addr_parent = NULL;
  if (t == NULL)
    return NULL;

  if (t->left == NULL)
    return t;

  *addr_parent = t;
  return aux_bst_min(t->left, addr_parent);
}

// encapsulation dans le cas où bst_min est utlisée "pour elle-même"
node *bst_min(bst *t)
{
  node *tmp;
  return aux_bst_min(t, &tmp);
}




// On adapte la fonction bst_find pour qu'elle renvoie égalemnt le parent ==> utile dans la fonction de suppression
bool aux_bst_find(elt_type k, bst *t, node **addr_parent, node **addr_node)
{ 
  if (t == NULL) // cas de base: arbre vide: valeur non trouvee
    return false;
  
  if (k == t->val) // cas de base: valeur trouvee
  {
    *addr_node = t;
    *addr_parent = NULL;
    return true;
  }

  // cas d'arrêt un cran plus haut pour pouvoir récupérer le noeud parent
  if (t->left != NULL && t->left->val == k)
  {
    *addr_parent = t;
    *addr_node = t->left;
    return true;
  }
  if (t->right != NULL && t->right->val == k)
  {
    *addr_parent = t;
    *addr_node = t->right;
    return true;
  }
  
  if (k < t-> val)
    return aux_bst_find(k, t->left, addr_parent, addr_node);
  else 
    return aux_bst_find(k, t->right, addr_parent, addr_node);

}

// encapsulation
bool bst_find(elt_type k, bst *t, node **addr_node)
{
  node *tmp;
  return aux_bst_find(k, t, &tmp, addr_node);
}

// exercice 3: plus difficile: suppression d'un noeud dans un ABR
void bst_remove(bst *t, elt_type k)
{
  node *n = NULL;
  bool found;
  node *parent_min = NULL;
  node *parent_node = NULL;
  node *node_min = NULL;

  // on repere le noeud a supprimer dans la valeur avec son etiquette
  found = aux_bst_find(k, t, &parent_node, &n);
  if (found == false) // rien a faire car valeur k non presente dans l'ABR
    return;

  assert(n != NULL); // normalement, on a recupere l'adresse du noeud trouve

  if (parent_node == NULL)
    parent_node = t;

  
  // cas (a): le noeud à supprimer n'a aucun fils
  if (n->left == NULL && n->right == NULL)
  {
    if (parent_node != NULL)
    {
      // on coupe le lien entre ce noeud à supprimer et son parent
      if (parent_node->left == n) // attention ici, test d'égalite sur des adresse pour savoir si le noeud a supprimer est le fils gauche ou droit du noeud parent!!
	parent_node->left = NULL;
      else if (parent_node->right == n)
	parent_node->right = NULL;
    }
    
    free(n); // on supprime le noeud    
  }

  // cas (b): le noeud à supprimer à un seul fils: on reconnecte ce fils directement au parent
  else if (n->left == NULL)
  {
    printf("cas b\n");
    if (parent_node != NULL)
    {
      if (parent_node->left == n)
	parent_node->left = n->right;
      else if (parent_node->right == n)
	parent_node->right = n->right;
    }
    free(n);
  }
  else if (n->right == NULL)
  {
    if (parent_node != NULL)
    {
      if (parent_node->left == n)
	parent_node->left = n->left;
      else if (parent_node->right == n)
	parent_node->right = n->left;
    }
    free(n);
  }


  // cas (c): le noeud à supprimer à deux fils: on remplace son étiquette pas l'étiquette minimum de son sous-arbre droit et on supprime ce noeud minimum du sous arbre droit
  else
  {
    // on récupere le noeud min du sous arbre droit du noeud n a supprimer
    node_min = aux_bst_min(n->right, &parent_min);
    elt_type min = node_min->val; // on sauvegarde la valeur du min

    if (parent_min == NULL)
      parent_min = n;
    
    //on relance la fonction pour supprimer ce noeud min du sous-arbre droit
    bst_remove(parent_min, min);

    // on ecrase la valeur du noeud a supprimer en la remplacant par la valeur min du sous arbre droit que l'on avait sauvegardé
    n->val = min;
  }

  
  return;
  
}




int main(int argc, char **argv)
{
  node *n      = NULL;
  bst  *t      = NULL;
  bool found;


  // test 1: test des versions recursives

  // creation de l'arbre
  t = bst_insert(5, NULL);
  bst_insert(3, t);
  bst_insert(7, t);
  bst_insert(-2, t);
  bst_insert(4, t);
  bst_insert(8, t);

  bst_print_inorder(t);printf("\n");
  
  // test bst_find
  found = bst_find(7, t, &n);
  printf("Found:%d\n", found);
  if (found == true)
    printf("Node found: %d\n", n->val);
  
  // test bst_min
  n= bst_min(t);
  printf("min=%d\n", n->val);

  

  // test bst_remove

  bst_remove(t, 3);
  printf("After remove: "); bst_print_inorder(t); printf("\n");
  
  bst_remove(t, 8);
  printf("After remove: "); bst_print_inorder(t); printf("\n");

  bst_remove(t, 5);
  printf("After remove: "); bst_print_inorder(t); printf("\n");

  bst_free(&t);
  
  return 0;
}
