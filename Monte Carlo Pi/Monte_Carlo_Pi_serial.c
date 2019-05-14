//  homeworkFour_Part_1_Serial

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct timeval tm1, tm2;
long long t;

int main(int argc, const char * argv[]) {
    //Get our points amount from args
    int n = atoi(argv[1]);
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
    
    //Begin timing
    gettimeofday(&tm1, NULL);
    int i;
    for (i = 0; i < n; i++) {
        //Create random coordinates
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        
        //Calculate the distance from (x,y) to origin
        distance = x * x + y * y;
        
        //If the distance is <=1 then we are inside the circle
        if (distance <= 1)
            count++;
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
