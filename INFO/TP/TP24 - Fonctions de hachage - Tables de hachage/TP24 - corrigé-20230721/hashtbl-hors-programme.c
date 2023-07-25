#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>
#include "mylist.h"

#define B 256


struct hashtbl
{
  unsigned int n_buckets; // limite pour la taille de la table: au max 2^{32}-1 = environ 4 milliards de "tiroirs"... en dessous de 10 milliards d'entrées à ranger dans la table, ca va!

  int (*hash_function)(unsigned int, char *);
  list **tab;
};

typedef struct hashtbl hashtbl;

enum hash_fun_type {HashFunNone, HashFunNaive, HashFunSommative, HashFunConversionBase};

int hash_function_pb(int b, unsigned int n_buckets, char *k)
{
  assert( k!= NULL);
  
  int sum;
  int n = strlen(k);
  
  sum = 0;
  for (int i = 1; i <= n; i++) // evaluation du polynome en x=b par la méthode de Horner (linéaire en le degré du polynome
    sum = k[n-i] + sum*b;

  
  return sum % n_buckets; //(sum & INT_MAX)%n_buckets;
}

int hash_fun_naive(unsigned int n_buckets, char *k)
{
  assert( k != NULL);
  
  uint8_t code_ascii = 0; // cas chaine vide géré
  if (strlen(k) > 0)
    code_ascii = k[0]; // code ascii du 1er caractere comme empreinte...
    
  return code_ascii % n_buckets;
}

int hash_fun_sommative(unsigned int n_buckets, char *k)
{
  assert( k != NULL);

  int sum = 0;
  for (unsigned int i = 0; i < strlen(k); i++)
    sum = (sum + (unsigned char)(k[i]) ) % n_buckets; // code ascii du 1er caractere comme empreinte...
    
  return sum;
}

int hash_fun_conversion_base(unsigned int n_buckets, char *k)
{
  assert( k != NULL);
  
  int sum = 0; // on peut utilsier un unsigned pour gagner un peu de marge sur l'overflow mais pas crucial
  unsigned int n = strlen(k);
  sum = 0;
  uint8_t code_ascii;
    // attention aux compteurs de type unsigned int dans les boucles for avec
  // décrémentation i = i-1 ==> la boucle s'arrete quand le compteur devient
  // négatif, ce compteur ne peut donc pas etre un unsigned int
  // ic on fait avec une incrémentation donc tout va bien
  for (unsigned int i = 1; i <= n; i++) // attention il faut adapter avec notre système..
  {
    code_ascii = (unsigned char)(k[n-i]); // le cast en unsigned int est nécessaire pour les caractères de la table ASCII étendu dont le bit de poids fort est à 1. Les caractères spéciaux comme les é è ë ê à â ô ä ï ù ü ...etc font partie de la table ASCII étendue. Le dictionnaire français traité à l'exercice 3 en contient donc on doit le faire. Mais sinon, il est inutile pour des cclés sans caractères spéciaux ou accents.
    // https://www.ascii33.com/liste-tables-ascii/table-ascii-etendue-ANSI.html
    // Sinon, le code ascii est interprété... comme un entier négatif
    sum = ( (code_ascii + sum*B) % n_buckets ) ; // on fait le modulo a chaque étape pour éviter tout débordement mémoire
  
  }
   // pour "tuer" le bit de poids fort donnant le signe de l'entier en le mettant à 0 à coup sur
  // INT_MAX = 011111....1111 et & est le ET bit à bit... c'est vraiment pour se rassurer car en fait, sum est bien positif avec toutes les précautions prises. On s'est convainc avec un assert

  assert(sum > 0);
  return sum;// &INT_MAX;
}


void set_hash_fun(hashtbl *ht, int (*addr_f)(unsigned int, char *))
{
   ht->hash_function = addr_f;
}

