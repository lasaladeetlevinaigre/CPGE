#ifndef __MYQUEUEH__
#define __MYQUEUEH__

#include <stdbool.h>


#define queue_elt_type int
#define queue_elt_fmt "%d"

struct s_queue
{
  int idx_peek; 
  int siz; // taille courante de la queue
  int siz_max; // taille maximale possible
  queue_elt_type *tab; // tableau contenant les valeurs (séquentielles) de la queuee
};

typedef struct s_queue queue;




// constructeur
queue *queue_create(int capacity);

// accesseurs
unsigned int length(queue *q);
bool queue_is_empty(queue *q);
queue_elt_type queue_peek(queue *q);
void queue_print(queue *q);

// transformateurs
queue_elt_type queue_dequeue(queue *q);
void queue_enqueue(queue *q, queue_elt_type v);

// destructeur
void queue_free(queue **addr_q);

#endif
