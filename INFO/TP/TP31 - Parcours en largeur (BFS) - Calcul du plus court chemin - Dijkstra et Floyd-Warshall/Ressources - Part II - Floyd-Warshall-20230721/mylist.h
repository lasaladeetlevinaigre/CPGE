#ifndef __MYLISTH__
#define __MYLISTH__

#include <stdbool.h>

#define key_type int
#define key_fmt  "%d"
#define elt_type double
#define elt_fmt  "%lf"



typedef struct s_cell cell;
typedef struct s_list list;


// constructeur
list *list_create();

// accesseurs
int list_length(list *l);
bool list_find(list *l, key_type k, elt_type *addr_v);
bool list_is_empty(list *l);
void list_print(list *l);



// accesseurs/itérateurs
cell *list_first_cell(list *l);
cell *list_next_cell(cell *c);
bool list_get_cell_data(cell *c, key_type *addr_k, elt_type *addr_v);

// transformateurs
void list_add(list *l, key_type k, elt_type v);
bool list_delete(list *l, key_type k);

// destructeur
extern void list_free(list **addr_l);
#endif
