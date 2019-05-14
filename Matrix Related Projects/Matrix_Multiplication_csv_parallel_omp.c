//  homeworkFive_Part_1_Parallel

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

struct timeval tm1, tm2;
long long t;
int N;

void multiply(int N, double a[N][N], double b[N][N]);

int main(int argc, const char * argv[]) {
    //Get our matrix size from args
    int N = atoi(argv[1]);

    double a [N][N];
    double b [N][N];
    
    //Input our first matrix from file 1.csv
    FILE *inOne = fopen("1.csv", "r");
    if (inOne){
        size_t i, j;
        char buffer[4096], *ptr;
        //Skip the first line
        fgets(buffer, 100, inOne);
        for ( i = 0; fgets(buffer, sizeof buffer, inOne); ++i )
        {
            for ( j = 0, ptr = buffer; j < N; ++j, ++ptr)
            {
                a[i][j] = strtod(ptr, &ptr);
            }
        }
        fclose(inOne);
    }
    
     //Input our second matrix from file 2.csv
        FILE *inTwo = fopen("2.csv", "r");
        if (inOne)
        {
            size_t i, j;
            char buffer[4096], *ptr;
            //Skip the first line
            fgets(buffer, 100, inTwo);
            for ( i = 0; fgets(buffer, sizeof buffer, inTwo); ++i )
            {
                for ( j = 0, ptr = buffer; j < N; ++j, ++ptr)
                {
                    b[i][j] = strtod(ptr, &ptr);
                }
            }
            fclose(inOne);
        }
    
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    //Set threads and run omp for our for loop
    omp_set_num_threads(N);
    
    //Multiply a & b (and output final matrix in 3.csv)
    multiply(N,a,b);
    
    //Get the total time in microseconds and print
    printf("%lld microseconds occured\n",t);
    
    return 0;
}

//Matrix multiplication function
void multiply(int N, double a[N][N], double b[N][N]){
    //Ints for our loops
    int i;
    int j;
    int k;
    //Final matrix
    double c [N][N];
    double r;
    //Algorithm
    #pragma omp parallel shared(a,b,c,N) private(i,j,k,r)
    {
        i = omp_get_thread_num();
    #pragma omg for
        for (k=0; k<N; ++k) {
            r=a[i][k];
            for (j=0; j<N; ++j) {
                c[i][j] += r * b[k][j];
            }
        }
    }
    //Get end timing
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    
    //Output to file 3.csv
    FILE *fp;
    char comma[2];
    fp = fopen("3.csv", "w");
    fprintf(fp, "%d\n", N);
    for (i = 0; i<N; i++)
    {
        sprintf(comma, "%s", "");
        for (j = 0; j<N; j++)
        {
            fprintf(fp, "%s%f",comma,c[i][j]);
            sprintf(comma, "%s", ",");;
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
    }


