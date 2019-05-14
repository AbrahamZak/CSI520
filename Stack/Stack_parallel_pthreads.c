//  homeworkThree_Part_1_Parallel

#include "stackHw.h"
#include <sys/time.h>
#include <pthread.h>

struct timeval tm1, tm2;
long long t;
pthread_mutex_t pop_mutex;

void *popPushThreads(void* arg);

//Create a stack with a linked list
struct Node {
    int number;
    struct Node* next;
};
struct Node* top;

void *popPushThreads(void* arg){
    long z = (long) arg;
    int i;
    int j;
    //Push to stack
    for (i=0; i<z; i++){
        for (j=0; j<numSize; j++){
            Push(numbers[j]);
        }
    }
    //Pop all numbers, but only under the condition that we can currently pop (threads take turns popping so we don't pop an empty stack)
    pthread_mutex_lock(&pop_mutex);
    for (i=0; i<z*numSize; i++){
        if (top==NULL){
            break;
        }
        Pop();
    }
    pthread_mutex_unlock(&pop_mutex);
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    //Get the numbers and size from text file
    GetNumbers("numbers.txt");
    
    //Initialize mutex and condition variable
    pthread_mutex_init(&pop_mutex, NULL);
    
    //Create NUM_THREADS amount of threads and run function for each thread
    pthread_t threads[NUM_THREADS];
    long z = (ITERATIONS / NUM_THREADS);
    int rc;
    long i;
    //For each thread we create an array an fill it with a portion of our matrix
    for (i=0; i<NUM_THREADS; i++){
        rc = pthread_create(&threads[i],NULL,popPushThreads,(void *)z);
        if (rc){
            printf("ERROR: return code form pthread_create() is %d\n",rc);
            exit(-1);
        }
    }
    
    //Join threads
    void *status;
    for (i=0; i<NUM_THREADS; i++){
        rc = pthread_join(threads[i], &status);
        if (rc){
            printf("\nError occured in pthread_join %d", rc);
        }
    }
    
    //Get the total time in microseconds and print
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    printf("%lld microseconds occured\n",t);
    
    //Exit threads
    pthread_mutex_destroy(&pop_mutex);
    pthread_exit(NULL);
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
        //free(temp);
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
