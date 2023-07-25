#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define elt_type int
#define elt_fmt "%d "

struct heap
{
  bool is_tab_allocated_outside; // ce champ permet d'indiquer si le tableau a été alloué par heap_create ou bien
  // si le tableau a été alloué à l'extérieur et fourni en entrée de heap_create_from_tab
  int size;
  int capacity;
  elt_type *tab;
  
};

typedef struct heap heap;
typedef struct heap pqueue;


// exercice 2: creation d'un tas et insertion dans un tas

// On donne en entrée de la fonction la capacité maximale du tableau qui servira à stocker les étiquettes du tas
heap *heap_create(int capacity)
{
  heap *t = malloc(sizeof(heap));

  t->is_tab_allocated_outside = false;
  
  t->capacity = capacity;
  t->size = 0;
  t->tab = calloc(capacity, sizeof(elt_type));
  
  return t;
}

// libération de l'espace alloué dans le tas
void heap_free(heap **addr_t)
{
  assert(addr_t != NULL);
  heap *t = *addr_t;

  if (t != NULL)
  {
    if (t->tab != NULL && t->is_tab_allocated_outside == false)
      free(t->tab);
    free(t);
  }
  *addr_t = NULL;
  return;
}

void print_tab(elt_type *tab, int n)
{
  for (int i = 0; i < n; i++)
    printf(elt_fmt, tab[i]);
  printf("\n");
}

// affichage dans l'ordre de numérotation naturel des arbres binaires complets
void heap_print(heap *t)
{
  printf("Tas (stockage tableau):\n\t");
  print_tab(t->tab, t->size);
  return;
}

void aux_heap_print_infixe(heap *t, int l)
{
  if (l >= t->size)
    return;
  aux_heap_print_infixe(t, 2*l+1);
  printf("%d ", t->tab[l]);
  aux_heap_print_infixe(t, 2*l+2);
  return;
}
void heap_print_infixe(heap *t)
{
  printf("Tas (parcours infixe):\n\t");
  aux_heap_print_infixe(t, 0);printf("\n");
  return;
}

bool heap_is_full(heap *t)
{
  return (t->size >= t->capacity);
}

void move_up(heap *t, int i, elt_type v, bool reverse_order) // reverse_order: cf exo heap_sort
{

  assert(i >= 0);
  
  int j;

  while (i > 0)
  {
   
    j = (i-1)/2; // indice du père

    if (    (reverse_order == true && t->tab[j] < v) // si le père et le fils sont rangés dans le bon ordre, on stope la remontée
	 || (reverse_order == false && t->tab[j] > v) )
      break;
    
    t->tab[i] = t->tab[j]; // descente fils <-> du père dans le fils
    i = j;
  }

  t->tab[i] = v; // on place la valeur dans le noeud où l'on s'est arrêté
  
  return;
}

void heap_insert(heap *t, elt_type v, bool reverse_order)
{
  if (heap_is_full(t) == true)
  {
    fprintf(stderr, "Impossible d'inserer: le tas a atteint sa capacite maximale\n");
    return;
  }
  
  move_up(t, t->size, v, reverse_order); // on insère v en bas à droite, dans la case d'indice t->size, et on le fait remonter jusqu'à rétablir la propriété de tas

  t->size = t->size + 1; // mise à jour du nombre de noeuds du tas
 
  return;
}


heap *heap_create_from_tab(elt_type *tab, int n, bool reverse_order)
{
  assert(tab != NULL);
 
  heap *t = malloc(sizeof(heap));

  t->is_tab_allocated_outside = true;
  t->capacity = n;
  t->size = 0;
  t->tab = tab; // on copie simplement l'adresse du tas qui a déjà été alloué à l'extérieur

  elt_type v;
  // réinsertion des éléments du tableau dans le tas
  for (int i = 0; i < n; i++)
  {
    v =  t->tab[i];
    heap_insert(t, v, reverse_order);
  }

  heap_print(t);
  return t;
}

void test_insert_ex2()
{
  printf("\n\n*** TEST ENONCE EX2 ****\n");
  int tab[8] = {8, 7, 2, 10, 1, 5, 3, 4};
  
  heap *t = heap_create_from_tab(tab, 8, false);

  heap_print(t);
  heap_print_infixe(t);
  heap_free(&t);
}


