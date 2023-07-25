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



// Valeur de sortie: matrice des distances minimales entre sommets
// contrairement à l'algorithme de Dijksra, l'algorithme de Floyd-Warshall ne prend
// pas en entrée un source. Il calcule beaucoup plus que Dijkstra:
// il calcule POUR TOUTES LES SOURCES tous les chemins les plus courts vers tous les sommets
// Complexité temporelle: triple boucle imbriquée, donc en O(nv^3), très cher!
// Mais on calcule plus de choses que dans dijkstra, car on ne calcule ps les chemins pour une seule source comme dans Dijkstra.
double **floyd_warshall(wgraph *g)
{
  assert(g != NULL);

  int nv = wgraph_number_of_vertices(g);
  int u, v, k; // etiquettes de sommets
  double d; // distance
  
  // allocation et initilisation de la matrice des distances de plus court chemin
  // au départ, cette matrice est une copie de la matrice d'adjacence
  double **dist = malloc(nv*sizeof(double *));
  assert(dist != NULL);
  for (u = 0; u < nv; u++)
  {
    dist[u] = malloc(nv*sizeof(double));
    assert(dist[u] != NULL);
    for (v = 0; v < nv; v++)
      wgraph_has_edge(g, u, v, &(dist[u][v]));    
  }

  
  

  // Complexite cubique en O(nv^3) car trois boucles de sommets imbriquées!!
  // mais on calcule tous les plus courts chemins, pour toutes les sources
  for (k = 0; k < nv; k++) // passage par un sommet intermédiaire k
  { 
    for (u = 0; u < nv; u++)
    {
      for (v = 0; v < nv; v++) 
      {
	d = dist[u][k] + dist[k][v];
	if (d < dist[u][v]) // si en passant par k on trouve un chemin plus court de u à v, on met à jour la distance et on note k comme sommet intermédiaire entre u et v
	{ 
	  dist[u][v] = d;
	}
      }
    }
  }
  
  return dist;
}

// valeur de sortie: matrice des distances minimales entre sommets
// interm (passage par adresse): interm[u][v] sommet intermédiaire minimisant le parcours de u -> v
double **floyd_warshall_with_interm(wgraph *g, int ***addr_interm)
{
  assert(g != NULL);
  assert(addr_interm != NULL);

  int nv = wgraph_number_of_vertices(g);
  int u,v,k;
  double d;
  
  // allocation et initilisation de la matrice des distances
  double **dist = malloc(nv*sizeof(double *));
  assert(dist != NULL);
  for (u = 0; u < nv; u++)
  {
    dist[u] = malloc(nv*sizeof(double));
    assert(dist[u] != NULL);
    for (v = 0; v < nv; v++)
      wgraph_has_edge(g, u, v, &dist[u][v]);    
  }

  // allocation et initialisation du tableau 2D des sommets intermédiaires servant à reconstituer les chemins.
  // Ce tableau 2D est alloué dans la fonction et sera retransmis en sorti grâce
  // à un passage par adresse *addr_interm = interm à la fin
  
  int **interm = malloc(sizeof(int *)*nv);
  for (u = 0; u < nv; u++)
  {
    interm[u] = malloc(nv*sizeof(int));
    assert(interm[u] != NULL); // check allocation ok
    for (v = 0; v < nv; v++)
      interm[u][v] = -1;
  }
  
 
  
  for (k = 0; k < nv; k++) // passage par un sommet intermédiaire k
  { 
    for (u = 0; u < nv; u++)
    {
      for (v = 0; v < nv; v++) 
      {
	d = dist[u][k] + dist[k][v];
	if (d < dist[u][v]) // si en passant par k on trouve un chemin plus court de u à v, on met à jour la distance et on note k comme sommet intermédiaire entre u et v
	{ 
	  interm[u][v] = k;
	  dist[u][v] = d;
	}
      }
    }
  }
  
  *addr_interm = interm; // pour retourner le tableau des sommets intermédiaires par adresse
 
  return dist; // on renvoie la matrice des distances les plus courtes, comme avant
}

// Reconstruction récursive du plus court chemin entre u et v. 
void create_path_rec(wgraph *g, int **interm, int u, int v, stack *path)
{
  assert(g != NULL);

  int nv = wgraph_number_of_vertices(g);
  assert(u >= 0 && u < nv);
  assert(v >= 0 && v < nv);
  assert(interm != NULL);

  double w;
  int k = interm[u][v]; // récupération de l'étiquette du sommet intermédiaire sur chemin le plus court de u à v
    //printf("%d %d %d\n", u, v, k);
  if (k == -1)
  {
    if (wgraph_has_edge(g, u, v, &w) == true)
      stack_push(path, v); // On empile v que quand on a un vrai arc, car de base de la récursivité
  }
  else
  {
    // appel récursif pour reconstruire le chemin de k -> u car on empile à l'envers 
    create_path_rec(g, interm, u, k, path);
     // appel récursif pour reconstruire le chemin de v -> k 
    create_path_rec(g, interm, k, v, path);

    // on a empile le chemin v->u donc à l'envers
  }
  
  return;
}



