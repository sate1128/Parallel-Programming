#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int isprime(int n) {
  long long int i,squareroot;
  if (n>10) {
    squareroot = (int) sqrt(n);
    for (i=3; i<=squareroot; i=i+2)
      if ((n%i)==0)
        return 0;
    return 1;
  }
  else
    return 0;
}

int main(int argc, char *argv[])
{
  int my_rank,
      p,
      stride;


  long long int local_pc,       
                total_pc,
                n, 
                limit,
                foundone,
                maxprime,
                local_start;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&p);

  local_start = (my_rank*2)+1;       
  stride = p*2;          
  local_pc = 0;                     
  foundone = 0;

  sscanf(argv[1],"%llu",&limit);	

  for (n=local_start; n<=limit; n=n+stride) {
    if (isprime(n)) {
      local_pc++;
      foundone = n;
    }			
  }

  if(my_rank == 0){
    printf("Starting. Numbers to be scanned= %lld\n",limit);
    MPI_Reduce(&local_pc,&total_pc,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
    total_pc += 4;
    printf("Done. Largest prime is %d Total primes %d\n",maxprime,total_pc);
  }else{
    MPI_Reduce(&local_pc,&total_pc,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
} 
