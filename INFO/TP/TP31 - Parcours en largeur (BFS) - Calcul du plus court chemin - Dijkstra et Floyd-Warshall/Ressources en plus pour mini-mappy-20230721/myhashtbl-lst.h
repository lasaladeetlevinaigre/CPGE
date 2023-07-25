#ifndef __MYHASHTBLLISTH__
#define __MYHASHTBLLISTH__

#include <stdbool.h>


#define myhashtbl_elt_type int
#define myhashtbl_elt_fmt  "%d"



typedef struct s_myhashtbl_list myhashtbl_list;


// constructeur
extern myhashtbl_list *myhashtbl_list_create();

// accesseurs
extern int myhashtbl_list_length(myhashtbl_list *l);
extern bool myhashtbl_list_find(myhashtbl_list *l, char *k, myhashtbl_elt_type *v);

// transformateurs
extern void myhashtbl_list_add(myhashtbl_list *l, char *k, myhashtbl_elt_type v);
extern bool myhashtbl_list_remove(myhashtbl_list *l, char *k);
extern void myhashtbl_list_print(myhashtbl_list *l);
extern bool myhashtbl_list_replace(myhashtbl_list *l, char *k, myhashtbl_elt_type v);

// destructeur
extern void myhashtbl_list_free(myhashtbl_list **addr_l);
#endif