stack *create_path(wgraph *g, int **interm, int vstart, int vend)
{
  assert(g != NULL);
  
  int nv = wgraph_number_of_vertices(g);
  
  assert(vstart >= 0 && vstart < nv);
  assert(vend >= 0 && vend < nv);
  assert(interm != NULL);
  
  // search path 
  stack *path = stack_create();
  stack_push(path, vstart);

  create_path_rec(g, interm, vstart, vend, path);

  if (stack_length(path) < 2) // pour gérer le cas particulier vstart == vend
    stack_pop(path);
   
  // comme on a empilé à l'envers, on renverse la pile pour bien avoir le chemin vstart->vend et non vend->vstart
  stack_reverse(path);
  
  return path;
}


stack *create_path_iter(wgraph *g, int **interm, int vstart, int vend)
{
  assert(g != NULL);
  int nv = wgraph_number_of_vertices(g);
  assert(vstart >= 0 && vstart < nv);
  assert(vend >= 0 && vend < nv);
  assert(interm != NULL);
  
  // pile qui sera renvoyée à la fin de l'algo et qui contiendra le chemin
  stack *path = stack_create();
  
  int u;
  int v;
  int k;
  double w;


  // pile de travail pour remplacer la récursivité
  stack *s = stack_create();

  
  
  stack_push(s, vend);
  stack_push(s, vstart);

  while (stack_is_empty(s) == false)
  {
    // la pile fonctionne par empilement deux sommets par deux sommets : on empile en fait des arcs 
    u = stack_pop(s);
    v = stack_pop(s);
    
    k = interm[u][v];
    //printf("%d %d %d\n", u, v, k);
    if (k == -1) // pas de sommet intermédiaire
    {
      if (wgraph_has_edge(g, u, v, &w) == true)
	stack_push(path, v);
    }
    else
    {
    
      // empilement de l'arc (u, k)
      stack_push(s,k);
      stack_push(s,u);
      // empilement de l'arc (k, v)
      stack_push(s,v);
      stack_push(s,k);

      // il faut les empiler dans cet ordre pour que le dépilement et
      // l'empilement dans la pile résultat path se fasse dans l'ordre u->v
    }
  }
 
  // libération de la pile de travail s
  stack_free(&s);
   
  stack_push(path, vstart);
  
  return path;
}


void test_floyd_warshall(void)
{
  printf("\n ********************************************** ");
  printf("\n *************** TEST FLOYD-WARSHALL ********** ");
  printf("\n ********************************************** \n");
  
  // test graph 4
  printf("\n --- TEST GRAPHE 4 ---\n");
  wgraph *g = create_graph4();
  double **dist_matrix = floyd_warshall(g);
  int nv = wgraph_number_of_vertices(g);
  print_and_free_dist_matrix(dist_matrix, nv); 
  wgraph_free(&g);

  // test graph 5
  printf("\n --- TEST GRAPHE 5 ---\n");
  g = create_graph5();
  dist_matrix = floyd_warshall(g);
  nv = wgraph_number_of_vertices(g);
  print_and_free_dist_matrix(dist_matrix, nv); 
  wgraph_free(&g);

  
  return;
}

void test_floyd_warshall_with_paths(void)
{
  printf("\n ************************************************************ ");
  printf("\n *************** TEST FLOYD-WARSHALL AVEC CHEMINS *********** ");
  printf("\n ************************************************************ \n");
  
  // test graph 4
  printf("\n --- TEST GRAPHE 4 ---\n");
  wgraph  *g = create_graph4();
  int    **interm = NULL;
  double **dist_matrix = floyd_warshall_with_interm(g, &interm);

  int ustart = 0;
  int uend = 5;
  
  double d = dist_matrix[ustart][uend];
  if (d < DBL_MAX)
    printf("Distance du plus court chemin entre %d et %d: %lf\n", ustart, uend, d);
  else
  {
    printf("Pas de chemin entre %d et %d\n", ustart, uend);
  }
  
  stack *path = create_path(g, interm, ustart, uend);
  printf("\tChemin le plus court (récursif): ");
  stack_print(path);
  stack_free(&path);
  path = create_path_iter(g, interm, ustart, uend);
  printf("\tChemin le plus court (itératif): ");
  stack_print(path);
  stack_free(&path);

  
  int nv = wgraph_number_of_vertices(g);
  free_2darray((void **)dist_matrix, nv);
  free_2darray((void **)interm, nv);
  wgraph_free(&g);

  // test graph 5
  printf("\n --- TEST GRAPHE 5 ---\n");
  
  g = create_graph5();
  interm = NULL;
  dist_matrix = floyd_warshall_with_interm(g, &interm);

  ustart = 0;
  uend = 6;
  
  d = dist_matrix[ustart][uend];
  if (d < DBL_MAX)
    printf("Distance du plus court chemin entre %d et %d: %lf\n", ustart, uend, d);
  else
  {
    printf("Pas de chemin entre %d et %d\n", ustart, uend);
  }
  path = create_path(g, interm, ustart, uend);
  printf("\tChemin le plus court (récursif): ");
  stack_print(path);
  stack_free(&path);
  path = create_path_iter(g, interm, ustart, uend);
  printf("\tChemin le plus court (itératif): ");
  stack_print(path);
  stack_free(&path);
  
  nv = wgraph_number_of_vertices(g);
  free_2darray((void **)dist_matrix, nv);
  free_2darray((void **)interm, nv);
  wgraph_free(&g);
  
  return;
}

int main(void)
{
  test_floyd_warshall();
  test_floyd_warshall_with_paths();
  return 0;
}


