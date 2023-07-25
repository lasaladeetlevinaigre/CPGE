#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

void swap(int *a, int *b)
{
  int buf; // variable tampon (buffer en anglais)

  buf = *a; 
  *a = *b;
  *b = buf;

  return;
}

int *genere_tab_aleatoire(int n, int a, int b)
{
  int i;
  int *tab = NULL;
  double d;
  
  assert(n > 0); // prog défensive

  tab = malloc(n*sizeof(int));

  assert(tab != NULL); // prog défensive: on vérifie que l'allocation s'est bien passée

  // résilience si a > b: on les échange!
  if (a > b)
    swap(&a, &b);


  for (i = 0; i < n; i++)
  {
    d = (double)(  rand()  / (double)RAND_MAX )*(b-a+1);

    tab[i] = a + (int)d;
  }
  
  return tab;
}


void affiche_tableau(int *t, int taille, int max)
{
  unsigned int i;

  int lim = taille; // lim permet de ne pas afficher trop de choses. 

  if (lim > max)
    lim = max;
  for (i = 0; i < lim; i++) // on parcourt toutes les cases du tableau t jusqqu'à lim
    printf("%d ", t[i]); // on affiche la valeur entière  contenue dans chaque case
  printf("\n"); // on oublie pas de mettre en retour à la ligne
  return;
}



void tri_insertion(int *tab, int n)
{

  assert(tab != NULL);
  assert(n > 0);
    
  int i, j;
  int v;
  
  for (i = 1; i < n; i++) // boucle sur les éléments que l'on va insérer un à un
  {
    v = tab[i]; // sauvegarde la valeur à insérer
    j = i;
    
    while (j >= 1 && tab[j-1] > v) // décalage à droite jusqu'à ce que la bonne place soit libre
    {
      tab[j] = tab[j-1];
      j = j - 1;
    }

    tab[j] = v; // on place la valeur à insérer 
  }

  return;
}


int main(int argc, char **argv)
{

  assert(argc == 2);
  // initialisation de l'algorithme de génération
  // de nombre pseudo-aléatoires avec comme germe
  // le nombre de secondes écoulées depuis le 01/01/1970
  srand(time(NULL));

  int *tab = NULL;
  int n = atoi(argv[1]);

  tab = genere_tab_aleatoire(n, 0, 1000000); // allocation d'un tableau aléatoire
  
  // test tri à bulles
  //affiche_tableau(tab, n, 20); // on commente les affichages
  
  tri_insertion(tab, n); // appel de l'algorithme de tri par insertion
 
  //affiche_tableau(tab, n, 20);

  free(tab); // libération de l'espace alloué sur le tas pour le tableau
  
  return 0;
}
