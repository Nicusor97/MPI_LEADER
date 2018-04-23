#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MASTER 0

int main(int argc, char** argv) {

	int numprocs, rank, len;
	int value = 0;
	int id_pairs[2] = { 0, 0 };
	int recv_value_id_pair[2] = { 0, 0 };
	int max_value_id_pair[2] = { 0, 0 };

	char hostname[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(hostname, &len);
	MPI_Status status;


	srand(time(NULL) + rank);
	value = rand() % numprocs;
	printf("Process %d generated number: %d \n", rank, value);
	id_pairs[0] = value;
	id_pairs[1] = rank;
	for (int i = 0; i < numprocs; i++)
	{
		if (i != rank)
		{
			MPI_Send(&id_pairs, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Recv(&recv_value_id_pair, 2, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			if (max_value_id_pair[0] < recv_value_id_pair[0])
			{
				max_value_id_pair[0] = recv_value_id_pair[0];
				max_value_id_pair[1] = recv_value_id_pair[1];
			}
			else if (max_value_id_pair[0] == recv_value_id_pair[0])
			{
				if (max_value_id_pair[1] < recv_value_id_pair[1])
				{
					max_value_id_pair[0] = recv_value_id_pair[0];
					max_value_id_pair[1] = recv_value_id_pair[1];
				}
			}
		}
	}

	if (rank == 0)
	{
		printf("Leader is process %d with the value: %d", max_value_id_pair[1], max_value_id_pair[0]);
	}

	MPI_Finalize();




}