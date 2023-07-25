#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

// constante globale allouée dans le segment statique
const int NMAX_CHAMPS = 5;

enum mois {Aucun, Janvier, Fevrier, Mars, Avril, Mai, Juin, Juillet, Aout, Septembre, Octobre, Novembre, Decembre};

// tableau donnant une chaine associee a chaque enum, pour les affichages surtout 
char *mois_to_string[13] = { "Aucun", "Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"};

enum genre {Fille, Garcon, NonBinaire};

char *genre_to_string[3] = { "Fille", "Garcon", "Non-Binaire"};

void affecter_genre(char *champ, enum genre *pgenre)
{
  if (champ[0] == 'F')
    *pgenre = Fille;
  else if (champ[0] == 'G')
    *pgenre = Garcon;
  else
    *pgenre = NonBinaire;

  return;
}

enum regime {DemiPensionnaire, Externe, Interne, InterneExterne, DemiPensionnaireAuTicket};

char *regime_to_string[5] = { "Demi-Pensionnaire", "Externe", "Interne", "Interne-Externe", "Demi-Pensionnaire au ticket"};

void affecter_regime(char *champ, enum regime *pregime)
{
    
  if (strcmp(champ, "DEMI-PENSIONNAIRE") == 0)
    *pregime = DemiPensionnaireAuTicket;  
  else if (strcmp(champ, "DEMI-PENSIONNAIRE-AU-TICKET") == 0)
    *pregime = DemiPensionnaire;
  else if (strcmp(champ, "INTERNE") == 0)
    *pregime = Interne;
  else if (strcmp(champ, "INTERNE-EXTERNE") == 0)
    *pregime = InterneExterne;
  else
    *pregime = Externe;
  
  return;
}


struct s_eleve
{
  char nom[256];
  char prenom[256];
  short int jour;
  enum mois mois;
  int annee;
  enum genre genre;
  enum regime regime;
  
};

typedef struct s_eleve eleve;


int decouper_chaine(char *chaine, char sep, char *champs[NMAX_CHAMPS])
{
  assert(chaine != NULL);

  int i; // indice du caracteres dans la chaine
  int idx; // indice du champ
  char c;
  
  i= 0;
  idx = 0;
  champs[idx] = &(chaine[i]);
  c = chaine[i];
  while (c != '\0' && idx < NMAX_CHAMPS)
  {
    if (c == sep)
    {
      chaine[i] = '\0';
      idx ++;
      champs[idx] = &(chaine[i+1]);
    }
    i = i + 1;
    c = chaine[i];
  }

  return idx+1;
}

void decouper_date(char *date, short int *pjour, enum mois *pmois, int *pannee)
{
  char *champs[NMAX_CHAMPS]; // OK car une date a 3 champs et 3 < NMAX_CHAMPS
  int n_champs = decouper_chaine(date, '/', champs);

  assert(n_champs == 3); // on verifie qu'on a bien decope la chaine de caractere jj/mm/aaaa en 3 champs
  
  *pjour  = atoi(champs[0]);
  *pmois  = atoi(champs[1]);
  *pannee = atoi(champs[2]);

  return;
}


int lire_fichier(char *chemin_fichier, eleve **ptab)
{
  char *champs[NMAX_CHAMPS];
  char  ligne[1024];
  FILE *f = NULL;
  int n_eleves;
  int idx_eleve;
  int n_champs;
  eleve *e = NULL;
  
  // 1ere etape: comptage du nombre de lignes du fichier
  f = fopen(chemin_fichier, "r");
  assert(f != NULL);

  
  n_eleves = 0;
  while (feof(f) == false)
  {
    fscanf(f, "%s", ligne);
    n_eleves = n_eleves + 1;
  }
  fclose(f);

  // allocation du grand tableau
  *ptab = malloc(n_eleves*sizeof(eleve));
  assert(*ptab != NULL);

  // remplissage du tableau: une ligne donne les infos pour une case du tableau
  f = fopen(chemin_fichier, "r");
  assert(f != NULL);
  
  idx_eleve = 0;
  while (feof(f) == false)
  {
    fscanf(f, "%s", ligne);
    n_champs = decouper_chaine(ligne, ',', champs);

    if (n_champs != 5)
      continue;
    
    e = &( (*ptab)[idx_eleve] );
    
    strcpy(e->nom, champs[0]);
    strcpy(e->prenom, champs[1]);
    decouper_date(champs[2], &(e->jour), &(e->mois), &(e->annee));

    affecter_genre(champs[3], &(e->genre));
    affecter_regime(champs[4], &(e->regime));
    
    idx_eleve  = idx_eleve + 1;
  }
  
  fclose(f);
  
  return idx_eleve;
}

void afficher_tab_eleves(eleve *tab_eleves, int n_eleves)
{
  eleve e;
  
  for (unsigned int i = 0; i < n_eleves; i++)
  {
    e  = tab_eleves[i];
    printf("\nEleve %d\n", i);
    printf("\t%s\n", e.nom);
    printf("\t%s\n", e.prenom);
    printf("\t%d %s %d\n", e.jour, mois_to_string[e.mois], e.annee); // mois est un enum, donc un entier, il peut servir d'indice
    printf("\t%s\n", genre_to_string[e.genre]);
    printf("\t%s\n", regime_to_string[e.regime]);
  }
}

void anniversaire(eleve *tab, int n, enum mois mois_anniv)
{
  eleve *e = NULL;

  printf("Liste des élèves nés en %s:\n", mois_to_string[mois_anniv]);
  for (unsigned int i = 0; i < n; i++)
  {
    e = &(tab[i]);
    if (e->mois == mois_anniv)
      printf("%s %s %d\n", e->nom, e->prenom, e->jour);
  }

  return;
}

int main(int argc, char **argv)
{
  assert(argc == 3);

  char *path = argv[1]; // chemin du fichier contenant les données

  int m = atoi(argv[2]); // numéro mois de l'annee (1-12) pour recherche anniversaires

  assert(m >= 1 && m <= 12);
  enum mois mois_anniv = m;

  eleve *tab_eleves  = NULL;

  int n_eleves = lire_fichier(path, &tab_eleves);

  //afficher_tab_eleves(tab_eleves, n_eleves);

  anniversaire(tab_eleves, n_eleves, mois_anniv);
  
  if (tab_eleves != NULL)
    free(tab_eleves);

  tab_eleves = NULL;
		 
  return 0;
}
