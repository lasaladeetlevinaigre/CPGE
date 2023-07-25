#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "myqueue.h"

queue *queue_create(int s_max)
{
  queue *q = malloc(sizeof(queue));
  q->siz = 0;
  q->idx_peek = 0;
  q->siz_max = s_max;
  q->tab = malloc(s_max*sizeof(queue_elt_type));

  return q;
}

int queue_length(queue *q)
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

void queue_enqueue(queue *q, queue_elt_type v)
{
  assert(q != NULL);
  assert(q->siz < q->siz_max);
  
  int idx_back  = (q->idx_peek + q->siz)%q->siz_max;

  q->tab[idx_back] = v;
  q->siz = q->siz+1;

  return;
}

queue_elt_type queue_peek(queue *q)
{
  assert(q != NULL);
  assert(q->siz > 0);
  
  return q->tab[q->idx_peek];
}

queue_elt_type queue_dequeue(queue *q)
{
  assert(q->siz > 0);
  
  queue_elt_type v = q->tab[q->idx_peek];
  
  q->idx_peek = (q->idx_peek + 1)%q->siz_max;
  q->siz = q->siz-1;
  
  return v;
}

void queue_print(queue *q)
{
  assert(q != NULL);
  int idx;
  printf("Longueur %d: ", queue_length(q));
  for (unsigned int i = 0; i < q->siz; i++)
  {
    idx = (q->idx_peek + i)%q->siz_max;
    printf(queue_elt_fmt, q->tab[idx]);
    printf(" ");
  }
  printf("\n");
}

int *queue_get_array(queue *q, int *addr_start, int *addr_end)
{
  assert(q != NULL);
  assert(addr_start != NULL); // passage par adresse
  assert(addr_end != NULL); // passage par adresse
  
  *addr_start = q->idx_peek;
  *addr_end = q->idx_peek + q->siz;
  return q->tab;
}


void queue_delete(queue *q)
{
  assert(q != NULL);
  q->idx_peek = 0;
  q->siz = 0;
}

void queue_free(queue **addr_q)
{
  assert(addr_q != NULL);
  queue *q = *addr_q;
  if (q->tab != NULL)
    free(q->tab);
  free(q);
  *addr_q = NULL;
}



queue *concatenate(queue *q1, queue *q2)
{
  assert(q1 != NULL);
  if (q2 == NULL)
    return q1;
  
  assert(q1->siz + q2->siz < q1->siz_max);
  
  for (unsigned int j = 0; j < q2->siz; j++)
    q1->tab[(q1->idx_peek+q1->siz+j)%q1->siz_max] = q2->tab[j];
  
  q1->siz = q1->siz + q2->siz;
  return q1;
}

/*int main(void)
{
  queue *q = queue_create(100);

  queue_enqueue(q, 2);
  queue_enqueue(q, 1);
  queue_enqueue(q, -8);
  queue_print(q);
  printf("pop=%d\n", queue_dequeue(q));
  queue_print(q);
  queue_enqueue(q, 0);
  queue_enqueue(q, 4);
  queue_print(q);
  printf("pop=%d\n", queue_dequeue(q));
  queue_print(q);
  printf("peek=%d\n", queue_peek(q));
  queue_print(q);
  queue_dequeue(q);
  queue_print(q);
  queue_dequeue(q);
  queue_print(q);
  queue_dequeue(q);
  queue_print(q);
  queue_dequeue(q);
  queue_print(q);
  queue_free(&q);
  
  return 0;
  }*/
