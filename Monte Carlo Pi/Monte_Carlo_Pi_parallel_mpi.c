//  homeworkSeven_Part_1_Parallel

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

struct timeval tm1, tm2;
long long t;

int main(int argc, char * argv[]) {
    //Get our points amount from args
    int n = atoi(argv[1]);
    //Create doubles for everything we are using,
    //which includes x and y coordinates, distance from origin, and final pi calcualtion
    double x;
    double y;
    double distance;
    double pi = 0;
    int rank;
    int nodes;
    //Count the points in the circle
    int count = 0;
    //Count result from reduction of all processes
    int countReduction;
    
    //Start MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nodes);
    
    //Randomizer ssed
    srand48(time(NULL)+rank);
    
    int i;
    
    if (rank==0){
    //Begin timing
    gettimeofday(&tm1, NULL);
    }
    
        for (i = 0; i < n / nodes; i++) {
            //Create random coordinates
            x = (double)drand48();
            y = (double)drand48();
            
            //Calculate the distance from (x,y) to origin
            distance = x * x + y * y;
            
            //If the distance is <=1 then we are inside the circle
            if (distance <= 1)
                count++;
        }
    
    //Reduction of all processes into countReduction from individual counts
    MPI_Reduce(&count, &countReduction, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    //Master thread calculates final pi value
if (rank==0){
    //Estimate pi
    pi = ((double)countReduction/(double)n)*4.0;
    //Get the total time in microseconds and print
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    printf("Pi estimation: %f\n", pi);
    printf("%lld microseconds occured\n",t);
}
    MPI_Finalize();    
    return 0;
}
