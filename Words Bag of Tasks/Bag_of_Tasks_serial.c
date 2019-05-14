//  homeworkFour_Part_1_Serial

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

struct timeval tm1, tm2;
long long t;


int main(int argc, const char * argv[]) {
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
    int length[count];
    int i;
    for (i=0; i<count; i++){
        length[i] = 0;
    }
    
    //Get the lines from the text file into a character array
    char (*data)[25] = malloc(sizeof(char [count][25]));
    int location = 0;
    
    while(fgets(line,sizeof(line),fp) != NULL){
        strcpy(data[location], line);
        location++;
    }
    fclose(fp);
    
    //Begin timing
    gettimeofday(&tm1, NULL);
    
    //From 0 -> count we check if each word has unique characters
    //If it does we set length[location] to the length of the word
    //We also skip non-words (anything starting w/ numbers/capital letters)
    for (location = 0; location<count; location++){
        //Get length of current word
        int len = (int)strlen(data[location]);
        len--;
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
        length[location] = len;
    }
    
    //Get the total time in microseconds and print
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
