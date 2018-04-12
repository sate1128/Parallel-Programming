#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>


long long int pi = 0;
int num_threads = 0;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void *estimate(void *data);

int main(int argc, char *argv[])
{
    num_threads = atoi(argv[argc-2]);
    long long int num = atoi(argv[argc-1]);
    pthread_t tid[num_threads];
    int i;
    for(i=0;i<num_threads;i++){
        if(pthread_create( &tid[i], NULL, &estimate,(void *)num) != 0){
        perror("create thread failed");
        }
    }
    for (i = 0; i < num_threads; i++){
        pthread_join(tid[i] , NULL);
    }
    pi*=4;
    double pi_estimate = pi/(double)num;
    printf("%lf\n",pi_estimate);
    return 0;
}

void *estimate(void *data)
{
    unsigned int seed = time(NULL);
    long long int num_total = (long long int)data;
    long long int num_toss = num_total / num_threads;
    long long int number_in_circle = 0;
    long long int i;
    for(i=0;i<num_toss;i++){
        double x = (double)rand_r(&seed)/RAND_MAX;
        double y = (double)rand_r(&seed)/RAND_MAX;
        double distance_squared = x*x + y*y;
        if(distance_squared <= 1.0)
            number_in_circle++;
    }
    pthread_mutex_lock( &mutex1 );
    pi+=number_in_circle;
    pthread_mutex_unlock( &mutex1 );
    pthread_exit(NULL);
}
