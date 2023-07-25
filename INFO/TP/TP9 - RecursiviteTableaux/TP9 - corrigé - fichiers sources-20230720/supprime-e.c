#include <stdio.h>
#include <assert.h>


int main(void)
{
  FILE *f_lecture = fopen("exemple.txt", "r");
  assert(f_lecture != NULL);
  FILE *f_ecriture = fopen("exemple-sans-e.txt", "w");
  assert(f_ecriture != NULL);

  char caractere_lu;
  int ne_suppr = 0;
  
  while (feof(f_lecture) == 0)
  {
    fscanf(f_lecture, "%c", &caractere_lu);
    if (caractere_lu != 'e')
      fprintf(f_ecriture, "%c", caractere_lu);
    else
      ne_suppr = ne_suppr + 1;
  }

  printf("Nombre de e supprimés: %d\n", ne_suppr);
  fclose(f_lecture);
  fclose(f_ecriture);

  return 0;
}
