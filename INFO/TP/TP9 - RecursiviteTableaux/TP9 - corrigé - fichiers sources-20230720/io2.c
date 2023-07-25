#include <stdio.h>
// Fichier exemple pour montrer l'utilisation des fonctions de la bibliothèque stdio
// Ici, on utilise les fonctions de lecture séquentielle d'un fichier
int main(void)
{
  // On ouvre un canal en lecture depuis le fichier exemple.txt
  FILE *f = fopen("exemple.txt", "r");
  if (f == NULL)
  {
    // si la fonction fopen renvoie un descripteur de fichier NULL
    // il y a eu un problème,
    //souvent c'est que le fichier n'existe pas chemin d'accès
    //qui consistue le nom du fichier n'est pas valide
    printf("Erreur lors de l'ouverture du fichier ./exemple.txt en lecture\n");
    return 1;
  }

  char caractere_lu;
  int compteur = 0;
  
  while (feof(f) == 0 )
  {
    // pour lire, on utilise un chaine format et on donne l'adresse
    // des variables dans lesquelles il faut stocker les valeurs lues
    // et formatées. Ici, on lit un char (%c) et on stocke le résultat
    // dans la variabel caractere_lu qui est bien de type char,
    //et dont on a passé l'adresse à la fonction fscanf
    fscanf(f, "%c", &caractere_lu);
   
    printf("Je viens de lire le caractère [%c]\n", caractere_lu);
    compteur = compteur + 1;
  }
  // On enlève 1 car emacs rajoute une ligne vide en fin de fichier
  printf("J'ai lu au total %d caractères\n", compteur-1);

  // On n'oublie pas de refermer le canal!
  fclose(f);

  return 0;
}
