#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "mygraph.h"
#include "mystack.h"
#include "myqueue.h"


stack *bfs(wgraph *g, int source)
{
  assert(g != NULL);
 
  int nv = wgraph_number_of_vertices(g);

  assert(source >= 0 && source < nv);

  int u, v;
  double w;
  
  
  bool *visited = malloc(nv*sizeof(bool));
  for (u = 0; u < nv; u++)
    visited[u] = false;
  
  queue *q = queue_create(nv); // capacité max de la file égale au nombre de sommets

  
  list *lsucc = NULL;
  stack *s = stack_create();

  queue_enqueue(q, source);
  visited[source] = true;
  
  while (queue_is_empty(q) == false)
  {
    u = queue_dequeue(q);
   
    stack_push(s, u);
    
    lsucc = wgraph_succ(g, u);
    
    for (cell *c = list_first_cell(lsucc); c != NULL;  c =  list_next_cell(c))
    {
      list_get_cell_data(c, &v, &w); // w ne sert à rien mais on doit qd meme le récupérer...
      if (visited[v] == false)
      {
	 queue_enqueue(q, v);
	 visited[u] = true; // grace a ceci, chaque sommet ne pourra entrer qu'une fois dans la file
	 // et on est assurés que la taille de la file ne pourra jamais excéder, dans le pire des cas, nv
	 // ce qui nous permet d'utiliser une implémentation ringbuffer, qui est super efficec mais qui requiert une bonne max pour la capacité de file
      }
    }
    
    /*
    c = list_first_cell(lsucc);
    while (c != NULL)
    {
      v = c->key;
      if (visited[v] == false)
	{
	  queue_enqueue(q, v); 
	}

      c = list_next_cell(c);
      }*/
  }

  stack_reverse(s); // on a empilé à l'envers... on fait ceci si on veut visualiser les sommets par ordre de visite
  queue_free(&q);
  free(visited);
  
  return s;
}

