#include <mpi.h> 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv)
{
     MPI_Init(&argc,&argv);
     const int SIZE = atoi(argv[1]);
     int size, rank;

     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     MPI_Comm_size(MPI_COMM_WORLD, &size);

     if (SIZE % size != 0) {
       perror("Matrix is not divisible by number of processes.");
       return 1;
     }

     int n_local = SIZE / size;

     int a[SIZE][SIZE], b[SIZE], cpart[n_local], ctotal[SIZE];

     /*
     for (int i = 0; i < SIZE; i++) {
       b[i] = 3;
       for(int j = 0; j < SIZE; j++)
         a[i][j] = 2;
         }*/

     double local_start, local_finish, local_elapsed, elapsed;
     MPI_Barrier(MPI_COMM_WORLD);
     local_start = MPI_Wtime();

     for(int i = rank*n_local; i < (rank+1)*n_local; i++)
     {
         cpart[i-rank*n_local]=0;
         for(int k = 0; k < SIZE; k++)
         {
           cpart[i-rank*n_local] += a[i][k] * b[k];
         }
     }

     MPI_Allgather(cpart, n_local, MPI_INT, ctotal, n_local, MPI_INT,
                    MPI_COMM_WORLD);

     local_finish = MPI_Wtime();
     local_elapsed = local_finish - local_start;
     MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0,
                 MPI_COMM_WORLD);

     if (rank == 0) {
       /* for (int i = 0; i < SIZE; i++)
         printf("%i ", ctotal[i]);
         printf("\n"); */
       printf("Elapsed time: %e milliseconds\n", elapsed * 1000);
     }

     MPI_Finalize();
     return 0;
}