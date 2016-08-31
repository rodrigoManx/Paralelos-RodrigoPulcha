#include <stdio.h>
#include <mpi.h>
#define NUM_ROWS_A 10
#define NUM_COLUMNS_A 2 
#define NUM_ROWS_B 2 
#define NUM_COLUMNS_B 6 

int rank; 
int size; 
int i, j, k; 
double matA[NUM_ROWS_A][NUM_COLUMNS_A]; 
double matB[NUM_ROWS_B][NUM_COLUMNS_B]; 
double matResult[NUM_ROWS_A][NUM_COLUMNS_B];
int lowBound; 
int upperBound; 
int portion;

void fillAB() {
    for (i = 0; i < NUM_ROWS_A; i++) 
        for (j = 0; j < NUM_COLUMNS_A; j++) 
            matA[i][j] = i + j;
    for (i = 0; i < NUM_ROWS_B; i++) 
        for (j = 0; j < NUM_COLUMNS_B; j++) 
            matB[i][j] = i*j;
}
void printOperation() {
    for (i = 0; i < NUM_ROWS_A; i++) {
        printf("\n");
        for (j = 0; j < NUM_COLUMNS_A; j++)
            printf("%8.2f  ", matA[i][j]);
    }
    printf("\n\n\t       * \n");
    for (i = 0; i < NUM_ROWS_B; i++) {
        printf("\n");
        for (j = 0; j < NUM_COLUMNS_B; j++)
            printf("%8.2f  ", matB[i][j]);
    }
    printf("\n\n\t       = \n");
    for (i = 0; i < NUM_ROWS_A; i++) {
        printf("\n");
        for (j = 0; j < NUM_COLUMNS_B; j++)
            printf("%8.2f  ", matResult[i][j]);
    }
    printf("\n\n");
}

int main(int argc, char *argv[]) {
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        fillAB();
        for (i = 1; i < size; i++) {
            portion = (NUM_ROWS_A / (size - 1)); 
            lowBound = (i - 1) * portion;
            if (((i + 1) == size) && ((NUM_ROWS_A % (size - 1)) != 0)) {
                upperBound = NUM_ROWS_A; 
            } else {
                upperBound = lowBound + portion; 
            }

            MPI_Send(&lowBound, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Send(&upperBound, 1, MPI_INT, i, 1, MPI_COMM_WORLD);

            MPI_Send(&matA[lowBound][0], (upperBound - lowBound) * NUM_COLUMNS_A, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
        }
    }

    MPI_Bcast(&matB, NUM_ROWS_B*NUM_COLUMNS_B, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank > 0) {

        MPI_Recv(&lowBound, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Recv(&upperBound, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Recv(&matA[lowBound][0], (upperBound - lowBound) * NUM_COLUMNS_A, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i = lowBound; i < upperBound; i++) {
            for (j = 0; j < NUM_COLUMNS_B; j++) {
                for (k = 0; k < NUM_ROWS_B; k++) {
                    matResult[i][j] += (matA[i][k] * matB[k][j]);
                }
            }
        }

        MPI_Send(&lowBound, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
      
        MPI_Send(&upperBound, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
      
        MPI_Send(&matResult[lowBound][0], (upperBound - lowBound) * NUM_COLUMNS_B, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        for (i = 1; i < size; i++) {

            MPI_Recv(&lowBound, 1, MPI_INT, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Recv(&upperBound, 1, MPI_INT, i, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Recv(&matResult[lowBound][0], (upperBound - lowBound) * NUM_COLUMNS_B, MPI_DOUBLE, i, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printOperation();
    }
    MPI_Finalize();
    return 0;
}
