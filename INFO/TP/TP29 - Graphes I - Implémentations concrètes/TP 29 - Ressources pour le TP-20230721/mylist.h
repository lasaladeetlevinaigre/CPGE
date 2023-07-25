#ifndef __MYLISTH__
#define MYLISTH

#include <stdbool.h>

#define key_type int
#define key_fmt  "%d" //"%s"
#define elt_type double
#define elt_fmt  "%lf"

// implémentation d'une structure de données de type liste
// sous forme de maillons (cell) chaînés
struct s_cell
{
  key_type            key;
  elt_type            val; // valeur contenue dans ce maillon
  struct s_cell *addr_next; // pointeur contenant l'adresse
                            // du maillon suivant
};

typedef struct s_cell cell;

// structure encapsulante, avec stockage de la taille courante
struct s_list
{
  int  siz;
  cell *addr_first;
};

typedef struct s_list list;


// constructeur
extern list *list_create();

// accesseurs
extern int list_length(list *l);
extern bool list_find(list *l, key_type k, elt_type *addr_v);
extern bool list_is_empty(list *l);

// transformateurs
extern void list_add(list *l, key_type k, elt_type v);
extern cell *list_first_cell(list *l);
extern cell *list_next_cell(cell *c);
extern bool list_delete(list *l, key_type k);
extern void list_print(list *l);


// destructeur
extern void list_free(list **addr_l);
#endif
