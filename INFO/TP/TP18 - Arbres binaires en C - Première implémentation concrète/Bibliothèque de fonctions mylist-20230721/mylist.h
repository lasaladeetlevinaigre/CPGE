#ifndef __MYLISTH__
#define MYLISTH

#define elt_type int
#define elt_fmt  "%d "

// implémentation d'une structure de données de type liste
// sous forme de maillons (cell) chaînés
struct s_cell
{
  elt_type            val; // valeur contenue dans ce maillon
  struct s_cell *addr_next; // pointeur contenant l'adresse
                            // du maillon suivant
};

typedef struct s_cell cell;

// structure encapsulante, avec stockage de la taille courante
struct s_list
{
  int  siz;
  cell         *addr_first;
};

typedef struct s_list list;


// constructeur
extern list *create_list();

// accesseurs
extern int get_length(list *l);
extern int length(list *l);
extern int get_ith(unsigned int i, list *l);


// transformateurs
extern void push(elt_type v, list *l);
extern void print_list(list *l);
extern void delete_ith(unsigned int i, list *l);
extern void insert_ith(unsigned int i, elt_type v, list *l);
extern list *concatenate(list *l1, list *l2);

// destructeur
extern void free_list(list **addr_l);
#endif
