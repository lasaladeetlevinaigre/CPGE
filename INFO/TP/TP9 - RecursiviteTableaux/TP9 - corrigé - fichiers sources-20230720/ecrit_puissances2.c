#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char **argv)
{
  assert(argc == 3);

  int   n              = atoi(argv[1]);
  char *chemin_fichier = argv[2];
  
  FILE *f = fopen(chemin_fichier, "w");
  assert(f != NULL);

  int p = 1;
  int i;
  for (i = 0; i <= n; i++)
  {
    fprintf(f, "2 puissance %d = %d\n", i, p);
    p = 2*p;
  }

  fclose(f);
  return 0;
}
