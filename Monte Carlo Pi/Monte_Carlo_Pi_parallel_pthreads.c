//  homeworkFour_Part_1_Parallel

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

struct timeval tm1, tm2;
long long t;

//Points in the circle for each thread, number of threads, and total points
int *counterThread;
int threadCount;
int n;

void *monteCarlo(void* arg);

void *monteCarlo(void* arg){
    //Get the current thread number
    long threadNum = (long)arg;
    //Create doubles for everything we are using,
    //which includes x and y coordinates, distance from origin, and count
    double x;
    double y;
    double distance;
    int count = 0;
    //Create a seed for current thread
    unsigned int seed = rand();
    int i;
    for (i = 0; i < (n / threadCount); i++) {
        //Create random coordinates
        x = (double)rand_r(&seed) / RAND_MAX;
        y = (double)rand_r(&seed) / RAND_MAX;
        
        //Calculate the distance from (x,y) to origin
        distance = x * x + y * y;
        
        //If the distance is <=1 then we are inside the circle
        if (distance <= 1)
            count++;
    }
    //Set the count for this thread
    counterThread[threadNum] = count;
    
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    //Randomizer
    srand((unsigned int) time(NULL));
    //Get our total points amount from args
    n = atoi(argv[1]);
    //Get our thread amount from args
    threadCount = atoi(argv[2]);
    //Allocate memory for counter array for each thread
    counterThread = malloc(threadCount*sizeof(int));
    
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    //Create 'n' threads and run function for each thread
    pthread_t threads[threadCount];
    int rc;
    long i;
    for (i=0; i<(long)threadCount; i++){
        rc = pthread_create(&threads[i],NULL,monteCarlo,(void *)i);
        if (rc){
            printf("ERROR: return code form pthread_create() is %d\n",rc);
            exit(-1);
        }
    }
    
    void *status;
    for (i=0; i<(long)threadCount; i++){
        rc = pthread_join(threads[i], &status);
        if (rc){
            printf("\nError occured in pthread_join %d", rc);
        }
    }
    
    //Get the total time in microseconds for thread calculations
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    
    //Add all the individual thread counts
    int finalCount = 0;
    for (i=0; i<threadCount; i++){
        finalCount = finalCount + counterThread[i];
    }
    //Estimate pi
    double pi = 0;
    pi = ((double)finalCount/(double)n)*4.0;
    
    //Print estimation and timing
    printf("Pi estimation: %f\n", pi);
    printf("%lld microseconds occured\n",t);
    
    free(counterThread);
    return 0;
}
