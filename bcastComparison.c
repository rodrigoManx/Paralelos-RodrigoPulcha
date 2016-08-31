#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <math.h>

typedef enum { false, true } bool;

bool isPowerOfTwo (int x) {
    return x && (!(x&(x-1)));
}

void RAP_Bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {
	int i;
	int rank;
	MPI_Comm_rank(communicator, &rank);
  	int size;
  	MPI_Comm_size(communicator, &size);
	if(root != 0) {
		if(rank == root)
			MPI_Send(data, count, datatype, 0, 0, MPI_COMM_WORLD);
		else if(rank == 0)
			MPI_Recv(data, count, datatype, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if(rank > 0) {
		if(rank % 2 != 0)
			MPI_Recv(data, count, datatype, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		else {
			int exp = log2(rank), lowBound = pow(2,exp), upperBound = pow(2,exp+1);
			//printf("%d\t,%d\t,%d\t,%d\n",rank,exp,lowBound,upperBound);
			if(isPowerOfTwo(rank))
				MPI_Recv(data, count, datatype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			else 
				MPI_Recv(data, count, datatype, lowBound, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			for (i = 1; rank + i < upperBound && rank + i < size; i*=2) {
					MPI_Send(data, count, datatype, rank + i, 0, MPI_COMM_WORLD);
				}
		}
	}
	else {
		for (i = 1; i < size; i*=2)
			MPI_Send(data, count, datatype, i, 0, MPI_COMM_WORLD);
	}
}

void my_bcast(void* data, int count, MPI_Datatype datatype, int root,
              MPI_Comm communicator) {
  int world_rank;
  MPI_Comm_rank(communicator, &world_rank);
  int world_size;
  MPI_Comm_size(communicator, &world_size);

  if (world_rank == root) {

    int i;
    for (i = 0; i < world_size; i++) {
      if (i != world_rank) {
        MPI_Send(data, count, datatype, i, 0, communicator);
      }
    }
  } else {

    MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
  }
}

int main(int argc, char** argv) {
    if (argc != 3) {
      fprintf(stderr, "Usage: compare_bcast num_elements num_trials\n");
      exit(1);
    }

  int num_elements = atoi(argv[1]);
  int num_trials = atoi(argv[2]);

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  double total_my_bcast_time = 0.0;
  double total_mpi_bcast_time = 0.0;
  double total_rap_bcast_time = 0.0;
  int i;
  int* data = (int*)malloc(sizeof(int) * num_elements);
  assert(data != NULL);

  for (i = 0; i < num_trials; i++) {
      
    MPI_Barrier(MPI_COMM_WORLD);
    total_my_bcast_time -= MPI_Wtime();
    my_bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    total_my_bcast_time += MPI_Wtime();
    
    
    MPI_Barrier(MPI_COMM_WORLD);
    total_mpi_bcast_time -= MPI_Wtime();
    MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    total_mpi_bcast_time += MPI_Wtime();


    MPI_Barrier(MPI_COMM_WORLD);
    total_rap_bcast_time -= MPI_Wtime();
    RAP_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    total_rap_bcast_time += MPI_Wtime();

  }


  if (world_rank == 0) {
    printf("Data size = %d, Trials = %d\n", num_elements * (int)sizeof(int),
           num_trials);
    printf("Avg my_bcast time = %lf\n", total_my_bcast_time / num_trials);
    printf("Avg MPI_Bcast time = %lf\n", total_mpi_bcast_time / num_trials);
    printf("Avg RAP_Bcast time = %lf\n", total_rap_bcast_time / num_trials);
  }

  MPI_Finalize();
}