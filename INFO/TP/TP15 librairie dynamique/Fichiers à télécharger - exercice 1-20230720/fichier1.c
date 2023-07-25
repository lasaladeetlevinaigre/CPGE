#include <stdio.h>

int main(void)
{
     int n1, n2 ;
     
     printf("Entrez le premier entier ") ;
     scanf("%d",&n1) ;
     printf("Entrez le second entier ") ;
     scanf("%d",&n2) ;
     
     printf("Le plus grand est %d\n", plusgrand(n1,n2)) ;

     return 0 ;
}
