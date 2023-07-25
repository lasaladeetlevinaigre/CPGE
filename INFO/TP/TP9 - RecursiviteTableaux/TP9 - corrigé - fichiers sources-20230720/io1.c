#include <stdio.h>
#include <assert.h>

int main(int argc, char **argv)
{
  assert(argc == 2);

  char *phrase = argv[1];

  // ouverture d'un canal vers le fichier de chemin "./toto.txt"
  FILE *f = fopen("toto.txt", "w");
  if (f == NULL)
  {
    printf("Erreur lors de l'ouverture du fichier toto.txt en écriture\n");
    return 1;
  }

  // ecriture dans le fichier de descripteur f
  fprintf(f, "J'ai écrit cette phrase dans le terminal: %s\n", phrase);

  // on n'oublie pas de refermer le canal 
  fclose(f);

  return 0;
}