hashtbl *hashtbl_create(unsigned int n_buckets, enum hash_fun_type hf)
{
  hashtbl *ht = (hashtbl *)malloc(sizeof(hashtbl));
  assert(ht != NULL); // check allocation
  
  ht->n_buckets = n_buckets;

  
  // choix de la fonction de hachage et mise en place du pointeur vers
  // la fonction de hachage
  switch(hf)
  {
    case HashFunNaive:
      set_hash_fun(ht, &hash_fun_naive);
      break;
    case HashFunSommative:
      set_hash_fun(ht, &hash_fun_sommative);
      break;
    case HashFunNone:
    case HashFunConversionBase:
    default:
      set_hash_fun(ht, &hash_fun_conversion_base);
      break;  
  }
  
  ht->tab = (list **)malloc(n_buckets*sizeof(list *));

 
  for (unsigned int i = 0; i < ht->n_buckets; i++)
    ht->tab[i] = NULL;
  
  return ht;
}


void hashtbl_add(hashtbl *ht, char * k, elt_type v)
{
  assert(ht != NULL);
  assert(k != NULL);
  int idx  = ht->hash_function(ht->n_buckets, k);
 
  list *l = ht->tab[idx];
  
  if (l == NULL)
  {
    ht->tab[idx] = list_create();
    l = ht->tab[idx];
  }
  
  list_add(l, k, v);
}

bool hashtbl_find(hashtbl *ht, char * k, elt_type *addr_v)
{
  assert(ht != NULL);
  assert(k != NULL);
  assert(addr_v != NULL);
  
  int idx  =  ht->hash_function(ht->n_buckets, k);
  list *l = ht->tab[idx];
  return list_find(l, k, addr_v);
}

bool hashtbl_replace(hashtbl *ht, char * k, elt_type v)
{
  assert(ht != NULL);
  assert(k != NULL);
  
  int idx  = ht->hash_function(ht->n_buckets, k);
  list *l = ht->tab[idx];
  if (l == NULL)
  {
    ht->tab[idx] = list_create();
    l = ht->tab[idx];
  }
  return list_replace(l, k, v);
}

bool hashtbl_remove(hashtbl *ht, char *k)
{
  assert(ht != NULL);
  assert(k != NULL);
  
  int idx  = ht->hash_function(ht->n_buckets, k);
  list *l = ht->tab[idx];
  
  return list_remove(l, k);
}


void hashtbl_print(hashtbl *h)
{
  assert(h != NULL);
  for (unsigned int i = 0; i < h->n_buckets; i++)
  {
    printf("Bucket %d: ", i);
    list_print(h->tab[i]);
  }
  printf("\n");
  return;
}

void hashtbl_free(hashtbl **addr_ht)
{
  assert(addr_ht != NULL);
  
  hashtbl *ht = *addr_ht;

  if (ht == NULL)
    return;
  
  list *l = NULL;
  
  for (unsigned int i = 0; i < ht->n_buckets; i++)
  {
    l = ht->tab[i];
    if (l != NULL)
      list_free(&l);
  }
  free(ht->tab);
  free(ht);

  *addr_ht = NULL;
  
  return;
}


int hashtbl_stats(hashtbl *ht)
{
  unsigned int sum = 0;
  unsigned int max_bucket_size = 0;
  unsigned int bucket_size;
  list *l = NULL;
  
  for (unsigned int i = 0; i < ht->n_buckets; i++)
  {
    l = ht->tab[i];
    if (l != NULL)
    {
      bucket_size = list_length(l);
      if (bucket_size > max_bucket_size)
	max_bucket_size = bucket_size;
      sum = sum + bucket_size;
    }
  }

  if (sum == 0)
    return 0;

  
  unsigned int *histo = calloc((max_bucket_size+1), sizeof(int));

  for (unsigned int  i = 0; i < ht->n_buckets; i++)
  {
    l = ht->tab[i];
    if (l != NULL)
      bucket_size = list_length(l);
    else
      bucket_size = 0;

    histo[bucket_size] = histo[bucket_size] + 1;
    
  }

  printf("Nombre de données (bindings): %d\n", sum);
  printf("Taille maximale de seau: %d\n", max_bucket_size);
  printf("Facteur d'équilibrage théorique: %lf\n", (double)(sum)/(double)(ht->n_buckets));
  printf("Diagramme en bâtons:\n");
  for (unsigned int i = 0; i <= max_bucket_size; i++)
    printf("\tTaille %d: %d seau(x)\n", i, histo[i]);

  
  free(histo);
  
  return sum;
}

