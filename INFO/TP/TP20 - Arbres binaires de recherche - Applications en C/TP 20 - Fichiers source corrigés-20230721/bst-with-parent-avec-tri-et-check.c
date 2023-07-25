#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include "limits.h"

// ces deux lignes sont des macros: pour des fonctions très simples, comme MIN et MAX de deux entiers
#define MAX(x,y) ( ( (x) > (y) )  ? (x):(y) )
#define MIN(x,y) ( ( (x) < (y) )  ? (x):(y) )


#define elt_type int
#define elt_fmt  "%d"


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

int bst_number_of_nodes(bst *t)
{
  if (t == NULL)
    return 0;

  return 1 + bst_number_of_nodes(t->left) + bst_number_of_nodes(t->right);				      
}



int bst_height(bst *t)
{
  if (t == NULL)
    return -1;

  return 1 + MAX(bst_height(t->left), bst_height(t->right));				      
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


// exercice 2: fonction de manipulations bsaiques d'un ABR
// question 4
bst *bst_insert(elt_type k, bst *t)
{
 
  if (t == NULL)
    return bst_create(NULL, k, NULL);
  
  node * n = NULL;


  if (k <= t->val) // adaptation pour tri avec doublon, insertion valeur identique a gauche
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



  return t;
}

// question 6
bst *bst_insert_iter(elt_type k, bst *t)
{

  if (t == NULL)
    return bst_create(NULL, k, NULL);


  node *n = t;
  node *new_node = NULL;
  while (n != NULL)
  {
    
    if (k <= n->val) // adaptation pour tri avec doublon, insertion valeur identique a gauche
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

// question 7: on descend le plus à gauche possible pour aller chercher le min
elt_type bst_min_old(bst *t)
{
  if (t == NULL) // cas d'erreur
  {
    fprintf(stderr, "Impossible de renvoyer le min: l'arbre est vide!\n");
    return INT_MIN; // entier min representable, dans la lib limits.h
  }

  if (t->left == NULL) // cas d'arrêt
    return t->val;

  return bst_min_old(t->left);
}

// question 8
node *bst_min(bst *t)
{
  if (t == NULL) // cas d'erreur
  {
    fprintf(stderr, "Impossible de renvoyer le min: l'arbre est vide!\n");
    return NULL; 
  }
 

  if (t->left == NULL)
    return t;

  return bst_min(t->left);
}

// question 9
node *bst_min_iter(bst *t)
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

// question 10 puis 11
bool bst_find_iter(elt_type k, bst *t, node **addr_node)
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

// question 12
bool bst_find(elt_type k, bst *t, node **addr_node)
{ 
  if (t == NULL) // cas de base: arbre vide: valeur non trouvee
    return false;
  
  if (k == t->val) // cas de base: valeur trouvee
  {
    *addr_node = t;
    return true;
  }

  // cas d'arrêt un cran plus haut pour pouvoir récupérer le noeud parent
  if (t->left != NULL && t->left->val == k)
  {
    *addr_node = t->left;
    return true;
  }
  if (t->right != NULL && t->right->val == k)
  {
    *addr_node = t->right;
    return true;
  }
  
  if (k < t-> val)
    return bst_find(k, t->left, addr_node);
  else 
    return bst_find(k, t->right, addr_node);

}



// exercice 3: plus difficile: suppression d'un noeud dans un ABR
bst *bst_remove(bst *t, elt_type k)
{
  node *n = NULL;
  bool found;
  node *parent_node = NULL;
  node *node_min = NULL;

  // on repere le noeud a supprimer dans la valeur avec son etiquette
  found = bst_find(k, t, &n);
  if (found == false) // rien a faire car valeur k non presente dans l'ABR
  {
    fprintf(stderr, "Warning: la cle a retirer n'est pas presente dans la structure\n");
    return t;
  }
 
  assert(n != NULL); // normalement, on a recupere l'adresse du noeud trouve

  parent_node = n->parent;

  
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
      free(n); // on supprime le noeud   
    }
    else // cas particulier: on est en train de retirer le dernier noeud de l'arbre!! Il devient vide, il faut retourner NULL. C'est pour cela que l'on doàit retourner un bst, sinon, pas nécessaire
    {
      free(n); // on supprime le noeud
      return NULL;
    }
     
  }

  // cas (b): le noeud à supprimer à un seul fils: on reconnecte ce fils directement au parent
  else if (n->left == NULL)
  {
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
    node_min = bst_min_iter(n->right);
    elt_type min = node_min->val; // on sauvegarde la valeur du min


    //on relance la fonction pour supprimer ce noeud min du sous-arbre droit
    bst_remove(node_min->parent, min);

    // on ecrase la valeur du noeud a supprimer en la remplacant par la valeur min du sous arbre droit que l'on avait sauvegardé
    n->val = min;
    
  
  }

  
  return t;
  
}

// exercice 4: tri par ABR

void bst_fill_tab_inorder(bst *t, elt_type *tab, int *addr_idx)
{
  if (t == NULL)
    return;

  bst_fill_tab_inorder(t->left, tab, addr_idx);
  tab[*addr_idx] = t->val;
  *addr_idx = *addr_idx + 1;
  bst_fill_tab_inorder(t->right, tab, addr_idx);

  return;
}
 
int bst_sort(elt_type *tab, int n)
{
  assert(tab != NULL);
  
  bst *t = NULL;
  for (int i = 0; i < n; i++)
    t = bst_insert(tab[i], t);

  int idx = 0;
  bst_fill_tab_inorder(t, tab, &idx);

  bst_free(&t);
  return idx;
}


// exercice 5: verification de la propriete ABR sur un arbre binaire
bool aux_bst_check(bst *t, int *addr_min, int *addr_max)
{
  bool check_left;
  bool check_right;
  
  if (t == NULL)
    return true;

  if (t->left == NULL && t->right == NULL)
  {
    *addr_min = t->val;
    *addr_max = t->val;
    return true;
  }

  int vmin, vmax;
  if (t->left == NULL)
  {
    check_right = aux_bst_check(t->right, &vmin, &vmax);
    *addr_min = MIN(t->val, vmin);
    *addr_max = MAX(t->val, vmax);
    return (check_right && t->val < vmin);
  }

  if (t->right == NULL)
  {
    check_left = aux_bst_check(t->left, &vmin, &vmax);
    *addr_min = MIN(t->val, vmin);
    *addr_max = MAX(t->val, vmax);
    return (check_left && t->val > vmin);
  }
  
  check_left  = aux_bst_check(t->left, &vmin, &vmax);
  check_left = check_left && (t->val > vmax);
 
  
  check_right = aux_bst_check(t->right, &vmin, &vmax);
  check_right = check_right && (t->val < vmin);
  *addr_min = MIN(t->val, vmin);
  *addr_max = MAX(t->val, vmax);
  
  
  return (check_left && check_right);
	
}

bool bst_check(bst *t)
{
  int vmin, vmax;
  return aux_bst_check(t, &vmin, &vmax);  
}


void print_tab(int *tab, int n)
{
  assert(tab != NULL);

  for (int i=0; i < n; i++)
    printf("%d ", tab[i]);
  printf("\n");
}


int main(int argc, char **argv)
{
  bst  *t = NULL;

  
  // ******  test bst_sort  *************//

  // test sans doublon
  int tab[9] = {3, 6, 2, 9, 1, 8, 4, 0, 7}; // tableau statique alloue dans la pile, dans le bloc d'activation du main!
  bst_sort(tab, 9);
  print_tab(tab, 9);

   // test avec doublons
  int tab2[9] = {3, 6, 6, 9, 1, 1, 4, 0, 7}; 
  bst_sort(tab2, 9);
  print_tab(tab2, 9);

  // ******  test bst_check  *************//

  // test sur un vrai ABR
  t = bst_insert_iter(5, NULL);
  bst_insert_iter(3, t);
  bst_insert_iter(7, t);
  bst_insert_iter(-2, t);
  bst_insert_iter(4, t);
  bst_insert_iter(8, t);

  bst_print_inorder(t);printf("\n");

  bool is_bst = bst_check(t);
  printf("Check bst:%d\n", is_bst); 
  bst_free(&t);
  
  // test sur un arbre binaire qui n'est pas un ABR: on utilise bst_create pour generer un arbre binaire qui n'est pas un ABR
  node *n2 = bst_create(NULL, 2, NULL);
  node *n7 = bst_create(NULL, 7, NULL);
  node *n3 = bst_create(n2, 3, n7);
  node *n5 = bst_create(NULL, 5, NULL);
  t = bst_create(n3, 4, n5);
  
  bst_print_inorder(t);printf("\n");
  is_bst = bst_check(t);
  printf("Check bst:%d\n", is_bst);
  bst_free(&t);
  
  return 0;
}
