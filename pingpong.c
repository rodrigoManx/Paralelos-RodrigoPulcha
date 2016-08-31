#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int rank;
int size;
const int limit = 5;

int main(int argc, char** argv) {

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	if (size == 2) {
		int count = 0;
		int adversary = 1 - rank;
		while (count < limit) {
			if (rank == count % 2) {
				count++;
				MPI_Send(&count, 1, MPI_INT, adversary, 0, MPI_COMM_WORLD);
				printf("%d: contador incrementado a %d\n", rank, count);
			} 
			else {
		  		MPI_Recv(&count, 1, MPI_INT, adversary, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		  		printf("%d: incremento a %d\n", rank, count);
			}
		}
	}
	MPI_Finalize();
	return 0;
}