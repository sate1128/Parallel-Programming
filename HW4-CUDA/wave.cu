/**********************************************************************
 * DESCRIPTION:
 *   Serial Concurrent Wave Equation - C Version
 *   This program implements the concurrent wave equation
 *********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXPOINTS 1000000
#define MAXSTEPS 1000000
#define MINPOINTS 20
#define PI 3.14159265

void check_param(void);
void init_line(void);
void update (void);
void printfinal (void);

int nsteps,                 	/* number of time steps */
    tpoints, 	     		/* total points along string */
    rcode;                  	/* generic return code */


/**********************************************************************
 *	Checks input values from parameters
 *********************************************************************/
void check_param(void)
{
   char tchar[20];

   /* check number of points, number of iterations */
   while ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS)) {
      printf("Enter number of points along vibrating string [%d-%d]: "
           ,MINPOINTS, MAXPOINTS);
      scanf("%s", tchar);
      tpoints = atoi(tchar);
      if ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS))
         printf("Invalid. Please enter value between %d and %d\n", 
                 MINPOINTS, MAXPOINTS);
   }
   while ((nsteps < 1) || (nsteps > MAXSTEPS)) {
      printf("Enter number of time steps [1-%d]: ", MAXSTEPS);
      scanf("%s", tchar);
      nsteps = atoi(tchar);
      if ((nsteps < 1) || (nsteps > MAXSTEPS))
         printf("Invalid. Please enter value between 1 and %d\n", MAXSTEPS);
   }

   printf("Using points = %d, steps = %d\n", tpoints, nsteps);

}

__global__ void calc(float* finalval,int tpoints, int nsteps)
{
	int i;
	float x, values, newval, oldval;
	int tx = blockIdx.x * blockDim.x + threadIdx.x;

	/* Calculate initial values based on sine curve */
	x = float(tx - 1) / (tpoints - 1);
	values = sin (2.0 * PI * x);
	
	/* Initialize old values array */
	oldval = values;
	
	if ((tx == 1) || (tx  == tpoints))
		values = 0.0;
	else{
		for (i = 1; i<= nsteps; i++) {
			newval = 1.82 * values - oldval;
			/* Update old values with new values */
			oldval = values;
			values = newval;
		}
	}
	finalval[tx] = values;
}

/**********************************************************************
 *     Print final results
 *********************************************************************/
void printfinal(float* values)
{
   int i;

   for (i = 1; i <= tpoints; i++) {
      printf("%6.4f ", values[i]);
      if (i%10 == 0)
         printf("\n");
   }
}

/**********************************************************************
 *	Main program
 *********************************************************************/
int main(int argc, char *argv[])
{
	float finalval[MAXPOINTS + 2];
	float* finald;
	int size = (MAXPOINTS + 2) * sizeof(float),
		threadPerBlock = 1024,
		numBlock = tpoints/threadPerBlock + 1;
	cudaMalloc(&finald,size);
	sscanf(argv[1],"%d",&tpoints);
	sscanf(argv[2],"%d",&nsteps);
	check_param();
	printf("Initializing points on the line...\n");
	//init_line();
	printf("Updating all points for all time steps...\n");
	//update();
	calc<<<numBlock,threadPerBlock>>>(finald,tpoints,nsteps);
	cudaMemcpy(finalval, finald, size, cudaMemcpyDeviceToHost);
	cudaFree(finald);
	printf("Printing final results...\n");
	printfinal(finalval);
	printf("\nDone.\n\n");
	
	return 0;
}
