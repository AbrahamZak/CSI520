//  homeworkOne_Part_3_parallel

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

struct timeval tm1, tm2;
long long t;
//Count for the final list
int count[10] = {0};
//Value of the amount of ints processed per thread
int splitSize = 0;
#define RAND 10

//Method that will count 1-10's in a portion of the matrix
void *ArrayCounter (void *arg){
    int i;
    int check;
    int *array = (int *) arg;
    for (i=0; i<splitSize; i++){
            check = array[i];
        if (check>=1 && check<=10){
            count[check-1]++;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    //Get matrix size N from args
    int N = atoi(argv[1]);
    //Get our P from args
    long P = atoi(argv[2]);
    //Get our split size
    splitSize = ((N*N)/P);
    if ((splitSize * P) < (N*N)) {
       splitSize++;
    }
    
    //Set randoms
    srand((unsigned)time(NULL));
    
    //Create our matrix
    int a [N][N];
    
    //Ints for our loops
    int x;
    int y;
    
    //Randomize the matrix
    for (x=0; x<N; x++){
        for (y=0; y<N; y++){
            a[x][y] = 1 + rand()%RAND;
        }
    }
    
    //Create a 1D representation of our matrix
    int oneD[N*N];
    int location = 0;
    for (x=0; x<N; x++){
        for (y=0; y<N; y++){
            oneD[location] = a[x][y];
            location++;
        }
    }
    
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    //Create 'p' threads and run function for each thread
    pthread_t threads[P];
    int rc;
    long i;
    //Set location back to 0
    location = 0;
    
    int split[P][splitSize];
    int l;
    for (i=0; i<P; i++){
    //Fill our array for each thread
    for (l=0; l<splitSize; l++){
        //If we've used all of our matrix fill the rest of the array with 0s
        if (location==(N*N)){
            split[i][l] = 0;
        }
        //Else we continue to fill and increment location
        else{
            split[i][l] = oneD[location];
            location++;
        }
    }
    }
    //For each thread we create an array an fill it with a portion of our matrix
    for (i=0; i<P; i++){
        rc = pthread_create(&threads[i],NULL,ArrayCounter,(void *)split[i]);
        if (rc){
            printf("ERROR: return code form pthread_create() is %d\n",rc);
            exit(-1);
        }
    }
    
    //Join threads
    void *status;
    for (i=0; i<P; i++){
        rc = pthread_join(threads[i], &status);
        if (rc){
            printf("\nError occured in pthread_join %d", rc);
        }
    }
    
    //Get the total time in microseconds and print
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    for (x=1; x<=10; x++){
        printf("%d: %d\n", x, count[x-1]);
    }
    printf("%lld microseconds occured\n",t);
    
    //Exit threads
    pthread_exit(NULL);
}
