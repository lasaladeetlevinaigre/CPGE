#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define NMAX 100

int piocher_nombre_mystere()
{
  return rand() % (NMAX+1);
}

int main(void)
{
  int essai;
  int n_essais = 0;
  bool trouve = false;

  srand(time(NULL));
  
  int nombre_mystere = piocher_nombre_mystere();

  printf("Quel est le nombre mystere?\n");
  
  while (trouve == false)
  {
    scanf("%d", &essai);
    n_essais = n_essais + 1;
    if (essai == nombre_mystere)
    {
      printf("Vous avez trouvé le nombre mystere en %d essai(s). Bravo!\n", n_essais);
      break;
    }
    else if (essai > nombre_mystere)
      printf("Trop grand!\n");
    else
      printf("Trop petit!\n");
  }

  return 0;
}
