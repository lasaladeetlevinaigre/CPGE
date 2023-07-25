#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

int compte_lignes(char *chemin_fichier)
{
  FILE *f = fopen(chemin_fichier, "r"); // on ouvre le fichier et on récupère
  //le descripteur de fichier permettant, clé d'accès au canal.

  assert(f != NULL); // prog defensive: on verifie que l'ouverture du canal
                     //vers le fichier en lecture a été effectuée avec succès

  int nb_lignes = 0; // compteur de lignes initialisé à 0
  char caractere_lu; // variable qui contiendra les caractères lus successivemement
  
  while( feof(f) == 0 ) // tant que le caractère EOF de fin de fichier n'a pas été lu
  {
    fscanf(f, "%c", &caractere_lu); // lire un caractère et stocker sa valeur
                                    //dans la variable caractere_lu
    
    if (feof(f) == 1) // si on vient de lire le caractère de fin de fichier.
      break; // on casse la boucle
    
    if (caractere_lu == '\n') // si on a lu un retour à la ligne
      nb_lignes = nb_lignes + 1; // on compte une ligne de plus
  }

  fclose(f); // on n'oublie pas de refermer le canal de lecture depuis le fichier

  return (nb_lignes); // on retourne le nombre de lignes comptées
}

int compte_mots(char *chemin_fichier)
{
  FILE *f = fopen(chemin_fichier, "r");

  assert(f != NULL);

  int nb_mots = 0;
  char caractere_lu;
  bool deb = true;

  
  while (feof(f) == 0)
  {
    fscanf(f, "%c", &caractere_lu);
     
    // on avance jusqu'à trouver un caractère non blanc ou jusqu'à la fin du fichier
    while (feof(f) == 0 && (caractere_lu == ' ' || caractere_lu == '\n'))
       fscanf(f, "%c", &caractere_lu);
    
    if (feof(f)) // si on a atteint la fin du fichier, on casse la boucle et on sort
      break;

    
    // sinon, si c'est la premiere fois qu'on retombe sur un caractere non blanc,
    //c'est le debut d'un mot
    if (deb == true)
    {
      // on incrémente le compteur de mots
      nb_mots = nb_mots + 1;
      deb = false;
    }

    // on lit la suite du mot et on arrête au prochain caractere blanc
    while (feof(f) == 0 && (caractere_lu != ' ' && caractere_lu != '\n'))
       fscanf(f, "%c", &caractere_lu);

    deb = true; // on se prépare pour la lecture du début d'un nouveau mot
    
  }

  // on n'oublie pas de fermer le canal de communication avec le fichier
  fclose(f);

  return nb_mots;
}


int main(int argc, char **argv)
{

  assert(argc == 2); // prog defensive: on vérifie que l'utilisateur a bien
  // donné un nom de fichier sur la ligne de commande
  
  char *chemin_fichier = argv[1];

  int nl = compte_lignes(chemin_fichier);
  int nm = compte_mots(chemin_fichier);

  printf("Le fichier %s possède:\n %d lignes\n %d mots\n", chemin_fichier, nl, nm);

  return 0;
    
}
