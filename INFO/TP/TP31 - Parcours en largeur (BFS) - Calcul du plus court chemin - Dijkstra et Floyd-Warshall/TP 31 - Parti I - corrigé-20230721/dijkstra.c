#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include <limits.h>

#include "mystack.h"
#include "mypqueue.h"
#include "mygraph.h"



// Complexité temporelle totale de la fonction en O(nelog(nv)) au pire (cf détail à l'intérieur de la fonction) si on utilise une implémentation de graphes par liste d'adjacence, qui renvoie en O(1) la liste des successeurs d'un sommet et une implémentation de file de priorité par tas (insertion dans un tas en log(nv))
// Complexité spatiale dans le segment de tas: O(nv) pour dist , O(ne) pour la file de priorité, borne qui peut être atteinte, donc O(ne+nv). 
// Complexité spatiale dans le segment de pile: O(1) car pas de récursivité, donc pas d'empilement de blocs d'activation
double *dijkstra(wgraph *g, int ustart)
{
  assert(g != NULL);
  int nv = wgraph_number_of_vertices(g);
  int ne = wgraph_number_of_edges(g);
  
  assert(ustart >= 0 && ustart < nv);

  int u;
  int v;
  // tableau qui va être rempli et renvoyé par la fonction
  // A l'issue de l'algo, pour chaque sommet u, dist[u] contiendra la distance la plus courte possible entre la source ustart et ce sommet u.
  double *dist = malloc(nv*sizeof(double));
  assert(dist != NULL); // vérification de l'allocation

  

  // initialisation du tableau: tous les sommets sont considérés initialement
  // comme à distance infinie de la source. L'exploration BFS va mettre à jour ces valeurs.
  for (v = 0; v < nv; v++) // Complexité temporelle O(nv)
    dist[v] = DBL_MAX;
  
  /*bool *visited = malloc(sizeof(bool)*nv);
  for (v = 0; v < nv; v++)
  visited[v] = false;*/
  
  // creation d'une file de propriété dont les éléments sont des couples (keyclé, valeur). Un élément (k,v) correspond à
  // v=une étiquette d'un sommet du graphe (entier),
  // k=clé=distance de la source jusqu'à v
  // la structure de données de file de priorité nous permet de pouvoir, à chaque étape du parcours, de repartir du sommet le plus proche de la source pour recherche un plus court chemin
  pqueue *q = pqueue_create(ne, true); // on a, au maximum, n_e clés/poids différentes donc n_e noeuds dans la file de priorités
  list *lsucc = NULL;

  double duv; // distance entre u et v (poids de l'arc (u, v))
  double du; // distance de la source jusqu'à u
  double dv;

  // Complexité temporelle: on boucle au pîre nv fois si on a O(nv) sommet dans la pile,
  // ce qui peut arriver pour des graphes linéaires (= graphes dégénérés qui ressemblent à des listes)!

   // on part bien sur du sommet source ustart, que l'on
  // place dans la file de priorité... avec une distance 0 (sa clé dans la file de priorité) à lui même
  pqueue_enqueue(q, 0., ustart);
  dist[ustart] = 0.;
  
  while (pqueue_is_empty(q) == false) // tant que la file de priorité n'est pas vide
  {
    // récupération du sommet qui est le plus proche de la source, à distance du
    u = pqueue_dequeue(q, &du);

    // récupération de la liste des successeurs de ce sommet (en O(1) grace à l'implémentation par liste d'adjacence   
    lsucc = wgraph_succ(g, u); // Complexité temporelle: récupération de la liste des successeurs en O(1) graec aux listes d'adjacence... pas la même histoire si nous avions utilisé un matrice d'adjacence!

    // parcours des successeurs
   // boucle for: valeur de démarrage de l'itérateur, puis condition pour continuer, puis comme on passe d'une valeur de l'itérateur à la suivante
    for (cell *c = list_first_cell(lsucc); c != NULL;  c =  list_next_cell(c))
    {
      list_get_cell_data(c, &v, &duv); // duv = w = poids = distance arc (u,v)
    
      // pour chaque successeur v, calcul de la distance du chemin source -> u -> v
      dv = du + duv;
      if (dv < dist[v]) // si ce chemin est plus court qu'un chemin source -> v précédement trouvé
      {
	dist[v] = dv; // mise à jour de la distance la plus courte source ->v
	pqueue_enqueue(q, dist[v], v); // on rentrer v dans la liste des prochains sommets à traiter lors de l'avancée BFS
	// Complexité temporelle de enqueue en O(log(nv)) au pire avec une implementation par tas (= arbre binaire complet avec relation d'ordre)
      }
     
    }
    /* Autre syntaxe plus classique pour parcourir la liste des sucesseurs, avec un while
    c = list_first_cell(lsucc); 
    while (c != NULL)
    {

      list_get_cell_data(c, &v, &duv);
      
      ...

      c =  list_next_cell(c);
    }*/
  }

  pqueue_free(&q);

  return dist;
}


