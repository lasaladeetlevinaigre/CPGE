#include <stdbool.h>
#include "mylist.h"

typedef struct wgraph_t wgraph;

// constructeur
wgraph *wgraph_init_no_edge(int nv, bool directed);

// destructeur
void    wgraph_free(wgraph **addr_g);

// accesseurs
list   *wgraph_succ(wgraph *g, int u); // liste des successeurs d'un sommet
void    wgraph_print(wgraph *g);
int     wgraph_number_of_vertices(wgraph *g);
int     wgraph_number_of_edges(wgraph *g);

// transformateurs
bool    wgraph_add_edge(wgraph *g, int u, int v, double weight);  
bool    wgraph_remove_edge(wgraph *g, int u, int v); 

