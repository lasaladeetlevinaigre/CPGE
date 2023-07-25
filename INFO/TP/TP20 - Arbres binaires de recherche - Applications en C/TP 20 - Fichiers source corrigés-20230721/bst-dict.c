#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include "limits.h" // pour INT_MIN
#include <string.h> // pour strdup et strcmp

// ces deux lignes sont des macros: pour des fonctions très simples, comme MIN et MAX de deux entiers
#define MAX(x,y) ( ( (x) > (y) )  ? (x):(y) )
#define MIN(x,y) ( ( (x) < (y) )  ? (x):(y) )

#define key_type char *
#define key_fmt  "%s"

#define elt_type int
#define elt_fmt  "%d"


struct node
{
  key_type key;
  elt_type val;
  struct node *left;
  struct node *right;
  struct node *parent;
};

typedef struct node node;
typedef struct node bst;



bst *bstdict_create(bst *l, key_type k, elt_type v, bst *r)
{

  node *nod = malloc(sizeof(node));
  assert(nod != NULL);

  nod->parent = NULL;
  nod->key = strdup(k);
  nod->val = v;
  nod->left = l;
  if (l != NULL)
    l->parent = nod;
  
  nod->right = r;
  if (r != NULL)
    r->parent = nod;
  
  return nod;
}

void node_free(node **addr_n)
{
  assert(addr_n != NULL);
  node *n = *addr_n;
  if (n->key != NULL) // liberation de l'espace alloue par strdup dans le tas pour la chaine de caractere
    free(n->key);
  free(n);
  *addr_n = NULL;
}

void bstdict_free(bst **addr_t)
{
  assert(addr_t != NULL);
  bst *t = *addr_t;
  if (t == NULL)
    return;
  
  if (t->key != NULL) // liberation de l'espace alloue par strdup dans le tas pour la chaine de caractere
    free(t->key);
  
  bstdict_free(&(t->left));
  bstdict_free(&(t->right));
  free(t);
  *addr_t = NULL;

  return;
}

int bstdict_number_of_entries(bst *t)
{
  if (t == NULL)
    return 0;

  return 1 + bstdict_number_of_entries(t->left) + bstdict_number_of_entries(t->right);				      
}

key_type max_key(key_type x, key_type y)
{
  if (strcmp(x,y) > 0)
    return x;
  else
    return y;
}
key_type min_key(key_type x, key_type y)
{
  if (strcmp(x, y) < 0)
    return x;
  else
    return y;
}

int compare_key(key_type x, key_type y)
{
  return strcmp(x, y);
}

int bstdict_height(bst *t)
{
  if (t == NULL)
    return -1;

  return 1 + MAX(bstdict_height(t->left), bstdict_height(t->right));				      
}

int bstdict_leaves(bst *t)
{
  if (t == NULL)
    return 0;

  if (t->left == NULL && t->right == NULL)
    return 1;
  
  return bstdict_leaves(t->left) + bstdict_leaves(t->right);				      
}

void bstdict_print_preorder(bst *t)
{
  if (t == NULL)
    return;
  
  printf("("); printf(key_fmt, t->key); printf(","); printf(elt_fmt, t->val);printf(";");
  if (t->parent != NULL)
  {
     printf("parent=");printf(key_fmt, t->parent->key);printf(") ");
  }
  else
    printf("parent=NULL) ");
  bstdict_print_preorder(t->left);
  bstdict_print_preorder(t->right);

  return;
				      
}

void bstdict_print_inorder(bst *t)
{
  if (t == NULL)
    return;
  
  bstdict_print_inorder(t->left);
  printf("("); printf(key_fmt, t->key); printf(","); printf(elt_fmt, t->val);printf(";");
  if (t->parent != NULL)
  {
     printf("parent=");printf(key_fmt, t->parent->key);printf(") ");
  }
  else
    printf("parent=NULL) ");
  bstdict_print_inorder(t->right);

  return;
				      
}

void bstdict_print_postorder(bst *t)
{
  if (t == NULL)
    return;
  
  bstdict_print_postorder(t->left);  
  bstdict_print_postorder(t->right);
  printf("("); printf(key_fmt, t->key); printf(","); printf(elt_fmt, t->val);printf(";");
  if (t->parent != NULL)
  {
     printf("parent=");printf(key_fmt, t->parent->key);printf(") ");
  }
  else
    printf("parent=NULL) ");

  return;
				      
}

// exercice 2: fonction de manipulations bsaiques d'un ABR
// question 4
bst *bstdict_insert(key_type k, elt_type v, bst *t)
{
 
  if (t == NULL)
    return bstdict_create(NULL, k, v, NULL);
  
  node * n = NULL;
  
  if (compare_key(k, t->key) < 0)
  {
    n = bstdict_insert(k, v, t->left);
    t->left = n;
    n->parent = t;
  }
  else if (compare_key(k, t->key) > 0)
  {
    n = bstdict_insert(k, v, t->right);
    t->right = n;
    n->parent = t;
  }

  // cas d'egalite: on ne fait rien

  return t;
}

