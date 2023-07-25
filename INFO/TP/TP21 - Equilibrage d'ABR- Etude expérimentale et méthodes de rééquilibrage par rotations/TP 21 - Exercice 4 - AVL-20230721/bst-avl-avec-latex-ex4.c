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
  int h; // stockage de la hauteur de chaque noeud
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
  
  int hl = -1;
  if (l != NULL)
  {
    l->parent = nod;
    hl = l->h;
  }
  
  nod->right = r;
  int hr = -1;
  if (r != NULL)
  {
    r->parent = nod;
    hr = r->h;
  }

  nod->h = 1 + MAX(hl, hr);
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

int bst_update_height(bst *t)
{
  if (t == NULL)
    return -1;

  t->h = 1 + MAX(bst_update_height(t->left), bst_update_height(t->right));
  return t->h;				      
}

int balance_factor(bst *t)
{
  if (t == NULL)
    return 0;
  
  int hl = -1;
  int hr = -1;

  if (t->left != NULL)
    hl = t->left->h;

  if (t->right != NULL)
    hr = t->right->h;

  return hr - hl;
}

bool aux_bst_is_avl(bst *t)
{
  
  if (t == NULL)
    return true;

  if (abs(balance_factor(t)) >= 2)
    return false;
  
  return ( aux_bst_is_avl(t->left) && aux_bst_is_avl(t->right) ); 
}

bool bst_is_avl(bst *t)
{
  bst_update_height(t); // il faut mettre a jour les hauteurs de chaque noeud pour etre certains que les facteurs d'équilibrage seront bien calculés 
  return aux_bst_is_avl(t);
}


void bst_print_preorder(bst *t)
{
  if (t == NULL)
    return;

  if (t->parent != NULL)
  {
    printf("h=%d, parent=", t->h);printf(elt_fmt, t->parent->val);printf(") ");
  }
  else
    printf("h=%d, parent=NULL) ", t->h);
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
    printf("h=%d, parent=", t->h);printf(elt_fmt, t->parent->val);printf(") ");
  }
  else
    printf("h=%d, parent=NULL) ", t->h);
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
    printf("h=%d, parent=", t->h);printf(elt_fmt, t->parent->val);printf(") ");
  }
  else
    printf("h=%d, parent=NULL) ", t->h);
  return;
				      
}

bst *bst_insert(elt_type k, bst *t)
{
 
  if (t == NULL)
    return bst_create(NULL, k, NULL);
  
  node * n = NULL;

   // on met à jour la hauteur de chaque noeud pour toute nouvelle insertion
   // on doit transmettre le changement de hauteur lié à l'insertion dans un sous arbre à l'appel récursif "du dessus", d'ou l'utilisation de addr_h pour permet de récupérer la nouvelle hauteur du sous arbre 
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



void aux_write_latex(bst *t, FILE *f)
{
  if (t == NULL)
  {
    fprintf(f, "{edge from parent[draw=none]}");
    return;
  }

  fprintf(f, "node[noeud,label=330:$%d$] {%d}\n", balance_factor(t), t->val);
  fprintf(f, "child\n{");
  aux_write_latex(t->left, f);
  fprintf(f, "}\n");
  fprintf(f, "child\n{");
  aux_write_latex(t->right, f);
  fprintf(f, "}\n");
}

FILE *write_latex_header(char *path)
{
  FILE *f = fopen(path, "w");
  assert(f != NULL);

  printf("Creation du fichier %s...\n", path);
  fprintf(f, "\\documentclass[a4paper,12pt]{report}\n");
  fprintf(f, "\\usepackage{tkz-tab}\n");
  fprintf(f, "\\begin{document}\n");

  return f;
  
}
void write_latex(bst *t, FILE *f)
{

  int h = bst_update_height(t);
  fprintf(f, "\\begin{tikzpicture}[label distance =-1mm]\n");
  fprintf(f, "\\tikzstyle{noeud}=[circle, minimum size=5mm, inner sep=0pt, fill=red!25!white]\n");

  int ini_dist = 40;
  for (int i = 1; i <= h+1; i++)
  {
    fprintf(f, "\\tikzstyle{level %d}=[sibling distance=%dmm, level distance=8mm]\n", i, ini_dist);
    ini_dist = 0.6*ini_dist;
  }
  fprintf(f, "\\");
  aux_write_latex(t, f);
	  
  fprintf(f, ";\n\\end{tikzpicture}\n\n");
  
}

void write_latex_end(FILE *f)
{
  fprintf(f, "\\end{document}\n");
  fclose(f);
}

void write_single_latex(bst *t, char *path)
{
  FILE *f = write_latex_header(path);
  write_latex(t,  f);
  write_latex_end(f);
}



// test AVL
void test(void)
{
  bst *t = bst_insert(3, NULL);
  t = bst_insert(2, t);
  t = bst_insert(5, t);
  t = bst_insert(4, t);
  t = bst_insert(6, t);
  t = bst_insert(7, t);
  
  FILE *f = write_latex_header("./test-avl.tex");
  write_latex(t, f);
  if (bst_is_avl(t) == true)
    printf("L'arbre est AVL\n");
  else
    printf("L'arbre n'est pas AVL\n");

  
  t = bst_insert(1, t);
  write_latex(t, f);
  if (bst_is_avl(t) == true)
    printf("L'arbre est AVL\n");
  else
    printf("L'arbre n'est pas AVL\n");
  write_latex_end(f);
  
  bst_free(&t);
  return;
}



int main(void)
{
  test();
  
  return 0;
}

