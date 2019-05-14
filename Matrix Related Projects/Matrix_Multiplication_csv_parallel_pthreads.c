//  homeworkTwo_Part_1_Parallel

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

struct timeval tm1, tm2;
long long t;
int N;

void *multiply(void* arg);

//Struct for our arrays
typedef struct Matrix
{
    int curCore;
    int MatrixSize;
    double **a;
    double **b;
    double **c;
}Matrix;

//Mutex to be used
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//Multithread implementation of algorithm 
void *multiply(void* arg){
    pthread_mutex_lock(&lock);
    Matrix *my_matrix  = (Matrix*)arg;
    //Ints for our loops
    int i = my_matrix->curCore;
    my_matrix->curCore = my_matrix->curCore + 1;
    pthread_mutex_unlock(&lock);
    int j;
    int k;
    //Algorithm
        for (k=0; k<my_matrix->MatrixSize; ++k) {
            double r=my_matrix->a[i][k];
            for (j=0; j<my_matrix->MatrixSize; ++j) {
                my_matrix->c[i][j] += r * my_matrix->b[k][j];
            }
        }
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    //Get our matrix size from args
    int N = atoi(argv[1]);
    
    //Create our matrix struct and allocate memory
    Matrix abc;
    abc.a = malloc(N * sizeof(double *));
    abc.b = malloc(N * sizeof(double *));
    abc.c = malloc(N * sizeof(double *));
    abc.MatrixSize = N;
    abc.curCore = 0;
    int z;
    for(z = 0; z < N; z++)
    {
        abc.a[z] = malloc(N * sizeof(double));
        abc.b[z] = malloc(N * sizeof(double));
        abc.c[z] = malloc(N * sizeof(double));
    }
    
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
                abc.a[i][j] = strtod(ptr, &ptr);
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
                abc.b[i][j] = strtod(ptr, &ptr);
            }
        }
        fclose(inOne);
    }
    
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n Mutex failed\n");
        exit(-1);
    }
    
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    //Multiply a & b into c
    //Create N amount of threads and run function for each thread
    pthread_t threads[N];
    int rc;
    long i;
    //For each thread we create an array an fill it with a portion of our matrix
    for (i=0; i<N; i++){
        rc = pthread_create(&threads[i],NULL,multiply,(void *)&abc);
        if (rc){
            printf("ERROR: return code form pthread_create() is %d\n",rc);
            exit(-1);
        }
    }
    
    //Join threads
    void *status;
    for (i=0; i<N; i++){
        rc = pthread_join(threads[i], &status);
        if (rc){
            printf("\nError occured in pthread_join %d", rc);
        }
    }
    
    //Destroy mutex
    pthread_mutex_destroy(&lock);
    
    //Get the total time in microseconds and print
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    
    //Export matrix c to 3.csv
    int j;
    FILE *fp;
    char comma[2];
    fp = fopen("3.csv", "w");
    fprintf(fp, "%d\n", N);
    for (i = 0; i<N; i++)
    {
        sprintf(comma, "%s", "");
        for (j = 0; j<N; j++)
        {
            fprintf(fp, "%s%f",comma,abc.c[i][j]);
            sprintf(comma, "%s", ",");;
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
    
    //Free the memory
    for(z = 0; z < N; z++){
        free(abc.a[z]);
        free(abc.b[z]);
        free(abc.c[z]);
    }
    free(abc.a);
    free(abc.b);
    free(abc.c);
    
   //print time
    printf("%lld microseconds occured\n",t);
    
    //Exit threads
    pthread_exit(NULL);
}
