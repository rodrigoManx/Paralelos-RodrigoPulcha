#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 4           /* Size of matrices */
#define MAX_RAND     100

int A[SIZE][SIZE], B[SIZE][1], C[SIZE][1];//D[SIZE][SIZE],E[SIZE][1];

void fill_matrix(int m[SIZE][SIZE])
{
    int i, j;
    for (i=0; i<SIZE; i++)
    {
        for (j=0; j<SIZE; j++){     
            m[i][j] = 1;//rand() % MAX_RAND;
        }
    }
}

void invert_matrix(int m[SIZE][SIZE])
{
    int i, j, n[SIZE][SIZE];
    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++){
            n[i][j] = m[i][j];
        }
    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++){
            m[i][j] = n[j][i];
        }
}

void fill_vector(int m[SIZE][1])
{
    int i, j;
    for (i=0; i<SIZE; i++)
    {
        for (j=0; j<1; j++){     
            m[i][j] = 1;//rand() % MAX_RAND;
        }
    }
}


void print_matrix(int m[SIZE][SIZE])
{
    int i, j = 0;
    for (i=0; i<SIZE; i++) {
        printf("\n\t| ");
        for (j=0; j<SIZE; j++)
            printf("%2d ", m[i][j]);
        printf("|");
    }
}




void print_vector(int m[SIZE][1])
{
    int i, j = 0;
    for (i=0; i<SIZE; i++) {
        printf("\n\t| ");
        for (j=0; j<1; j++)
            printf("%2d ", m[i][j]);
        printf("|");
    }
}


int main(int argc, char *argv[])
{
    int myrank, P, from, to, i, j, k, *a, *recvcounts, *res;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);   /* who am i */
    MPI_Comm_size(MPI_COMM_WORLD, &P); /* number of processors */


    if (SIZE%P!=0) {
        if (myrank==0) printf("Matrix size not divisible by number of processors\n");
        MPI_Finalize();
        exit(-1);
    }
    k = SIZE/P;
    from = myrank * k;
    to = ((myrank+1) * k);

    a = (int *)malloc(k * sizeof(int) );
    for (i=0; i<k; i++)
        a[i] = 0;
    recvcounts = (int *)malloc( P * sizeof(int) );
    for (i=0; i<P; i++)
        recvcounts[i] = 0;
    recvcounts[0] = SIZE;

    res = (int *)malloc( SIZE * sizeof(int) );
    for (i=0; i<SIZE; i++)
        res[i] = 0;

    if (myrank==0) {
        fill_matrix(A);
        fill_vector(B);
        invert_matrix(A);
    }



    MPI_Bcast (B, SIZE*1, MPI_INT, 0, MPI_COMM_WORLD);
    if(myrank==0){
        MPI_Scatter (&A[0][0], SIZE*SIZE/P, MPI_INT, MPI_IN_PLACE, SIZE*SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);
    }else{
        MPI_Scatter (&A[0][0], SIZE*SIZE/P, MPI_INT, &A[from][0], SIZE*SIZE/P, MPI_INT, 0, MPI_COMM_WORLD);
    }


    printf("computing slice %d (from row %d to %d)\n", myrank, from, to-1);
    for (i=0; i<SIZE; i++) 
        for (j=from; j<to; j++) {
            C[i][0] += A[j][i]*B[i][0];
        }

    MPI_Reduce_scatter(&C[0][0], res, recvcounts, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    
    if (myrank == 0)
    {
        //print_vector(C);
        for (i = 0; i < SIZE; ++i)
        {
            printf("%i\n", res[i]);
        }
    }
    

    MPI_Finalize();
    return 0;
}
