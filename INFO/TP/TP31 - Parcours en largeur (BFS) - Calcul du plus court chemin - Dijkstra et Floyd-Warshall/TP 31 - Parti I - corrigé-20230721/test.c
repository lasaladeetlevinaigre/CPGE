#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <float.h>
#include "mygraph.h"
#include "mystack.h"

// prototypes des fonctions qui vont être implémentées, qui doivent être connues lors de la compilation séparée de ce fichier, pour que le compilateur puisse effectuer l'analyse du code sans ambiguité
stack  *bfs(wgraph *g, int source);
double *dijkstra(wgraph *g, int source);
double *dijkstra_with_previous_vertex(wgraph *g, int ustart, int **addr_previous_vertex);
stack  *create_path(int nv, int ustart, int uend, int *previous_vertexr);


void print_dist_array(double *t, int n)
{
  assert(t != NULL);
  printf("\nDistances: ");
  double d;
  for (int v = 0; v < n; v++)
  {
    d = t[v]; 
    if (d == DBL_MAX)
      printf("%d (inf) ", v);
    else
      printf("%d(%lf) ", v, d);
  }
  printf("\n");
}

// fonction fournie, pour la création du graphe1
wgraph *create_graph1(void)
{
  int nv = 6;
  wgraph *g = wgraph_init_no_edge(nv, true);

  wgraph_add_edge(g, 1, 2, 1.);
  wgraph_add_edge(g, 1, 5, 1.);
  wgraph_add_edge(g, 2, 5, 1.);
  wgraph_add_edge(g, 2, 3, 1.);
  wgraph_add_edge(g, 3, 0, 1.);
  wgraph_add_edge(g, 3, 4, 1.);
  wgraph_add_edge(g, 3, 1, 1.);
  wgraph_add_edge(g, 4, 1, 1.);
  wgraph_add_edge(g, 3, 1, 1.);

  return g;
}

// fonction fournie, pour la création du graphe 2
wgraph *create_graph2(void)
{
  int nv = 8;
  wgraph *g = wgraph_init_no_edge(nv, true);

  wgraph_add_edge(g, 1, 4, 1.);
  wgraph_add_edge(g, 1, 3, 1.);
  wgraph_add_edge(g, 2, 3, 1.);
  wgraph_add_edge(g, 3, 2, 1.);
  wgraph_add_edge(g, 3, 0, 1.);
  wgraph_add_edge(g, 4, 0, 1.);
  wgraph_add_edge(g, 4, 1, 1.);
  wgraph_add_edge(g, 5, 2, 1.);
  wgraph_add_edge(g, 5, 7, 1.);
  wgraph_add_edge(g, 6, 2, 1.);
  wgraph_add_edge(g, 6, 5, 1.);
  wgraph_add_edge(g, 6, 1, 1.);
  wgraph_add_edge(g, 7, 6, 1.);
  return g;
}

