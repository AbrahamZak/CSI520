#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//Use these to change iterations and threads
#define NUM_THREADS 10
#define ITERATIONS 100

//globals 
extern int *numbers;
extern int numSize ;
//add More globals/etc below


//All functions
extern void GetNumbers(char fileName[]);

//You need to create all of the below functions
extern long long GetStackCount(); 
extern int Pop();
extern void Push(int number);
//extern void PrintAll(); // optional



