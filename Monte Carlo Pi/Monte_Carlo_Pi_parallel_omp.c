//  homeworkFour_Part_2_Parallel

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

void seedThreads(void);

struct timeval tm1, tm2;
long long t;
unsigned int *seeds;

int main(int argc, const char * argv[]) {
    //Get our points amount from args
    int n = atoi(argv[1]);
    //Get our thread amount from args
    int threadCount = atoi(argv[2]);
    //Allocate memory for seeds for each thread
    seeds = malloc(threadCount*sizeof(unsigned int));
    //Create doubles for everything we are using,
    //which includes x and y coordinates, distance from origin, and final pi calcualtion
    double x;
    double y;
    double distance;
    double pi = 0;
    
    //Count the points in the circle
    int count = 0;
    
    //Randomizer
    srand((unsigned int) time(NULL));
    
    int i;
    //Set threads and run omp for our for loop
    omp_set_num_threads(threadCount);
    seedThreads();
    
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    int tid;
    unsigned int seed;
#pragma omp parallel private(tid, seed, x, y, distance) shared(seeds) reduction(+:count)
    {
        tid = omp_get_thread_num();
        seed = seeds[tid];
        srand(seed);
#pragma omp for
        for (i = 0; i < n; i++) {
            //Create random coordinates
            x = (double)rand_r(&seed) / RAND_MAX;
            y = (double)rand_r(&seed) / RAND_MAX;
            
            //Calculate the distance from (x,y) to origin
            distance = x * x + y * y;
            
            //If the distance is <=1 then we are inside the circle
            if (distance <= 1)
                count++;
        }
    }
    //Estimate pi
    pi = ((double)count/(double)n)*4.0;
    
    //Get the total time in microseconds and print
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    printf("Pi estimation: %f\n", pi);
    printf("%lld microseconds occured\n",t);
    
    return 0;
}

//Function to get seeds for all threads
void seedThreads() {
    int my_thread_id;
    unsigned int seed;
#pragma omp parallel private (seed, my_thread_id)
    {
        my_thread_id = omp_get_thread_num();
        //create seed for thread with time
        seed = (unsigned) time(NULL);
        seeds[my_thread_id] = (seed & 0xFFFFFFF0) | (my_thread_id + 1);
    }
    
}
