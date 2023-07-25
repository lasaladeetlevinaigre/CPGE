#include <stdio.h>
#include <assert.h>


struct s_point
{
  double x;
  double y;
  char   nom[2];
};

typedef struct s_point point;


  

void demander_triangle(point tri[3])
{
  
  for (unsigned short int i = 0; i < 3; i++)
  {
    printf("Entrez le nom (1 caractère) et les deux coordonnées du point numéro %d puis appuyez sur entree\n", i);

    scanf("%s %lf %lf" , &(tri[i].nom), &(tri[i].x), &(tri[i].y));

    printf("Point %s: (%lf, %lf)\n", tri[i].nom, tri[i].x, tri[i].y);
  }

  return;
}

double calcul_aire_triangle(point tri[3])
{
  
  double ux, uy;
  double vx, vy;

  ux = tri[1].x - tri[0].x;
  uy = tri[1].y - tri[0].y;

  vx = tri[2].x - tri[0].x;
  vy = tri[2].y - tri[0].y;

  double det = ux*vy-uy*vx;

  if (det < 0.)
    det = det * (-1.0);
  
  return 0.5 * det;
}

int main(void)
{
  point tri[3];

  demander_triangle(tri);

  calcul_aire_triangle(tri);
  
  return 0;
}
