#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>
#include "mylist.h"



const int64_t B =  256;  // dans le segment statique RODATA :-)

int64_t M; // dans le segment statique BSS :-)


int64_t modulo(int64_t x, int64_t m)
{
  
  // On stocke sur 64 bits, donc -x est équivalent informatiquement à 2^64-x modulo 2^64 (rappelez vous le cercle dessiné en début d'année (complément à deux...etc)
  // Dans ce cas, quand on a des négatifs, c'est parce que l'on est victime d'un overflow: si on a -24 par exemple, c'est qu'en fait le résultat d'un précédente opération était égale à 2^64-24. Or, on ne peut représenter les entiers positfs que jusqu'à 2^63-1 si on utilise des entiers signés 64 bits: on est donc repassé dans les négatifs

  // Par ailleurs, ici, on travaille sur le plan mathématique avec un deuxième modulo m
  // -x est équivalent mathématiquement à n'importe quel -x +k*m modulo m.
  // Nous on veut juste, repasser dans les positifs, dans l'interalle [0,m[. Donc on a le droit de faire des sauts de taille m jusqu'à y arriver
  // Exemple ( (2^64-24) ) + 5*5 mod 5 = 2^64 + (-24 + 25) mod 5 = 2^64 + 1 mod 5 équivalent à  1 mod 5 sur 64 bits
  // donc pour "repasser dans les positifs modulo m" après un overflow, on additionne des m jusqu'à retrouver un nombre positif.
  // la difficulté ici est de comprendre que l'on travaille modulo m (mathématiquement) et modulo 2^64 (informatiquement)...
    // notre fonction modulo est donc un modulo "double": modulo m et modulo 64... c'est cela qui est contre intuitif
  // Et ca explique que modulo((-24), 5) renvoie 1!
  
  int64_t tmp = x%m;
  
  while (tmp < 0)
    tmp = tmp + m;

  return tmp;
}


int64_t pow_bin(int64_t x, int64_t j)
{
  assert(j >= 0);
  int64_t acc = 1.;
  
  while (j > 0)
  {
    if (j % 2 == 1)
      acc = (acc * x);
     
    x = (x * x);
    j = j / 2;
  }

  return acc;
}

int64_t pow_bin_mod(int64_t x, int64_t j, int64_t m)
{
  assert(j >= 0);
  int64_t acc = 1.;

  x =  modulo(x, m);
  while (j > 0)
  {

    if (j % 2 == 1)
      acc = modulo(acc * x, m);// à chaque produit, il y a un risque depasser en mémoire, on fait donc un modulo à chaque fois
    x = modulo(x * x , m); // à chaque produit, il y a un risque depasser en mémoire, on fait donc un modulo à chaque fois
    
    j = j / 2;
  }

  return acc;
}


int64_t hash(char *k, int p, int64_t m)
{
  assert( k != NULL);
  
  int64_t sum = 0; // on peut utilsier un unsigned pour gagner un peu de marge sur l'overflow mais pas crucial

  sum = 0;
  int64_t code_ascii;
    // attention aux compteurs de type unsigned int dans les boucles for avec
  // décrémentation i = i-1 ==> la boucle s'arrete quand le compteur devient
  // négatif, ce compteur ne peut donc pas etre un unsigned int
  // ic on fait avec une incrémentation donc tout va bien
  for (unsigned int i = 0; i < p; i++) // attention il faut adapter avec notre système..
  {
    code_ascii = (unsigned char)(k[i]); // le cast en unsigned int est nécessaire pour les caractères de la table ASCII étendu dont le bit de poids fort est à 1. Les caractères spéciaux comme les é è ë ê à â ô ä ï ù ü ...etc font partie de la table ASCII étendue. Le dictionnaire français traité à l'exercice 3 en contient donc on doit le faire. Mais sinon, il est inutile pour des cclés sans caractères spéciaux ou accents.
    // https://www.ascii33.com/liste-tables-ascii/table-ascii-etendue-ANSI.html
    // Sinon, le code ascii est interprété... comme un entier négatif
    //sum = ( (code_ascii + (sum*B)%m ) % m ) ; // on fait le modulo a chaque étape pour éviter tout débordement mémoire
   
    sum = modulo(code_ascii + modulo(sum*B, m), m );
    
  }
  
  assert(sum > 0);
  return sum;
}



int rabin_karp_naive(char *text, char *pattern)
{
  assert(text != NULL);
  assert(pattern != NULL);

  int n = strlen(text);
  int p = strlen(pattern);

  int64_t m = M;
  

  if (p > n) // motif plus grand que le texte!
  {
    fprintf(stderr, "Attention, motif plus grand que le texte!\n");
    return 0;
  }
  
  char *s = NULL;
  int64_t h_pattern = hash(pattern, p, m); // à ne calculer qu'une seule fois! 
  int64_t h;
  int nbocc = 0;
  for (unsigned int i = 0; i <= n-p; i++)
  {
    s = &(text[i]); // pointeur vers la case correspondant au caractère d'indice i 
    h = hash(s, p, m); // a partir de s, on hache la chaine constituée des p carctères suivants
    //printf("%ld %ld\n", h, h_pattern);
    if (h == h_pattern) // si l'empreinte matche avec celle du motif, on verifie a la main, sinon, c'est mort et on continue
    {      
      if (strncmp(s, pattern, p) == 0) // p = taille du motif
	nbocc = nbocc + 1;    
    }
  }
  
  return nbocc;
}

