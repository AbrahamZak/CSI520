//  homeworkEight_Part_1_Distributed

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <limits.h>
#include <mpi.h>
#define DEBUG 1

struct timeval tm1, tm2;
long long t;

//Struct containing cell information and neighbors
typedef struct Cell {
    char state;
    char up;
    char down;
    char left;
    char right;
    char upLeft;
    char upRight;
    char downLeft;
    char downRight;
} CELL;

void Simulate(int G, int p, int rank, int cols, int rows, CELL localRow[rows][cols], MPI_Status status);
CELL DetermineState(CELL cell);
void DisplayGrid(int p, int rank, int rows, int cols, CELL localRow[rows][cols], MPI_Status status);
int mod(int x, int y);

int main(int argc, char * argv[]) {
    int rank, p, i, j, localSeed;
    MPI_Status status;
    
    //get rank and processes
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    //Randomizer ssed
    srand((unsigned int)time(NULL));
    
    //Matrix size m x n
    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    //Number of iterations K
    int K = atoi(argv[3]);
    
    int globalSeeds[p];
    
    if (rank == 0) {
        //Begin timing
        gettimeofday(&tm1, NULL);
        // populate seeds with random numbers
        for (i = 0; i < p; i++) {
            globalSeeds[i] = rand() % INT_MAX + 1;
        }
    }
    
    //Send seeds to other ranks
    MPI_Scatter(&globalSeeds, 1, MPI_INT, &localSeed, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    //Create the local matrix
    int localMatrix[M][N];
    srand(localSeed);
    
    //Give each cell a random number
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            localMatrix[i][j] = rand() % INT_MAX + 1;
        }
    }
    
    //Get status of each cell, even is alive, odd is dead
    CELL localRow[M][N];
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            if (localMatrix[i][j] % 2 == 0) {
                localRow[i][j].state = 'A';
            }
            else {
                localRow[i][j].state = 'D';
            }
        }
    }
    //Simulate for K times
    Simulate(K, p, rank, M, N, localRow, status);
   
    //If rank is 0 get the final timing and print
    if (rank==0){
        //Get the total time in microseconds and print
        gettimeofday(&tm2, NULL);
        t = (tm2.tv_sec - tm1.tv_sec) * 1000000 + (tm2.tv_usec - tm1.tv_usec);
        printf("%lld microseconds occured\n",t);
    }
    MPI_Finalize();
    return 0;
}

//Runs each generation and update matrix
void Simulate(int K, int p, int rank, int rows, int cols, CELL localRow[rows][cols], MPI_Status status) {
    int i = 0, j = 0;
    char top[255], bottom[255], row[255];
    char rTop[255], rBottom[255], rRow[255];
    
    i = 0;
    while (i < K) {
        //See neighbors
        for (i = 0; i < cols; i++) {
            top[i] = localRow[0][i].state;
            bottom[i] = localRow[rows - 1][i].state;
        }
        //Send top row up
        MPI_Send(top, cols + 1, MPI_CHAR, mod(rank - 1, p), 0, MPI_COMM_WORLD);
        //Send bottom row down
        MPI_Send(bottom, cols + 1, MPI_CHAR, mod(rank + 1, p), 1, MPI_COMM_WORLD);
        //Recieve bottom row from down
        MPI_Recv(rBottom, cols + 1, MPI_CHAR, mod(rank + 1, p), 0, MPI_COMM_WORLD, &status);
        //Recieve top row from up
        MPI_Recv(rTop, cols + 1, MPI_CHAR, mod(rank - 1, p), 1, MPI_COMM_WORLD, &status);
        
        
        //Assign neighbors
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                //If we're on the first row
                if (i == 0) {
                    localRow[0][j].up = rTop[j];
                    localRow[0][j].down = localRow[1][j].state;
                }
                //If on last row
                else if (i == rows - 1) {
                    localRow[rows - 1][j].down = rBottom[j];
                    localRow[rows - 1][j].up = localRow[rows - 2][j].state;
                }
                //If we're on any other row
                else{
                    
                    localRow[i][j].up = localRow[i - 1][j].state;
                    localRow[i][j].down = localRow[i + 1][j].state;
                }
                localRow[i][j].left = localRow[i][mod(j - 1, cols)].state;
                localRow[i][j].right = localRow[i][mod(j + 1, cols)].state;
            }
        }
        
        //Assign diagnol neighbors
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                localRow[i][j].upLeft = localRow[i][mod(j - 1, cols)].up;
                localRow[i][j].upRight = localRow[i][mod(j + 1, cols)].up;
                localRow[i][j].downLeft = localRow[i][mod(j - 1, cols)].down;
                localRow[i][j].downRight = localRow[i][mod(j + 1, cols)].down;
            }
        }
        
        //Iterate through all cells and determine the state
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                localRow[i][j] = DetermineState(localRow[i][j]);
            }
        }
        i++;
        //Display the result after changed
        DisplayGrid(p, rank, rows, cols, localRow, status);
    }
}

//Display the grid
void DisplayGrid(int p, int rank, int rows, int cols, CELL localRow[rows][cols], MPI_Status status) {
    int i = 0, j = 0;
    
    //Send all the grids to the root
    if (rank == 0) {
        //Create a master and add everything to it
        char masterMatrix[rows * p][cols];
        for (i = 0; i < rows; i++){
            for (j = 0; j < cols; j++){
                masterMatrix[i][j] = localRow[i][j].state;
            }
        }
        
        j = 1;
        //Recieving for master
        for (i = rows; i < rows * p; i += rows) {
            MPI_Recv(&masterMatrix[i][0], rows * cols, MPI_CHAR, j++, 0,MPI_COMM_WORLD, &status);
        }
        
        //Print the master
        for (i = 0; i < rows * p; i++){
            for (j = 0; j < cols; j++){
                printf("%c ", masterMatrix[i][j]);
            }
            printf("\n");
        }
    }
    else {
        char localMatrix[rows][cols];
        //Make an array of the states
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                localMatrix[i][j] = localRow[i][j].state;
            }
        }
        MPI_Send(&localMatrix, rows * cols, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
}

//Determine state of a new cell
CELL DetermineState(CELL cell) {
    //Count how many neighbors are alive
    int A_Count = 0;
    if (cell.up == 'A') A_Count++;
    if (cell.down == 'A') A_Count++;
    if (cell.left == 'A') A_Count++;
    if (cell.right == 'A') A_Count++;
    if (cell.upLeft == 'A') A_Count++;
    if (cell.upRight == 'A') A_Count++;
    if (cell.downLeft == 'A') A_Count++;
    if (cell.downRight == 'A') A_Count++;
    
    //If the cell is alive and has 2 or 3 living neighbors, it continues living
    if (cell.state == 'A') {
        if (A_Count == 2 || A_Count == 3) {
            return cell;
        }
        //If not, it dies
        else {
            cell.state = 'D';
        }
    }
    //If the cell is dead and there are more than three living neighbors, it lives
    else {
        if (A_Count >= 3) {
            cell.state = 'A';
        }
    }
    return cell;
}

//Gets the modulus
int mod(int x, int y)
{
    int r = x % y;
    return r < 0 ? r + y : r;
}
