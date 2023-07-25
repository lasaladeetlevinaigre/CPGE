#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <time.h>


/********************************************************/
/*************** UTILITAIRES         ********************/
/********************************************************/
void swap(int *a, int *b)
{
  int buf; // variable tampon (buffer en anglais)

  buf = *a; 
  *a = *b;
  *b = buf;

  return;
}

void print_tab(int *t, int n, int max)
{
  unsigned int i;

  int lim = n; // lim permet de ne pas afficher trop de choses. 

  if (lim > max)
    lim = max;
  for (i = 0; i < lim; i++) // on parcourt toutes les cases du tableau t jusqqu'à lim
    printf("%d ", t[i]); // on affiche la valeur entière  contenue dans chaque case
  printf("\n"); // on oublie pas de mettre en retour à la ligne
  return;
}


int *generate_random_array(int n, int a, int b)
{
    // Résilience: si a > b: on les échange!
  if (a > b)
    swap(&a, &b);
  
  int *tab = malloc(n*sizeof(int));
  assert(tab != NULL);
  
  for (int i = 0; i < n; i++)
    tab[i] = a + rand()%(b-a+1);

  return tab;
}

/********************************************************/
/***************  QUICKSORT           *******************/
/********************************************************/

// modification pour renvoie du nombre de comparaisons effectuées
// on passe par adresse pour l'utiliser comme accumulateur
// au fil des appels récursifs
void quicksortrec(int *tab, int l, int r_strict, int *addr_n_comp)
{
  int lo, hi; // lo = indice max zone gauche <; hi = indice min zone droite >
  int p; // valeur du pivot
  int v; // valeur courante
  int i;

  if (r_strict - l <= 1) // cas de base
    return;

  p = tab[l];

  i = l + 1;
  lo = l;
  hi = r_strict;
  
  while (i < hi)
  {
    v = tab[i];

    *addr_n_comp = *addr_n_comp + 1;
    if (v == p)
    {
      //lo = lo + 1; // facultatif?
      i = i + 1;
    }
    else if (v < p)
    {
      swap(&(tab[lo]), &(tab[i]));
      lo = lo + 1;
      i = i + 1;
    }
    else
    {
      hi = hi - 1;
      swap(&(tab[i]), &(tab[hi]));    
      // i reste identique
    }
  }

  quicksortrec(tab, l, lo, addr_n_comp);
  quicksortrec(tab, hi, r_strict, addr_n_comp);

  return;
}


int quicksort(int *tab, int n)
{
  int n_comp = 0; // nombre comparaison, accumulateur transmis a chaque appel récursif
  quicksortrec(tab, 0, n, &n_comp);
  return n_comp;
}


/********************************************************/
/***************  INSERTIONSORT           *******************/
/********************************************************/

// on fait en sorte qu'elle aussi renvoie le nombre de comparaisons
// pour qu'elle ait le même prototype que quicksort
int insertion_sort(int *tab, int n)
{

  assert(tab != NULL);
  assert(n > 0);
    
  int i, j;
  int v;
  int ncomp = 0;
  
  for (i = 1; i < n; i++)
  {
    v = tab[i];
    j = i;
    
    while (j >= 1 && tab[j-1] > v)
    {
      ncomp = ncomp + 1;
      tab[j] = tab[j-1];
      j = j - 1;
    }
    ncomp = ncomp + 1; // le test qui a mis fin au while doit etre compté
    tab[j] = v;
  }

  return ncomp;
}


/*
int main(int argc, char **argv)
{

  assert(argc == 2);
  srand(time(NULL));
  
  int n = atoi(argv[1]);

  if (n <= 0)
  {
    fprintf(stderr, "Erreur: l'entier fourni (%d) est négatif ou nul\n", n);
    return 1;
  }


  int *tab = generate_random_array(n, 0, n*100);
  print_tab(tab, n, 100);
  quicksort(tab, n);
  print_tab(tab, n, 100);
  
  free(tab);
  
  return 0;
  }*/