void test_rabin_karp_naive(int argc, char **argv)
{
  assert(argc == 3);
  
  char *text = argv[1];
  char *pattern = argv[2];
  
  int nbocc = rabin_karp_naive(text, pattern);

  printf("Le motif \"%s\" a été trouvé %d fois dans le texte suivant:\n%s\n", pattern, nbocc, text);
}




// exercice 3
// un premier choix facile pour renforcer l'injectivité: B = 256
// comme le nombre m n'est pas lié à un "nombre de seaux", et n'amènera pas d'alllocations mémoire comme
// dans le cas ou on voudrait construire une table de hachage, on peut se lacher et le prendre le plus grand
// possible, c'est à dire m= 2^64-1. en encodant les
// empreintes sur des entiers 64 bits uint64_t

// Cependant, pour limiter l'utilisation de l'opérateur modulo sans créer de débordement mémoire, il est plus judicieux de se limite à m <= 2^{30}-1  tout en gardant le stockage sur 64 bits. Grace à cela, tous les produits b*h, avec h une empreinte inférieure à 2^{30} et b=256=2^8, seront assurés de rester dans l'intervalle des entiers 64 bits positifs (bits de poids fort à 0, sans dépassement mémoire et sans nécessité d'effectuer un modulo sur chaque produit.
// Tous les calculs de modulo restent coherents car le calcul des x*x dans pow_bin_mod reste aussi dans la capacité memoire
// Toutefois, pour améliorer l'injectivité on choisit m premier ou en tout cas n'ayant pas de petits diviseurs, donc un choisissant le plus grand nombre premier inférieur strictement à 2^31: c'est 1 869 461 003
// 
int rabin_karp(char *text, char *pattern)
{
  assert(text != NULL);
  assert(pattern != NULL);

  int n = strlen(text);
  int p = strlen(pattern);

  if (p > n) // motif plus grand que le texte!
  {
    fprintf(stderr, "Attention, motif plus grand que le texte!\n");
    return 0;
  }

  int64_t m  = M;  // permet de supprimer un modulo apres l'opération B*(...) dans la formule
  // le -1 sert à éviter de prendre un entier qui a 2 comme diviseur, très mauvais pour l'injectivité
  // dans l'idéal: m plus grand entier premier inféieur ou égal à 2^55-1

  assert(m > B);
  
  int64_t preproc = pow_bin_mod(B, p-1, m);
  
  int64_t h_pattern = hash(pattern, p, m); // on hache le motif  
  
  int64_t h_prev;
  int64_t ci, cipp; // c_{i} et c_{i+p}
  char *s;

  int nbocc = 0;

  int64_t h = hash(text, p, m); // on hache les p premiers caracteres du texte
  if (h == h_pattern)
  {
    if (strncmp(text, pattern, p) == 0)
      nbocc = nbocc + 1;
  }

  for (unsigned int i = 1; i <= n-p; i++)
  {
    
    ci = (unsigned char)(text[i-1]); // code ASCII du caractere, ok avec ASCII étendu
    cipp = (unsigned char)(text[i+p-1]);
    h_prev = h;

    
     // il faut gérer le cas où tmp est négatif, ce qui peut arriver par le jeu des modulo m
    // quand cette situation ce produit, on rajoute m pour repasser en positif, ce qui est correct du point de vue de l'arithmétique modulaire
    int64_t tmp = modulo(h_prev -  modulo(ci*preproc, m), m);
    h = modulo(modulo(B*tmp, m) + cipp, m); // le modulo sur le produit est absolument obligatoire, sinon la soustraction donne un résultat négatif et c'est la catastrophe, les clé sont fausses et l'algorithme devient faux! Mais on a pu en dégager un sur le produit B*(....)
    
    s = &(text[i]);
    
    // pour vérifier l'absence de débordement mémoire, qui sera repéré par des nombres négatifs et une incohérence entre les deux calculs avec modulo (hash direct ou hash deroulant)
    // à commenter bien sur qd on veut vraiment utiliser le code!
    int64_t h_expected = hash(s, p, m);
    if (h != h_expected)
    {
      printf("i=%d %s ==> calculé=%ld <> attendu=%ld\n", i, s, h, h_expected);
      printf("tmp1=%ld - %ld = %ld\n", h_prev, (ci*preproc)%m, h_prev - (ci*preproc)%m);
      printf("temp2=%ld\n", B*(h_prev - (ci*preproc)%m));
      printf("%ld\n", ci*preproc);
      printf("%ld %ld\n", ci, cipp);
      printf("%ld\n", h_prev);
      fflush(stdout);
      assert(h == h_expected);
    }

    

    if (h == h_pattern)
    {
      if (strncmp(s, pattern, p) == 0)
	nbocc = nbocc + 1;    
    }
  }
  
  return nbocc;
}
void test_rabin_karp(int argc, char **argv)
{
  assert(argc == 3);
  
  char *text = argv[1];
  char *pattern = argv[2];
  int nbocc = rabin_karp(text, pattern);

  printf("Le motif \"%s\" a été trouvé %d fois dans le texte suivant:\n%s\n", pattern, nbocc, text);
}






int main(int argc, char **argv)
{
  // M = 123167; nombre au pif
  M = pow_bin(2, 31) - 1; // max possible comme entier positif sur sur int64_t variable globale de sorte que M*M < 2^63
  // dans l'idéal, M= plus grand nombre premier possible inférieur à 2^31... à calculer avec Eratosthene pour les plus courageux :-)

  printf("M=%ld\n", M);
  printf("Test Rabin-Karp naive (sans formule déroulante):\n");
  test_rabin_karp_naive(argc, argv);

  printf("Test Rabin-Karp:\n");
  test_rabin_karp(argc, argv);

  return 0;
}