hashtbl *hash_file(char *path, unsigned int n_buckets, enum hash_fun_type hf_type)
{
  assert(path != NULL);
  char word[1024]; // taille fixe arbitraire de chaine pour
  // stocker le mot lu... cet espace de 1024 octets est alloué dans le bloc d'activation de cette fonction, dans le segment de pile (segment = region memoire dans la mémoire virtuelle allouée au processus)
  FILE *f = NULL;

  // première passe pour compter le nombre de mots du fichiers afin d'évaluer la taille de la table de hachage.
  if (n_buckets == 0) // si le nombre de seaux n'est pas prescrit par l'utilisateur, on utilise l'heurisitique un peu naive mais pas si mal en pratique
    // n_bucket <- n_entries
  {
    f = fopen(path, "r");
    assert(f != NULL); // vérification ouverture fichier ok

    int n_word = 0;
    while (feof(f) == false)
    {
      fscanf(f, "%s", word); // word donne l'adresse mémoire à partir de laquelle stocker la chaine de caractère lue. L'espace mémoire est alloué dans le bloc d'activation de cette fonction, de manière statique char word[1024];
	// pas super sécurisé car souci si on lit un mot de plus de 1024 caractères...
      n_word = n_word  + 1; 
    }
    fclose(f);

    n_buckets = n_word;
  }

  hashtbl *ht = hashtbl_create(n_buckets, hf_type);// on choisit un nombre de seaux égal au nombre de mots du fichier pour aller vers une "pseudo-"injectivite


  f = fopen(path, "r");
  assert(f != NULL);
  
  double count_line =0; // on utilise des double car nos listes de clé-valeurs sont à valeurs v de type double
  while (feof(f) == false)
  {   
    fscanf(f, "%s", word);
    count_line = count_line + 1;
    //printf("%s\n", word);
    hashtbl_replace(ht, word, count_line); // pour etre sur d'éviter les doublons. replace ajoute l'entrée si la clé n'est pas déjà présente. La valeur associee à chaque clé, ici le numéro de la ligne où la clé a été trouvée dans le fichier, est bidon. C'est l'étude des stats de la table qui nous intéresse, pas les valeurs stockées.
  }
  
  fclose(f);

  return ht;
}

void test_hash_function(uint64_t n_buckets)
{
  char key[1024];

  while (1)
    {
      printf("Entrez une chaine de caractères ou la chaine fin si vous souhaitez arrêter:\n");
      scanf("%s", key);
      if (strcmp(key, "fin") == 0)
	break;
      printf("Empreinte calculée: %d\n", hash_fun_conversion_base(n_buckets, key));
    }
  return;
}

void test_primitives()
{
  hashtbl *ht = hashtbl_create(8, HashFunNaive);

  hashtbl_add(ht, "toto", 4.1);
  hashtbl_add(ht, "tata", 7.);
  hashtbl_add(ht, "titi", 5.3);
  hashtbl_print(ht);

  hashtbl_replace(ht, "titi", 5.7);
  
  double v;
  if (hashtbl_find(ht, "tata", &v) == true)
    printf(elt_fmt, v);
  printf("\n");
 
  hashtbl_remove(ht, "titi");

  hashtbl_print(ht);

  hashtbl_stats(ht);
  printf("\n");
  
  hashtbl_free(&ht);
}

void test_hash_file(char *path, uint64_t n_buckets)
{
 
  //hashtbl *ht = hash_file(path, n_buckets, HashFunConversionBase);

  // test intéressant pour voir l'efficacité de la fonction de hachage
  // de conversion de base par rapport à la somative sur de gros volumes de onnées  et de vraies clés :-)
  hashtbl *ht = hash_file(path, n_buckets, HashFunSommative);
  
  double v; // 8 octets (64 bits) alloués dans le bloc d'activation de cette fonction

  // Remarque: avec les caractères de l'ASCII étendu, dont les caractères accentués français font partie, ca passe encore!
  bool found = hashtbl_find(ht, "persévérance", &v);
  if (found == true)
    printf("Mot trouvé: ligne %f\n", v);
  
  hashtbl_stats(ht);
  hashtbl_free(&ht);
}

int main(void)
{
  //ex1
  //test_hash_function(8);

  //ex2
  //test_primitives();

  //ex3
  test_hash_file("/usr/share/dict/french", 350033);


  return 0;
}

