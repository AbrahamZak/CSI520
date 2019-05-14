//  homeworkSix_Part_1_Parallel

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <mpi.h>

struct timeval tm1, tm2;
long long t;
MPI_Status status;

int main(int argc, char * argv[]) {
    
    int tasks,rc,tid,slaves,source,destination,rows,offset,i,j,k;
    
    //Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &tid);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    
    //Get our matrix size from args
    int N = atoi(argv[1]);

    //Allocate memory for matrices
    double (*a)[N] = malloc(N * N * sizeof (double));
    double (*b)[N] = malloc(N * N * sizeof (double));
    double (*c)[N] = malloc(N * N * sizeof (double));
    
    slaves = tasks-1;
    
    //Input our first matrix from file 1.csv
    FILE *inOne = fopen("1.csv", "r");
    if (inOne){
        size_t i, j;
        char buffer[65536], *ptr;
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
    if (inTwo)
    {
        size_t i, j;
        char buffer[65536], *ptr;
        //Skip the first line
        fgets(buffer, 100, inTwo);
        for ( i = 0; fgets(buffer, sizeof buffer, inTwo); ++i )
        {
            for ( j = 0, ptr = buffer; j < N; ++j, ++ptr)
            {
                b[i][j] = strtod(ptr, &ptr);
            }
        }
        fclose(inTwo);
    }
    
    //For master
    if (tid == 0) {
        //Begin timing
        gettimeofday(&tm1, NULL);
        
        offset = 0;
        //Get the rows per worker value
        int rows = (N + (slaves - 1)) / slaves;
        
        //Send data to workers
        for (destination=1; destination<=slaves; destination++)
        {
            MPI_Send(&offset, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], rows*N, MPI_DOUBLE,destination,1, MPI_COMM_WORLD);
            MPI_Send(*b, N*N, MPI_DOUBLE, destination, 1, MPI_COMM_WORLD);
            offset = offset + rows;
        }
        
        //Recieve the results from workers
        for (i=1; i<=slaves; i++)
        {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset][0], rows*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
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
        
        //Get the total time in microseconds and print
        printf("%lld microseconds occured\n",t);
        
    }
    
    //For workers
    if (tid>0){
        //Recieve pieces of matrix to work on
        source = 0;
        MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(*a, rows*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(*b, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
        
        //Multiply a & b portions and send back to master
        for (k=0; k<N; k++)
            for (i=0; i<rows; i++) {
                c[i][k] = 0.0;
                for (j=0; j<N; j++)
                    c[i][k] = c[i][k] + a[i][j] * b[j][k];
            }
        
        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(*c, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}



