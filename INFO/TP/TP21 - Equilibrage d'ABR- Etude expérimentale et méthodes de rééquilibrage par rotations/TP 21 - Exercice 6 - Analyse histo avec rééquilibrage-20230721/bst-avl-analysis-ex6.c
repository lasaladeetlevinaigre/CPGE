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

bst *bst_insert_balance_iter(elt_type k , bst *t)
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
        break; // on casse la boucle
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
        break;  // on casse la boucle
      }
      n = n->right;
    }
  }

  // on peut même s'affranchir de cet appel à bst_update_height en recalculant juste les hauteurs nécessaires à la volée dans la boucle while ci-dessous... c'est fait ci-dessous.
  bst_update_height(t);


  // correction AVL

  // On commence par chercher le noeud au-dessus le plus proche du noeud d'insertion dont le bf n'est plus bon
  n = new_node;
  int newh;
  n->h = 0;
  int b = balance_factor(n);
  while (n->parent!= NULL && b != 2 && b !=(-2) )
  {
     newh = MAX(n->parent->h, n->h + 1);
     n = n->parent;
     n->h = newh;
     b = balance_factor(n);
  } // cette boucle remonte a partir du noeud inséré et  s'arrête au premier noeud n qui a un bf non acceptable (+2 ou - 2)

  // On corrige ce sous arbre n uniquement
  if (b == -2) // déséquilibre à gauche
  {
     int bl = balance_factor(n->left);
     if (bl == 1)// déséquilibre à droite du sous-arbre gauche --> double rotation gauche puis droite
     { 
       //printf("(Double) rotation gauche autour de %d\n", n->left->val);
        rotate_left(n->left);
     }
     //printf("Rotation droite autour de %d\n", n->val);
     rotate_right(n);
  }
  else if (b == 2) // déséquilibre à droite
  {
     int br = balance_factor(n->right);
     if (br == -1) // déséquilibre à gauche du sous-arbre droite --> double rotation droite puis gauche
     {
       //printf("(Double) rotation droite autour de %d\n", n->right->val);
       rotate_right(n->right);
     }
     //printf("Rotation gauche autour de %d\n", n->val);
     rotate_left(n);
     
  }

  bst_update_height(t);
  return t;
}


bst *bst_insert_balance(elt_type k, bst *t)//, int *addr_h)
{
 
  if (t == NULL)
    return bst_create(NULL, k, NULL);

  bst_update_height(t);
   
  node * n = NULL;

  if (k < t->val)
  {
    n = bst_insert_balance(k, t->left);
    t->left = n;
    n->parent = t;
    bst_update_height(t);
    
  
    if (balance_factor(t) == -2) // déséquilibre à gauche
    {
      if (balance_factor(t->left) == 1) // déséquilibre à droite du fils gauche
	{
	  //printf("(Double) rotation gauche autour de %d\n", t->left->val);
	   rotate_left(t->left);
	}
      //printf("Rotation droite autour de %d\n", t->val);
      rotate_right(t); 
    }

    return t;

  }
  else if (k > t->val)
  {

    n = bst_insert_balance(k, t->right);
    t->right = n;
    n->parent = t;
    bst_update_height(t);

    if (balance_factor(t) == 2) // déséquilibre à droite
    {
      if (balance_factor(t->right) == -1) // déséquilibre à gauche du fils droit
	{
	  //printf("(Double) rotation droite autour de %d\n", t->right->val);
	  rotate_right(t->right);
	  
	}
      //printf("Rotation gauche autour de %d\n", t->val);
      rotate_left(t);
    }
  }

  return t;
}


bst *bst_create_random_unbalanced(int n_nodes)
{
  int a = 0;
  int b = RAND_MAX;
  bst *t = NULL;
  int v;
  for (int i = 0; i < n_nodes; i++)
  {
    v = a + rand()%(b-a+1);
    t = bst_insert(v, t);
  }
  return t;
}

bst *bst_create_random_balanced(int n_nodes)
{
  int a = 0;
  int b = RAND_MAX;
  bst *t = NULL;
  int v;
  for (int i = 0; i < n_nodes; i++)
  {
    v = a + rand()%(b-a+1);
    t = bst_insert_balance_iter(v, t);
  }
  return t;
}

bst *bst_create_random(int n_nodes, bool balanced)
{
  if (balanced == true)
    return bst_create_random_balanced(n_nodes);
  else
    return bst_create_random_unbalanced(n_nodes);
}

void write_barchart(char *path, int n_samples, int n_nodes, bool balanced)
{

  int *barchart = calloc((n_nodes-1), sizeof(int)); // tableau qui va nous servir a stocker l'histogramme: pour chaque hauteur, nombre d'arbres aléatoirement générés ayant cette hauteur. On a vu que, au pire, la hauteur generee est celle d'un arbre peigne, donc n-1
  // calloc permet d'allouer + initialiser tous les bits à 0
  int h;
  bst *t = NULL;
  int hmax = -1;

  // calcul de l'histogramme
  for (int k = 0; k < n_samples; k++)
  {
    t = bst_create_random(n_nodes, balanced); // creation d'un abr aleatoire

    h = bst_update_height(t); // calcul de sa hauteur
    
    if (balanced == true && bst_is_avl(t) == false)
    {
      printf("PB\n");
      exit(1);
    }
    
    if (h > hmax) 
      hmax = h;
    
    barchart[h] = barchart[h] + 1;
    bst_free(&t); // liberation de l'arbre
  }


  // ecriture du fichier de l'histogramme
  FILE *f = fopen(path, "w");
  assert(f != NULL); // verif fichier ouvert correctement

  // on sauvegarde dans le fichier le nombre de noeuds utilisés pour l'analyse
  fprintf(f, "%d\n", n_nodes);

  // on sauvegarde l'histo sous la forme d'une fichier csv
  for (h = 0; h <= hmax; h++)
     fprintf(f, "%d,%d\n", h, barchart[h]);
  
  fclose(f);

  free(barchart); // on libère l'espace memoire alloue pour le stockage de l'histo

  printf("Fichier d'analyse %s correctement écrit.\n", path);
  return;
}

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "Erreur: il faut 2 valeurs en entrées:\n - nombre de noeuds des ABR générés aléatoirement\n - puis nombre d'arbres générés pour effectuer l'analyse\n");
    return 1;
  }

  int n_samples = atoi(argv[1]);
  if (n_samples <= 0)
  {
    fprintf(stderr, "Erreur: le nombre d'arbres à générer aléatoirementfourni en entrée est <= 0!\n");
    return 1;
  }
  
  int n_nodes = atoi(argv[2]);
  if (n_nodes <= 0)
  {
    fprintf(stderr, "Erreur: le nombre de noeuds donné pour les arbres est <= 0!\n");
    return 1;
  }
  

  
  // initialisation du terme u0 (s pour seed -> germe) de la suite pseudo-aléatoire
  srand(time(NULL));

  printf("Génération de %d ABR de %d noeuds chacun pour analyse de hauteur...\n", n_samples, n_nodes);
  
  write_barchart("./random_bst.csv", n_samples, n_nodes, true);
  write_barchart("./random_bst_unbalanced.csv", n_samples, n_nodes, false);
 
  return 0;
}



