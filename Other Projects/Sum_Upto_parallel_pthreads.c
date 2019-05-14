//  homeworkOne_Part_2_parallel

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

struct timeval tm1, tm2;
long long t;

//SumUpto function
void *SumUpto (void *arg){
    long number = (long)arg;
    int result = 0;
    int i;
    //Start at 0, add every number up to the number input
    for (i=1; i<=number; i++){
        result = result + i;
    }
    //Print the result
    printf("%d\n", result);
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    //Get our number from args
    long number = atoi(argv[1]);
    
    //Get our p from args, if no args set it to 2
    long p;
    if (argc == 2){
        p = 2;
    }
    else{
        p = atoi(argv[2]);
    }
    
    //Create 'p' threads and run function for each thread
    pthread_t threads[p];
    int rc;
    long i;
    for (i=0; i<p; i++){
        rc = pthread_create(&threads[i],NULL,SumUpto,(void *)number);
        if (rc){
            printf("ERROR: return code form pthread_create() is %d\n",rc);
            exit(-1);
        }
    }
    
    void *status;
    for (i=0; i<p; i++){
        rc = pthread_join(threads[i], &status);
        if (rc){
            printf("\nError occured in pthread_join %d", rc);
        }
    }
    //Get the total time in microseconds and print
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    printf("%lld microseconds occured\n",t);
    
    //Exit threads
    pthread_exit(NULL);
}


