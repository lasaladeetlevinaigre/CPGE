
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <stdbool.h>
#include "sort.h"


double time_diff(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}





double compute_average_complexity(int size, int n_samples)
{
  struct timeval start;
  struct timeval end;
  int *tab = NULL;

  
  double sum = 0.;

  for (int sample = 0; sample < n_samples; sample++)
  {
    tab = generate_random_array(size, 0, 100*size);
    
    gettimeofday(&start, NULL);
    quicksort(tab, size);
    gettimeofday(&end, NULL);
    
    sum += time_diff(&start, &end);
    
    free(tab);
    tab = NULL;
  }
  sum = sum / n_samples;
  
  return sum;
}

int relation_ordre(const void *a, const void *b)
{
  int *pa = (int *)a;
  int *pb = (int *)b;

  if (*pa < *pb)
    return 1;
  else
    return 0;
}


void write_analysis_file(char *path, int *sizes, int n_sizes, int n_samples)
{
 
  double average_time;
  int size;


  FILE *f = fopen(path, "w");
  assert(f != NULL);
  
  for (int iSiz = 0; iSiz < n_sizes; iSiz++)
  {
    size = sizes[iSiz] ;
    average_time = compute_average_complexity(size, n_samples);
    printf("Taille=%d: Temps moyen d'exécution: %0.8f sec.\n", size, average_time);
    fprintf(f, "%d,%0.8f\n", size, average_time);
  }

  fclose(f);
  return;
}


int main(int argc, char **argv)
{
  
  srand(time(NULL));
  
  int sizes[9] = {1000, 10000, 25000, 50000, 75000, 100000, 125000, 250000, 500000};
  int n_sizes = 9;
  int n_samples = 20;

  
  write_analysis_file("./complexity_analysis.csv", sizes, n_sizes, n_samples);
  
  return 0;
}