// fonction fournie, pour la création du graphe-arbre
wgraph *create_graph3_tree(void)
{
  int nv = 11;
  wgraph *g = wgraph_init_no_edge(nv, false);

  wgraph_add_edge(g, 0, 1, 1.);
  wgraph_add_edge(g, 0, 7, 1.);
  wgraph_add_edge(g, 1, 2, 1.);
  wgraph_add_edge(g, 1, 3, 1.);
  wgraph_add_edge(g, 3, 4, 1.);
  wgraph_add_edge(g, 3, 4, 1.);
  wgraph_add_edge(g, 4, 5, 1.);
  wgraph_add_edge(g, 5, 6, 1.);
  wgraph_add_edge(g, 7, 8, 1.);
  wgraph_add_edge(g, 8, 9, 1.);
  wgraph_add_edge(g, 8, 10, 1.);
  return g;
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

void test_bfs_one_graph(wgraph *g, int source, char *name)
{
  assert(g != NULL);
  assert(name != NULL);
  char str[255];
  sprintf(str, "%d", source);
  
  printf("\n --- TEST %s SOMMET SOURCE %s ---\n", name, str);
  
  stack * s = bfs(g, source);
  stack_print(s);
  stack_free(&s);
  wgraph_free(&g);
}


void test_bfs()
{
  printf("\n **************************************** ");
  printf("\n *************** TEST BFS *************** ");
  printf("\n **************************************** \n");
  
  wgraph *g = create_graph1();
  test_bfs_one_graph(g, 1, "GRAPHE 1");

  g = create_graph1();
  test_bfs_one_graph(g, 3, "GRAPHE 1");

  g = create_graph2();
  test_bfs_one_graph(g, 1, "GRAPHE 1");
  
  g = create_graph3_tree();
  test_bfs_one_graph(g, 0, "GRAPHE 3 ARBRE");
  
}

void test_dijkstra_one_graph(wgraph *g, int ustart, char *name)
{
  assert(g != NULL);
  assert(name != NULL);
  char str[255];
  sprintf(str, "%d", ustart);
  
  printf("\n --- TEST %s SOMMET SOURCE %s ---\n", name, str);

  double *dist = dijkstra(g, ustart);
  int nv = wgraph_number_of_vertices(g);
  print_dist_array(dist, nv); 
  free(dist);
  wgraph_free(&g);
}

void test_dijkstra(void)
{
  printf("\n **************************************** ");
  printf("\n *************** TEST DIJKSTRA ********** ");
  printf("\n **************************************** \n");
  
  // test graph 4
  wgraph *g = create_graph4();
  test_dijkstra_one_graph(g, 0, "GRAPHE 4");

  // test graph 5
  g = create_graph5();
  test_dijkstra_one_graph(g, 0, "GRAPHE 5");
  
  return;
}

void print_and_free_paths(int nv, int ustart, double *dist, stack **paths)
{
  for (int uend = 0; uend < nv; uend ++)
  {
    if (dist[uend] == DBL_MAX)
    {
      printf(" Pas de chemin entre %d et %d\n\n", ustart, uend);
      continue;
    } 
    printf("\n Distance la plus courte entre %d et %d: %lf\n", ustart, uend, dist[uend]);
    printf("\tChemin le plus court: ");
    stack_print(paths[uend]);
    printf("\n");
    stack_free(&(paths[uend]));
  }
  free(paths);
}

// Fonction qui crée tous les plus courts chemins depuis ustart vers tous les sommets du graphe
stack **create_paths(int nv, int ustart, int *prv_ver)
{
  assert(nv >= 0);
  assert(ustart >= 0 && ustart < nv);
  assert(prv_ver != NULL);
  
  stack **paths = malloc(sizeof(stack *)*nv);
  assert(paths != NULL);

  
  // pour chaque sommet d'arrivée uend, on va reconstruire le chemin le plus court depuis la source jusqu'à uend grâce au tableau prv_ver (previous vertex) qui, pour chaque sommet v, nous donne le sommet d'avant dans le chemin le plus court de source -> v
  
  for (int uend = 0; uend < nv; uend++)
    paths[uend] = create_path(nv, ustart, uend, prv_ver);      
  
  
  return paths;
}


void test_dijkstra_with_paths_one_graph(wgraph *g, int ustart, char *name)
{
  assert(g != NULL);
  assert(name != NULL);
  char str[255];
  sprintf(str, "%d", ustart);
  
  printf("\n --- TEST %s SOMMET SOURCE %s ---\n", name, str);

   int nv = wgraph_number_of_vertices(g);
  int *previous_vertex = NULL;
  double *dist = dijkstra_with_previous_vertex(g, ustart, &previous_vertex);
  print_dist_array(dist, nv);

  stack **paths = create_paths(nv, ustart, previous_vertex);
  print_and_free_paths(nv, ustart, dist, paths);
  free(dist);
  free(previous_vertex);
  wgraph_free(&g);
}

void test_dijkstra_with_paths(void)
{

  printf("\n ************************************************** ");
  printf("\n *************** TEST DIJKSTRA WITH PATHS ********* ");
  printf("\n ************************************************** \n");
  
  wgraph *g = create_graph3_tree();
  test_dijkstra_with_paths_one_graph(g, 0, "GRAPH 3 (ARBRE)");
  
  // test graph 4
  g = create_graph4();
  test_dijkstra_with_paths_one_graph(g, 0, "GRAPH 4");

  // test graph 5
  g = create_graph5();
  test_dijkstra_with_paths_one_graph(g, 0, "GRAPH 5");
   
    
  return;
}


int main(int argc, char *argv[])
{
  //ex1
  //test_bfs();
  
  //ex2
  test_dijkstra();

  // ex3
  test_dijkstra_with_paths();

  
  return 0;
}
