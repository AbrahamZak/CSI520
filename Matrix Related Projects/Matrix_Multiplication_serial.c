//  homeworkOne_Part_1_serial

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 500
#define RAND 100

void naiveImp(double a[N][N], double b[N][N]);
void optimizedImp(double a[N][N], double b[N][N]);

    //Naïve implementation of algorithm
void naiveImp (double a[N][N], double b[N][N]){
    //Ints for our loops
    int i;
    int j;
    int k;
    //Final matrix
    double c [N][N];
    //Algorithm
    for (i = 0; i<N; i++){
        for (j = 0; j<N; j++){
            for (k = 0; k<N; k++){
                c[i][j]= c[i][j]+a[i][k]*b[k][j];
            }
        }
    }
}

//Optimized implementation of algorithm
void optimizedImp(double a[N][N], double b[N][N]){
    //Ints for our loops
    int i;
    int j;
    int k;
    int t;
    //Final matrix
    double c [N][N];
    //Algorithm
    for (i=0; i<N; ++i) {
        for (k=0; k<N; ++k) {
            double r=a[i][k];
            for (j=0; j<N; ++j) {
                c[i][j] += r * b[k][j];
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    //Create our matrices
    double a [N][N];
    double b [N][N];
    
    //Ints for our loops
    int i;
    int j;
    
    //Randomize matrices a and b
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            a[i][j] = 1 + rand()%RAND;
            b[i][j] = 1 + rand()%RAND;
        }
    }

    //Record the start time
    clock_t startTime = clock();
    
    naiveImp(a, b);
    
    //Record the end time
    clock_t endTime = clock();
    
    //Use start and end time to get computation time
    //Print the final computation time
    double compTime = (double)(endTime - startTime)/CLOCKS_PER_SEC;
    printf("Total computation time (naive): %e seconds\n", compTime);
    
    //Run the test with the optimized algorithm
    //Record the start time
    clock_t startTime2 = clock();
    
    optimizedImp(a, b);
    
    //Record the end time
    clock_t endTime2 = clock();
    
    //Use start and end time to get computation time
    //Print the final computation time
    double compTime2 = (double)(endTime2 - startTime2)/CLOCKS_PER_SEC;
    printf("Total computation time (optimized): %e seconds\n", compTime2);
    
    return 0;
}
