//  homeworkOne_Part_2_serial

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct timeval tm1, tm2;
long long t;

void SumUpto(int input);

int main(int argc, const char * argv[]) {
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    //Get our number from args
    int number = atoi(argv[1]);
    
    //Get our p from args, if no args set it to 2
    int p;
    if (argc == 2){
         p = 2;
    }
    else{
     p = atoi(argv[2]);
    }
    
    //Loop the SumUpto function p times
    int i;
    for (i=0; i<p; i++){
    SumUpto(number);
    }
    
    //Get the total time in microseconds and print
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    printf("%lld microseconds occured\n",t);
    
    return 0;
}

//SumUpto function
void SumUpto (int number){
    int result = 0;
    int i;
    //Start at 0, add every number up to the number input
    for (i=1; i<=number; i++){
        result = result + i;
    }
    //Print the result
    printf("%d\n", result);
}