// question 6
bst *bstdict_insert_iter(key_type k, elt_type v, bst *t)
{

  if (t == NULL)
    return bstdict_create(NULL, k, v, NULL);


  node *n = t;
  node *new_node = NULL;
  while (n != NULL)
  {
    if (compare_key(k, n->key) == 0) // la valeur est déjà présente, on n'a rien à insérer
      return t;
    
    else if (compare_key(k, n->key) < 0)
    {
      if (n->left == NULL)
      {
	new_node = bstdict_create(NULL, k, v, NULL); // on cree le nouveau noeud isole
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
	new_node = bstdict_create(NULL, k, v, NULL);
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
elt_type bstdict_min_old(bst *t)
{
  if (t == NULL) // cas d'erreur
  {
    fprintf(stderr, "Impossible de renvoyer le min: l'arbre est vide!\n");
    return INT_MIN; // entier min representable, dans la lib limits.h
  }

  if (t->left == NULL) // cas d'arrêt
    return t->val;

  return bstdict_min_old(t->left);
}

// question 8
node *bstdict_min(bst *t)
{
  if (t == NULL) // cas d'erreur
  {
    fprintf(stderr, "Impossible de renvoyer le min: l'arbre est vide!\n");
    return NULL; 
  }
 
  if (t->left == NULL)
    return t;

  return bstdict_min(t->left);
}

// question 9
node *bstdict_min_iter(bst *t)
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
bool bstdict_find_iter(key_type k, bst *t, node **addr_node)
{
  *addr_node = NULL;
  
  node *n = t; 
  while (n != NULL)
  {
    if (compare_key(k, n->key) == 0)
    {
      *addr_node = n;
      return true;
    }
    
    if (compare_key(k, n->key) < 0)
      n = n->left;
    else 
      n = n->right;
  }
  
  return false;
}

// question 12
bool bstdict_find(key_type k, bst *t, node **addr_node)
{ 
  if (t == NULL) // cas de base: arbre vide: valeur non trouvee
    return false;
  
  if (compare_key(k, t->key) == 0) // cas de base: valeur trouvee
  {
    *addr_node = t;
    return true;
  }

  // cas d'arrêt un cran plus haut pour pouvoir récupérer le noeud parent
  if (t->left != NULL && t->left->key == k)
  {
    *addr_node = t->left;
    return true;
  }
  if (t->right != NULL && t->right->key == k)
  {
    *addr_node = t->right;
    return true;
  }
  
  if (compare_key(k, t->key) < 0)
    return bstdict_find(k, t->left, addr_node);
  else 
    return bstdict_find(k, t->right, addr_node);

}



// exercice 3: plus difficile: suppression d'un noeud dans un ABR
bst *bstdict_remove(key_type k, bst *t)
{
  node *n = NULL;
  bool found;
  node *parent_node = NULL;
  node *node_min = NULL;

  // on repere le noeud a supprimer dans la valeur avec son etiquette
  found = bstdict_find(k, t, &n);
  if (found == false) // rien a faire car valeur k non presente dans l'ABR
  {
    fprintf(stderr, "Warning: la clé à retirer n'est pas presente dans la structure\n");
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

      node_free(&n); // on supprime le noeud: attention, on doit bien libérer l'espace alloué par strdup, ce qui est fait dans node_free!
    }
    else // cas particulier ou on retire le tout dernier noeud du dictionnaire
    {
      node_free(&n); // on supprime le noeud
      return NULL; // on doit renvoyer un arbre vide, c'est uniquement pour cette raison qu'on a besoin de retourner un bst*
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
    node_free(&n);
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
    node_free(&n);
  }


  // cas (c): le noeud à supprimer à deux fils: on remplace son étiquette pas l'étiquette minimum de son sous-arbre droit et on supprime ce noeud minimum du sous arbre droit
  else
  {
    // on récupere le noeud min du sous arbre droit du noeud n a supprimer
    node_min = bstdict_min_iter(n->right);
    key_type min = node_min->key; // on sauvegarde la valeur du min


    //on relance la fonction pour supprimer ce noeud min du sous-arbre droit
    bstdict_remove(min, node_min->parent);

    // on ecrase la valeur du noeud a supprimer en la remplacant par la valeur min du sous arbre droit que l'on avait sauvegardé
    n->key = min;
    
  
  }

  
  return t;
  
}





int main(int argc, char **argv)
{
  node *n= NULL;
  bool found;

  bst *d = bstdict_insert("jean-corentin", 4, NULL);
  d = bstdict_insert("tristan2", 28, d);
  d = bstdict_insert("jean-stephane", 8, d);
  d = bstdict_insert("tristan3", 11, d);
  d = bstdict_insert("jean-michel", 2, d);
  d = bstdict_insert("tristan1", 3, d);
  d = bstdict_insert("celia", 20, d);
  bstdict_print_inorder(d); printf("\n");
 
  found = bstdict_find("tristan2", d, &n);
  if (found == true)
    printf("%s a été trouvé! Il est associé à la valeur %d\n", n->key, n->val);
 
  bstdict_remove("tristan3", d);
  bstdict_print_inorder(d); printf("\n");
  
  bstdict_free(&d);
  return 0;
}
