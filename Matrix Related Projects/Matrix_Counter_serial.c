//  homeworkOne_Part_3_serial

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

struct timeval tm1, tm2;
long long t;

#define RAND 10

int main(int argc, const char * argv[]) {
    //Get matrix size N
    int N = atoi(argv[1]);
    
    //Set randoms
    srand((unsigned)time(NULL));
    
    //Create our matrix
    int a [N][N];
    
    //Ints for our loops
    int i;
    int j;
    
    //Randomize the matrix
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            a[i][j] = 1 + rand()%RAND;
        }
    }
    
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    int count[10] = {0};
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            int z = a[i][j];
            count[z-1]++;
        }
    }
    
    //Get the total time in microseconds and print
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    
    for (i=1; i<=10; i++){
        printf("%d: %d\n", i, count[i-1]);
    }
    
     printf("%lld microseconds occured\n",t);
    
    return 0;
}
