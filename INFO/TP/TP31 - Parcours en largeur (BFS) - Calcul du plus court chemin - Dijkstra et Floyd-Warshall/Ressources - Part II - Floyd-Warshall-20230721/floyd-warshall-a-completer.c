#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include <limits.h>

#include "mystack.h"
#include "mygraph.h"

void print_and_free_dist_matrix(double **mat, int nv)
{
  assert(mat != NULL);
  double d;
  printf("\nMatrice des distances:\n\n");
  printf("            ");
  for (int v = 0; v < nv; v++)
    printf("%8d ", v);
  printf("\n");
  for (int u = 0; u < nv; u++)
  {
    printf("Sommet %3d: ", u); // affiche l'entier sur 3 chiffres
    for (int v = 0; v < nv; v++)
    {
      d  = mat[u][v];
      if (d == DBL_MAX)
	printf("%8s ", "inf"); // %8s signifie afficher sur 8 caractères, quitte à remplir sur la gauche par des caractères vides
      else
	printf("%8.5f ", d); // %8.5f signifie: afficher 8 caractères de chiffres, le .5 signifie que l'on veut un affichage avec 5 chiffres apres la virgule
    }
    printf("\n");
    free(mat[u]);
  }
  free(mat);
  return;
}



void free_2darray(void **a, int nv)
{
  for (int u = 0; u < nv; u++)
    free(a[u]);
  free(a);
}

// fonction fournie, pour la création du graphe-arbre
wgraph *create_graph4(void)
{
  int nv = 10;
  wgraph *g = wgraph_init_no_edge(nv, true);
  wgraph_add_edge(g, 0, 1, 1.5);
  wgraph_add_edge(g, 0, 7, 2.);
  wgraph_add_edge(g, 0, 2, 0.5);
  wgraph_add_edge(g, 0, 4, 3.);
  wgraph_add_edge(g, 1, 2, 0.5);
  wgraph_add_edge(g, 2, 3, 2.);
  wgraph_add_edge(g, 2, 4, 1.);
  wgraph_add_edge(g, 3, 5, 1.5);
  wgraph_add_edge(g, 4, 6, 0.5);
  wgraph_add_edge(g, 6, 5, 0.5);
  wgraph_add_edge(g, 7, 6, 1.);
  wgraph_add_edge(g, 7, 8, 0.5);
  wgraph_add_edge(g, 8, 9, 0.5);
  wgraph_add_edge(g, 9, 6, 0.5);

  return g;
}

wgraph *create_graph5(void)
{
  int nv = 10;
  
  wgraph *g = wgraph_init_no_edge(nv, true);
  wgraph_add_edge(g, 0, 1, 0.5);
  wgraph_add_edge(g, 7, 0, 2.);
  wgraph_add_edge(g, 0, 2, 2);
  wgraph_add_edge(g, 0, 4, 1.);
  wgraph_add_edge(g, 1, 2, 0.5);
  wgraph_add_edge(g, 2, 3, 2.);
  wgraph_add_edge(g, 2, 4, 0.5);
  wgraph_add_edge(g, 5, 3, 1.5);
  wgraph_add_edge(g, 4, 6, 3.5);
  wgraph_add_edge(g, 5, 6, 0.5);
  wgraph_add_edge(g, 7, 6, 1.);
  wgraph_add_edge(g, 7, 8, 0.5);
  wgraph_add_edge(g, 8, 9, 0.5);
  wgraph_add_edge(g, 9, 6, 0.5);
  
  return g;
}



int main(void)
{
  //test_floyd_warshall();
  //test_floyd_warshall_with_paths();
  return 0;
}


