#ifndef __MYSTACKH__
#define __MYSTACKH__

#include <stdbool.h>

#define vtype int
#define fmt  "%d "

// implémentation d'une structure de données de type liste
// sous forme de maillons (cell) chaînés
struct stack_cell
{
  vtype            key; // valeur contenue dans ce maillon
  struct stack_cell *addr_next; // pointeur contenant l'adresse
                            // du maillon suivant
};
typedef struct stack_cell stack_cell;


// structure encapsulante, avec stockage de la taille courante
struct stack
{
  int  siz;
  stack_cell *addr_first;
};

typedef struct stack stack;


// constructeur
extern stack *stack_create();

// accesseurs
extern int stack_length(stack *s);
extern vtype stack_top(stack *s);
extern bool stack_is_empty(stack *s);
extern void stack_print(stack *s);

// transformateurs
extern void stack_push(stack *s, vtype v);
extern vtype stack_pop(stack *s);
extern void stack_reverse(stack *s);


// destructeur
extern void stack_free(stack **addr_s);
#endif