// Cout de création d'un chemin à partir de previous_vertex en O(nv) au pire (cas d'un graphe linéaire)
stack *create_path(int nv, int ustart, int uend, int *prv_ver)
{
   assert(nv >= 0);
   assert(ustart >= 0 && ustart < nv);
   assert(prv_ver != NULL);

   stack *path = stack_create();

  // pour chaque sommet d'arrivée uend, on va reconstruire le chemin le plus court depuis la source jusqu'à uend grâce au tableau prv_ver (previous vertex) qui, pour chaque sommet v, nous donne le sommet d'avant dans le chemin le plus court de source -> v

   int v = uend; // on se place à destination
   bool no_path = false;
   while (v != ustart) // on remonte le chemin le plus court jusqu'à la source
   {
      stack_push(path, v); // on empile les sommets en parcourant le chemin à l'envers, destination -> source
      v = prv_ver[v]; // on recule sur le chemin grace au tableau prv_ver

      if (v == -1) // dans ce cas, uend n'est pas accessible depuis la source
      {
	stack_free(&path); // il n'y a pas de chemin, donc on libère ce que l'on avait commencé à empiler
	path = stack_create();
	no_path = true;
	break;
      }
   }
   if (no_path == false) // s'il y a bien un chemin source ->uend
     stack_push(path, ustart); // on rajoute le sommet source au tout début

   return path;
}



double *dijkstra_with_previous_vertex(wgraph *g, int ustart, int **addr_previous_vertex)
{

  assert(g != NULL);  
  assert(addr_previous_vertex != NULL);
  *addr_previous_vertex = NULL;
  
  int nv = wgraph_number_of_vertices(g); 
  assert(ustart >= 0 && ustart < nv);
  
  int ne = wgraph_number_of_edges(g);

  int u;
  int v;


  double *dist = malloc(nv*sizeof(double));
  assert(dist != NULL);
  int *previous_vertex = malloc(nv*sizeof(int));
  assert(previous_vertex != NULL);
 
  // on fait toute les initialisations en même temps pour optimiser
  for (v = 0; v < nv; v++)
  {
    dist[v] = DBL_MAX;
    previous_vertex[v] = -1;
  }
   
   
  // creation une file de propriété dont les éléments sont des couples (key, val). Un élément (k,v) correspond à v=un sommet du graphe (entier),
  // k=clé=distance de la source jusqu'à v. On a maximum ne clés/poids différénts, donc en entrées dans la file
  pqueue *q = pqueue_create(ne, true);
  list *lsucc = NULL;

  double duv; // distance entre u et v (poids)
  double du; // distance de la source jusqu'à u
  double dv;

  pqueue_enqueue(q, 0., ustart);
  dist[ustart] = 0.;
  previous_vertex[ustart] = ustart;
  
  while (pqueue_is_empty(q) == false)
  {
    u = pqueue_dequeue(q, &du);
    
    lsucc = wgraph_succ(g, u);

    // parcours des successeurs immediats  
    for (cell *c = list_first_cell(lsucc); c != NULL;  c =  list_next_cell(c))
    {
      list_get_cell_data(c, &v, &duv); // duv = w = poids = distance arc (u,v)
      

      dv = du + duv;
      if (dv < dist[v])
      {
	dist[v] = dv;

	previous_vertex[v] = u; // on met un nouveau sommet de provenance, plus performant
	pqueue_enqueue(q, dist[v], v);
      }
    }
  }

  pqueue_free(&q);

  // on renvoie le tableau des sommets précédents sur le plus court chemin
  // avec un passage par adresse
  *addr_previous_vertex = previous_vertex;
  

  // on renvoie le tableau des distances les plus courtes entre la source et tous les sommets v du graphe.
  // dist[v] donne la longueur (flottant) du chemin le plus court de ustart vers v, et décrit par paths[v]
  return dist;
}





