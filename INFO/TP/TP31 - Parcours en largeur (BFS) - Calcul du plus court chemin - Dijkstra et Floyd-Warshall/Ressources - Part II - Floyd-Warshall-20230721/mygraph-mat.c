#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include "mylist.h"


struct wgraph_t
{
  bool directed;
  int ne;
  int nv;
  double **mat; // matrice d'adjacence, donnant les arêtes entre sommets du graphe et le poids associé à l'arête. Si deux sommets ne sont pas liés par une arête, le poids est mis à DBL_MAX;
};
typedef struct wgraph_t wgraph;

wgraph *wgraph_create()
{
  wgraph *g = malloc(sizeof(wgraph));
  assert( g != NULL);
  g->ne = 0;
  g->directed = true;
  g->nv = 0;
  g->mat = NULL;
  return g;
}

wgraph *wgraph_init(double **adjacency_matrix, int nv)
{
  assert(adjacency_matrix != NULL);
  
  wgraph *g = wgraph_create();
  
  g->nv = nv;
  g->mat = adjacency_matrix;

  // forcage de la diagonale
  for (int v = 0; v < nv; v ++)
    g->mat[v][v] = 0.; // un sommet est toujours connecté avec lui meme
  
  g->directed = false;
  g->ne = 0;
  for (int u = 0; u < nv; u ++)
  {
    for(int v = 0; v < g->nv; v++)
    {
      if (g->mat[u][v] != g->mat[v][u])
	g->directed = true;
      
      if (g->mat[u][v] != DBL_MAX)
	g->ne = g->ne + 1;
    }
  }
  return g;
}

wgraph *wgraph_init_no_edge(int nv, bool directed)
{
  assert(nv > 0);
  
  wgraph *g = wgraph_create();
  g->ne = 0;
  g->nv = nv;
  g->directed = directed;
  g->mat = malloc(sizeof(double *)*nv);
  for (int u = 0; u < g->nv; u++)
  {
    g->mat[u] = malloc(sizeof(double)*nv);
    for (int v = 0; v < g->nv; v++)
      g->mat[u][v] = DBL_MAX;
    g->mat[u][u] = 0.; // un sommet est toujours connecté avec lui meme
  }

  return g;
}

void wgraph_free(wgraph **addr_g)
{
  assert(addr_g != NULL);
  wgraph *g = *addr_g;

  // libération du tableau bidimensionnel
  for (int u = 0; u < g->nv; u++)
    free(g->mat[u]);
  free(g->mat);

  // libération de la structure de graphe elle-même
  free(g);

  // remise à NULL du pointeur vers ce graphe (grace au passage par adresse) par sécurité
  *addr_g = NULL;
}

// ajout d'un arc de u -> v de poids weight
bool wgraph_add_edge(wgraph *g, int u, int v, double weight)
{
  assert(g!= NULL);
  assert(u < g->nv && u >= 0);
  assert(v < g->nv && v >= 0);

  if (g->mat[u][v] == DBL_MAX)
  {
    g->mat[u][v] = weight;
    g->ne = g->ne + 1; // mise à jour de l'invariant de structure
  
    if (g->directed == false)
      g->mat[v][u] = weight;

    return true;
  }
  
  
  return false;
}

// suppression de l'arc u -> v
bool wgraph_remove_edge(wgraph *g, int u, int v)
{
  assert(g!= NULL);
  assert(u < g->nv && u >= 0);
  assert(v < g->nv && v >= 0);

  if (g->mat[u][v] != DBL_MAX)
  {
    g->mat[u][v] = DBL_MAX;
    g->ne = g->ne - 1; // mise à jour de l'invariant de structure
  
    if (g->directed == false)
      g->mat[v][u] = DBL_MAX;

    return true;
  }
  
  
  return false;
}

int wgraph_number_of_vertices(wgraph *g)
{
  assert(g!= NULL);
  return g->nv;
}

int wgraph_number_of_edges(wgraph *g)
{
  assert(g!= NULL);
  return g->ne;
}

bool wgraph_has_edge(wgraph *g, int u, int v, double *w)
{
  assert(g!= NULL);
  assert(u < g->nv && u >= 0);
  assert(v < g->nv && v >= 0);

  if (g->mat[u][v] == DBL_MAX)
  {
    *w = DBL_MAX;
    return false;
  }
  if (u == v)
    *w = 0.;
  else
    *w = g->mat[u][v];
  
  return true;
  
}

void wgraph_print(wgraph *g)
{
  assert(g!= NULL);
  double val;

    printf("            ");
  for (int v = 0; v < g->nv; v++)
    printf("%8d ", v);
  printf("\n");
  for (int u = 0; u < g->nv; u++)
  {
    printf("Sommet %3d: ", u); // affiche l'entier sur 3 chiffres
    for (int v = 0; v < g->nv; v++)
    {
      val  = g->mat[u][v];
      if (val == DBL_MAX)
	printf("%8s ", "inf"); // %8s signifie afficher sur 8 caractères, quitte à remplir sur la gauche par des caractères vides
      else
	printf("%8.5f ", val); // %8.5f signifie: afficher 8 caractères de chiffres, le .5 signifie que l'on veut un affichage avec 5 chiffres apres la virgule
    }
    printf("\n");
  }

  return;
}

// fonction primordiale: renvoie les sommets successeurs d'un sommet u
list *wgraph_succ(wgraph *g, int u)
{
   fprintf(stderr, "wgraph_has_edge: ATTENTION:  pour les graphes par matrice d'adjacence, cette primitive est en O(nv)...Avez vous choisi la bonne implémentation concrète de graphe??\n");
   
  list *l = list_create();
  double weight;

  for (int v = 0; v < g->nv; v++)  
  {
    if (v == u) // on ne prend pas en compte l'interaction de v avec lui meme
      continue;
    
    weight = g->mat[u][v];
    if (weight != DBL_MAX)
      list_add(l, v, weight);   
  }
  return l;
}

