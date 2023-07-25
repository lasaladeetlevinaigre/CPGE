#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

//#define elt_type float
//#define elt_fmt  "%f"

#define elt_type double
#define elt_fmt  "%lf"

//#define elt_type int
//#define elt_fmt  "%d"

//#define elt_type char
//#define elt_fmt  "%c"

//#define elt_type bool
//#define elt_fmt  "%d"

struct s_cell
{
  elt_type       val; // valeur contenue dans ce maillon
  struct s_cell *addr_next; // pointeur contenant l'adresse
                            // du maillon suivant
};

typedef struct s_cell cell;

struct s_queue
{
  unsigned int  siz;
  cell         *addr_peek; // pointeur vers le premier maillon (tête de file)
  cell         *addr_last; // pointeur vers le dernier maillon pour enqueue
};

typedef struct s_queue queue;


queue *queue_create()
{
  queue *q = NULL;
  q = malloc(sizeof(queue));
  assert(q != NULL);
  q->siz = 0;
  q->addr_peek = NULL;
  q->addr_last = NULL;
  return q;
}




unsigned int queue_length(queue *q)
{
  assert(q != NULL);
  return q->siz;
}

bool queue_is_empty(queue *q)
{
  assert(q != NULL);
  if (q->siz == 0)
    return true;
  return false;
}

elt_type queue_peek(queue *q)
{
  assert(q != NULL);

  cell *c = q->addr_peek;
  assert(c != NULL);
  
  return c->val;
}

elt_type queue_dequeue(queue *q)
{
  assert(q != NULL);

  cell *c = q->addr_peek;
  assert(c != NULL);

  int v = c->val;
  q->addr_peek = c->addr_next;

  free(c);
  q->siz = q->siz - 1;
  return v;
}

void queue_enqueue(elt_type v, queue *q)
{
  assert(q != NULL);
  
  cell *c_last   = q->addr_last;
  cell *c_new = NULL;

  
  if (c_last == NULL)
  {
    c_new = malloc(sizeof(cell));
    c_new->val = v;
    c_new->addr_next = NULL;
    q->addr_peek = c_new;
    q->addr_last = c_new;
    q->siz = q->siz + 1;
    return;
  }
  

  /*
  // cette boucle devient inutile grace au stockage de addr_last
  for (int j = 0; j < (int)(q->siz-1); j++)
  {
    assert(c != NULL);
    c = c->addr_next;
  }

  assert(c != NULL);*/
  
  c_new = malloc(sizeof(cell));
  c_new->val = v;
  c_new->addr_next = NULL;
  c_last->addr_next = c_new;

  q->addr_last = c_new;

  q->siz = q->siz + 1;

  return;
  
}


// le passage par adresse est nécessaire pour être certain
// de bien remettre l'adresse de la queuee NULL
void queue_free(queue **addr_q)
{
  queue *q = *addr_q;
  cell  *c = q->addr_peek;
  cell  *tmp = NULL;
  
  while (c != NULL)
  {
    tmp = c;
    c = c->addr_next;
    free(tmp);
  }
  free(q);
  *addr_q = NULL;

  return;
}

void queue_print(queue *q)
{
  cell *c = q->addr_peek;

  printf("\n%d elements: ", q->siz);
  while (c != NULL)
  {
    printf(elt_fmt, c->val);
    printf(" ");
    c = c->addr_next;
  }
  printf("\n");

  return;
}

int main(void)
{
  queue *q = queue_create();

  queue_enqueue(2, q);
  queue_enqueue(3.7, q);
  
  queue_print(q);
  queue_enqueue(4.1, q);

  printf(elt_fmt, queue_dequeue(q));
  queue_print(q);

  printf("peek=%f\n", queue_peek(q));
  queue_enqueue(5.35, q);

  queue_print(q);
  queue_dequeue(q);
  queue_print(q);
  queue_free(&q);
  
  return 0;

}
