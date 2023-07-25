#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include "myhashtbl.h"
#include "mygraph.h"
#include "mystack.h"


double *dijkstra_with_previous_vertex(wgraph *g, int ustart, int **addr_previous_vertex);
stack *create_path(int nv, int ustart, int uend, int *previous_vertex);



hashtbl *create_cities_dictionnary(char *cities_filepath, char ***addr_num_to_city)
{
  assert(cities_filepath != NULL);
  FILE *f = fopen(cities_filepath, "r");
  assert(f != NULL);
  
  int nv;
  fscanf(f, "%d\n", &nv);
  hashtbl *ht = hashtbl_create(nv);
  char **num_to_city = malloc(nv*sizeof(char *));
  
  char city_name[1024];
  int city_number = 0;
 
  while(feof(f) == false)
  {
    fscanf(f, "%s\n", city_name);
    hashtbl_add(ht, city_name, city_number);
    num_to_city[city_number] = strdup(city_name);
    city_number ++;
  }
  fclose(f);
  
  if (city_number != nv)
  {
    fprintf(stderr, "Le nombre de villes indiqué %d ne correspond pas au nombre de lignes lues %d dans le fichier %s\n", nv, city_number, cities_filepath);
    *addr_num_to_city = NULL;
    free(num_to_city);
    hashtbl_free(&ht);
    return NULL;
  }

  *addr_num_to_city = num_to_city;
  return ht;
}

wgraph *create_cities_graph(int nv, char *distances_filepath)
{
  assert(distances_filepath != NULL);
  
  FILE *f = fopen(distances_filepath, "r");
  assert(f != NULL);

  int u, v;
  double d;

  wgraph *g = wgraph_init_no_edge(nv, false);
  while (feof(f) == false)
  {
    fscanf(f, "%d", &u);
    fscanf(f, "%d", &v);
    //printf("%d %d %d\n", u, v, nv);
    assert(u >= 0 && u < nv);
    assert(v >= 0 && v < nv);
    fscanf(f, "%lf", &d);
    assert(d>= 0);
    wgraph_add_edge(g, u, v, d);
  }
  fclose(f);
 
  return g;
}


void print_shortest_path(char *from, char *to)
{
  char **num_to_city = NULL;
  hashtbl *ht = create_cities_dictionnary("./cities.txt", &num_to_city);

  int nv = hashtbl_number_of_bindings(ht);
  wgraph *g = create_cities_graph(nv, "./distances.txt");

  int ustart, uend;
  if (hashtbl_find(ht, from, &ustart) == false)
  {
    fprintf(stderr, "Je ne trouve pas le ville de départ %s dans ma base de données...\n", from);
    hashtbl_free(&ht);
    return;
    
  }
  if (hashtbl_find(ht, to, &uend) == false)
  {
    fprintf(stderr, "Je ne trouve pas le ville d'arrivée %s dans ma base de données...\n", to);
    hashtbl_free(&ht);
    return;
  }
    
  int *previous_vertex = NULL;
  double *dist = dijkstra_with_previous_vertex(g, ustart, &previous_vertex);
  stack *path = create_path(nv, ustart, uend, previous_vertex);


  // Affichage du résulat
  printf("Distance la plus courte entre %s et %s: %2.2lf km\n", from, to, dist[uend]);
  printf("\tChemin le plus court: ");
  int v;
  while(stack_is_empty(path) == false)
  {
    v = stack_pop(path);
    if (stack_length(path) > 0)
      printf("%s -> ", num_to_city[v]);
    else
      printf("%s\n", num_to_city[v]);
  }


 
  free(previous_vertex);
  free(dist);
  for (int u = 0; u< nv; u++)
    free(num_to_city[u]); // attention, on a utilisé strdup...il faut libérer es chaines qui ont été allouées avec strdup dans le tas
  free(num_to_city);
  stack_free(&path);
  hashtbl_free(&ht);
  wgraph_free(&g);

  return;
}


int main(int argc, char *argv[])
{
  assert(argc == 3);

  char *from = argv[1]; // ville de départ donnée sur la ligne de commande
  char *to = argv[2]; // ville d'arrivée donnée sur la ligne de commande
  
  print_shortest_path(from, to);
  
  return 0;
}
