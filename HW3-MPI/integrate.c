#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define PI 3.1415926535

int main(int argc, char **argv) 
{
  int my_rank, p, stride, local_start;
  long long i, num_intervals;
  double rect_width, area, local_sum, total_sum, x_middle; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&p);

  sscanf(argv[1],"%llu",&num_intervals);

  rect_width = PI / num_intervals;
  local_start = my_rank+1;
  stride = p;
  local_sum = 0;

  for(i=local_start; i < num_intervals + 1; i+=stride) {

    /* find the middle of the interval on the X-axis. */ 

    x_middle = (i - 0.5) * rect_width;
    area = sin(x_middle) * rect_width; 
    local_sum = local_sum + area;
  } 

  if(my_rank == 0){
    MPI_Reduce(&local_sum,&total_sum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    printf("The total area is: %f\n", (float)total_sum);
  }else{
    MPI_Reduce(&local_sum,&total_sum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}   