// exercice 3: suppression dans un tas, tamisage

// cette opération est parfois appelée "tamisage" en français
elt_type move_down(heap *t, int i, elt_type v, bool reverse_order)
{

  //printf("Tamisage de %d\n", v);

  elt_type sav_vi = t->tab[i];
  
  int j = 2*i+1;
  
  while (j < t->size)
  {
    if (j+1 < t->size && ( (reverse_order == true && t->tab[j+1] < t->tab[j]) || (reverse_order == false && t->tab[j+1] > t->tab[j])))
      j = j + 1;

    if ( (reverse_order == true && v < t->tab[j]) || ( reverse_order == false && v > t->tab[j] ) ) // i père, j fils
      break;
    
 
    t->tab[i] = t->tab[j];

    i = j;
    
    j = 2*i + 1;
  }
  
  t->tab[i] = v;
  
 
  return sav_vi;
}


elt_type heap_remove(heap *t, int i, bool reverse_order)
{
  assert(i < t->size);

  // on réinsere le dernier élément à la place de l'élément d'indice i, qui est renvoyé par la fonction
  elt_type last = t->tab[t->size-1];
  t->size = t->size-1;
  return move_down(t, i, last, reverse_order);
}

void test_remove_ex3()
{
  printf("\n\n*** TEST ENONCE EX3 ****\n");
  int tab[8] = {8, 7, 2, 10, 1, 5, 3, 4};
  
  heap *t = heap_create_from_tab(tab, 8, false);

  printf("BEFORE:\n\t"); heap_print_infixe(t);

  // on retire la racine
  heap_remove(t, 0, false);

  printf("AFTER:\n\t"); heap_print_infixe(t);
  
  heap_free(&t);
}

// exercice 4: files de priorité implémentées avec des tas!
void pqueue_enqueue(pqueue *q, elt_type v, bool reverse_order)
{
  heap_insert(q, v, reverse_order);
  return;
}


// la fonction renvoie l'élémént qui se trouvait à la racine du tas
// donc l'élément de priorité max
elt_type pqueue_dequeue(pqueue *q, bool reverse_order)
{
  return heap_remove(q, 0, reverse_order);
}

// exercice 5: heapsort
void heap_sort(elt_type *tab, int n, bool reverse_order)
{
  heap *t = heap_create_from_tab(tab, n, reverse_order); // pas de duplication du tableau... le tri se fait donc en place!

 
  // échange du plus petit et du dernier (mise hors-jeu) puis tamisage
  // pour faire remonter le 2eme plus petit

  elt_type v;
  for (int i = 0; i < n; i++)
  {
    v = pqueue_dequeue(t, reverse_order);
    t->tab[t->size] = v; // on stocke l'élément retiré à la fin du tas, zone qui ne sera plus modifiée par t->size a été mis à jour
  }
  

  // libération du tas: le tableau n'est pas désalloué car il a été alloué en dehors de heap_create
  heap_free(&t);
  
  return;
}



void test_heapsort()
{
  printf("\n\n*** TEST HEAPSORT ****\n");
  int tab[13] = {5, 3, 7, 1, 8, 9, 2, 4, 6, 0, 11, 23, -4};

  heap_sort(tab, 13, false);

  printf("Tableau trié:\n\t");
  print_tab(tab, 13);
}


int *generate_random_array(int n, int a, int b)
{
  int *tab = malloc(n*sizeof(n));
  for (int i = 0; i < n; i++)
    tab[i] = a + rand()%(b-a+1);

  return tab;
}

void test_heapsort_random()
{
   printf("\n\n*** TEST HEAPSORT RANDOM ****\n");
  int n1 = 100;
  int *test1 = generate_random_array(n1, 0, 1000);

  print_tab(test1, n1);
  heap_sort(test1, n1, false);

  printf("Tableau trié:\n\t");
  print_tab(test1, n1);

  free(test1);
}



int main()
{
  srand(time(NULL));

  // exercice 2
  test_insert_ex2();

  // exercice 3
  test_remove_ex3();
  
  // exercice 5
  test_heapsort();
  test_heapsort_random();
  
  return 0;
}
