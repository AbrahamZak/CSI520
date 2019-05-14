//  homeworkThree_Part_1_Serial

#include "stackHw.h"
#include <sys/time.h>

struct timeval tm1, tm2;
long long t;

//Create a stack with a linked list
struct Node {
    int number;
    struct Node* next;
};
struct Node* top;

int main(int argc, const char * argv[]) {
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    //Get the numbers and size from text file
    GetNumbers("numbers.txt");
    
    //Push all numbers to stack N times
    int i;
    int j;
    for (i=0; i<ITERATIONS; i++){
        for (j=0; j<numSize; j++){
            Push(numbers[j]);
        }
    }
    //Pop all numbers
    for (i=0; i<ITERATIONS*numSize; i++){
        Pop();
    }
    //Get the total time in microseconds and print
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    printf("%lld microseconds occured\n",t);
    
    return 0;
}

long long GetStackCount(){
    //Create a counter i
    long i = 0;
    //Temp node
    struct Node* temp;
    temp = top;
    //Iterate and increment i by 1 each time
    while (temp != NULL) {
        i++;
        temp = temp->next;
    }
    return i;
}

int Pop(){
    //Create a temp node
    struct Node* temp;
    
    //Check if there is anything to pop
    if (top == NULL) {
        printf("Stack Empty\n");
        return 0;
    }
    else {
        //Get the data from the top
        int num = top->number;
        //Begin pop process
        temp = top;
        //Make top = top -> next
        top = top->next;
        //Finalize the pop
        temp->next = NULL;
        //Free the memory
        free(temp);
        //return the data from the original top
        return num;
    }
    
}
void Push(int number){
    //Create a temp node and allocate the memory
    struct Node* temp;
    temp = (struct Node*)malloc(sizeof(struct Node));
    //Set the data of temp to input number
    temp->number = number;
    //Set the next to current top
    temp->next = top;
    //Make the temp into the top
    top = temp;
}
