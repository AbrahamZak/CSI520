//  homeworkFour_Part_1_Parallel

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

struct timeval tm1, tm2;
long long t;
int task [26];
char (*data)[25];
int* length;
void *bagOfTasks(void* arg);

void *bagOfTasks(void* arg){
    int totalFromThisTask = 0;
    long a = (long) arg;
    int z = (int)(a);
    
    int location;
    int start = 0;
    if (z==0){
        start = 0;
    }
    else if (z>0){
        start = task[z-1];
        if (task[z-1]==0){
            start = task[z-2];
        }
    }
    //From 0 -> count we check if each word has unique characters
    //If it does we set length[location] to the length of the word
    //We also skip non-words (anything starting w/ numbers/capital letters)
    for (location = start; location<task[z]; location++){
        //Get length of current word
        int len = (int)strlen(data[location]);
        len--;
        int i;
        //Skip words that are not only lowercase letters
        int skip = 0;
        for (i=0; i<len; i++){
            if (skip==1){
                break;
            }
            if (data[location][i]<97 || data[location][i]>122){
                skip = 1;
            }
        }
        if (skip==1){
            skip = 0;
            continue;
        }
    
        //Match is 0 is false and 1 if true (skip the word because it isn't unique)
        int match = 0;
        int j;
        for (i = 0; i < len; i++) {
            for (j = i + 1; j < len; j++) {
                if (data[location][i] == data[location][j]) {
                    //If we find a match we set match to 1 and break the loop
                    match=1;
                    break;
                }
            }
            //If match is 1 we break this loop as well
            if (match==1){
                break;
            }
        }
        //If match is 1 we set it back to 0 and ontinue to next word
        if (match==1){
            match = 0;
            continue;
        }
        //If we didn't find a match we set the value of length[location] to the length of the current word
        totalFromThisTask++;
        length[location] = len;
    }
    printf("Count (thread %c): %d\n",(int)z+97, totalFromThisTask);
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    //Get amount of workers from args
    int N = atoi(argv[1]);
    
    //Count lines in the file
    FILE *fp;
    int count = 0;
    fp = fopen("words.txt", "r");
    // Check if file exists
    if (fp == NULL){
        printf("Could not open file\n");
        return 0;
    }
    char line[25];
    while(fgets(line,sizeof(line),fp) != NULL){
        count++;
    }
    fseek(fp, 0, SEEK_SET);
    
    //Create an array for lengths
    length = (int*) malloc(count * sizeof(int));
    int x;
    for (x=0; x<count; x++){
        length[x] = 0;
    }
    
    //Get the lines from the text file into a character array
    //Also mark positions for each thread by start letter
    data = malloc(sizeof(char [count][25]));
    int location = 0;
    int letter = 98;
    int pos = 0;
    while(fgets(line,sizeof(line),fp) != NULL){
        if (line[0]==letter){
            task[pos] = location;
            letter++;
            pos++;
        }
        //If we skip a letter give that task a value of 0
        else if (line[0]==letter+1){
            task[pos] = location;
            task[pos+1] = 0;
            pos = pos+2;
            letter = letter+2;
        }
        strcpy(data[location], line);
        location++;
    }
    task[25] = count;
    fclose(fp);
    
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    //Create N amount of threads and run function for each thread
    pthread_t threads[N];
    int rc;
    long i;
    //For each thread we create a task
    for (i=0; i<N; i++){
        rc = pthread_create(&threads[i],NULL,bagOfTasks,(void *)i);
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
    
    //Get the total time in microseconds
    gettimeofday(&tm2, NULL);
    t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
    
    //Count total number of words and write words greater than length 6 to results.txt
    FILE *fp2;
    fp2 = fopen("results.txt", "w");
    int totalCount=0;
    for (location = 0; location<count; location++){
        if  (length[location]>1){
            totalCount++;
        }
        if (length[location]>=6){
            fprintf(fp2, "%s", data[location]);
        }
    }
    fclose(fp2);
    
    //Print total words and timings
    printf("Total count: %d\n", totalCount);
    printf("%lld microseconds occured\n",t);
    return 0;
}
