#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include "mylist.h"


struct wgraph
{
  bool directed;
  int ne;
  int nv;
  list **adj; // tableau de listes d'adjacences 
};
typedef struct wgraph wgraph;

wgraph *wgraph_create()
{
  wgraph *g = malloc(sizeof(wgraph));
  assert( g != NULL);
  g->ne = 0;
  g->directed = true;
  g->nv = 0;
  g->adj = NULL;
  return g;
}


wgraph *wgraph_init_no_edge(int nv, bool directed)
{
  assert(nv > 0);
  
  wgraph *g = wgraph_create();
  g->ne = 0;
  g->nv = nv;
  g->directed = directed;
  g->adj = malloc(sizeof(list *)*nv);
  for (int u = 0; u < g->nv; u++)
    g->adj[u] = list_create();

  return g;
}

void wgraph_free(wgraph **addr_g)
{
  assert(addr_g != NULL);
  wgraph *g = *addr_g;

  // libération du tableau bidimensionnel
  for (int u = 0; u < g->nv; u++)
  {
    if (g->adj[u] != NULL)
      list_free(&(g->adj[u]));
  }
  free(g->adj);

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

  assert(g->adj[u] != NULL);

  double tmp;
  if (list_find(g->adj[u], v, &tmp) == false) // pas de doublons
  {
    list_add(g->adj[u], v, weight);
    g->ne = g->ne + 1; // mise à jour de l'invariant de structure nombre d'arêtes
  
    if (g->directed == false) // on ajoute l'arete dans les deux 2 directions
      list_add(g->adj[v], u, weight);
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
  
  bool suppr = list_delete(g->adj[u], v);
  
  g->ne = g->ne - 1; // mise à jour de l'invariant de structure
  
  if (g->directed == false && suppr == true)
    list_delete(g->adj[v], u);

  return suppr;
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

double wgraph_get_weight(wgraph *g, int u, int v)
{
  assert(g!= NULL);
  assert(u < g->nv && u >= 0);
  assert(v < g->nv && v >= 0);

  assert(g->adj[u] != NULL);

  if (u == v)
    return 0.;
  
  double weight;
  
  if (list_find(g->adj[u], v, &weight) == true)
    return weight;
  
  
  return DBL_MAX;
}

void wgraph_print(wgraph *g)
{
  assert(g!= NULL);
  
  for (int u = 0; u < g->nv; u++)
  {
    assert(g->adj[u] != NULL);
    printf("Edges from %d: ", u);
    list_print(g->adj[u]);
    printf("\n");
  }

  return;
}

// fonction primordiale: renvoie les sommets successeurs d'un sommet u
list *wgraph_succ(wgraph *g, int u)
{
  assert(g != NULL);
  return g->adj[u];
  
}

