#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    int comm_sz;
    int my_rank;
    int r;
    int c;
    int* matrix = NULL;
    int* vector = NULL;
    int* resultVector = NULL;
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

    if (my_rank != 0) {
        matrix = (int*)malloc(r * c * sizeof(int));
        vector = (int*)malloc(c * sizeof(int));
        resultVector = (int*)malloc(r * sizeof(int));
    }

    MPI_Bcast(matrix, r * c, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, c, MPI_INT, 0, MPI_COMM_WORLD);

    int baseC = c / comm_sz;
    int extraC = c % comm_sz;

    int startC;
    int endC;

    if (my_rank < extraC) {
        startC = my_rank * (baseC + 1);
        endC = startC + baseC + 1;
    }
    else {
        startC = my_rank * baseC + extraC;
        endC = startC + baseC;
    }

    int* localResult = (int*)malloc(r * sizeof(int));
    for (int i = 0; i < r; i++) {
        localResult[i] = 0;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    startTime = MPI_Wtime();

    for (int i = 0; i < r; i++) {
        localResult[i] = 0;
    }
    for (int j = startC; j < endC; j++) {
        for (int i = 0; i < r; i++) {
            localResult[i] += matrix[i * c + j] * vector[j];
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