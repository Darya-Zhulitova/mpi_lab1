#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char** argv) {
    int comm_sz;
    int my_rank;
    int r;
    int c;
    int* matrix = NULL;
    int* vector = NULL;
    int* resultVector = NULL;
    int* localResult = NULL;
    double startTime;
    double endTime;
    double localComputationTime;
    double maxComputationTime;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	srand(time(0));

    if (my_rank == 0) {
		r = atoll(argv[1]);
		c = atoll(argv[2]);

        matrix = (int*)malloc(r * c * sizeof(int));
        vector = (int*)malloc(c * sizeof(int));
        resultVector = (int*)malloc(r * sizeof(int));

        for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				matrix[i * c + j] = rand() % 10;
			}
		}
        for (int i = 0; i < c; i++) {
			vector[i] = rand() % 10;
		}

        for (int i = 0; i < r; i++) {
            resultVector[i] = 0;
        }
    }

    MPI_Bcast(&r, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&c, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int blockL = (int)sqrt(comm_sz);
    int blockN = blockL * blockL;
    int blockR = r / blockL;
    int blockC = c / blockL;

    if (my_rank != 0) {
        matrix = (int*)malloc(r * c * sizeof(int));
        vector = (int*)malloc(c * sizeof(int));
        resultVector = (int*)malloc(r * sizeof(int));
    }

    localResult = (int*)malloc(r * sizeof(int));
    for (int i = 0; i < r; i++) {
        localResult[i] = 0;
    }

    MPI_Bcast(matrix, r * c, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, c, MPI_INT, 0, MPI_COMM_WORLD);

    int startR = (my_rank / blockL) * blockR;
    int endR = startR + blockR;
    int startC = (my_rank % blockL) * blockC;
    int endC = startC + blockC;

    if (my_rank < blockN) {
        if (my_rank == blockN - 1) {
            endR = r;
            endC = c;
        }
        else if ((my_rank + 1) % blockL == 0) {
            endC = c;
        }
        else if ((my_rank + 1) > blockN - blockL) {
            endR = r;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    startTime = MPI_Wtime();

    if (my_rank < blockN) {
		for (int i = startR; i < endR; i++) {
			for (int j = startC; j < endC; j++) {
				localResult[i] += matrix[i * c + j] * vector[j];
			}
		}
    }

    endTime = MPI_Wtime();
    localComputationTime = endTime - startTime;

    MPI_Reduce(localResult, resultVector, r, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localComputationTime, &maxComputationTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("%f ms\n", maxComputationTime * 1000.0);
    }

    free(matrix);
    free(vector);
    free(resultVector);
    free(localResult);
    MPI_Finalize();
    return 0;
}