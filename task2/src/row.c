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
    int* sendCount = NULL;
    int* sendDisplacements = NULL;
    int* recvCounts = NULL;
    int* recvDisplacements = NULL;
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

        sendCount = (int*)malloc(comm_sz * sizeof(int));
        sendDisplacements = (int*)malloc(comm_sz * sizeof(int));
        recvCounts = (int*)malloc(comm_sz * sizeof(int));
        recvDisplacements  = (int*)malloc(comm_sz * sizeof(int));

        int baseR = r / comm_sz;
        int extraR = r % comm_sz;
        int currentDisplacement = 0;
        for (int i = 0; i < comm_sz; i++) {
            int rowsForProc = baseR + (i < extraR ? 1 : 0);

            sendCount[i] = rowsForProc * c;
            sendDisplacements [i] = currentDisplacement  * c;

            recvCounts[i] = rowsForProc;
            recvDisplacements [i] = currentDisplacement ;

            currentDisplacement += rowsForProc;
        }
    }

    MPI_Bcast(&r, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&c, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int localR;
    if (my_rank == 0) {
        for (int i = 0; i < comm_sz; i++) {
            if (i == my_rank) localR = recvCounts[i];
        }
    }

    MPI_Scatter(recvCounts, 1, MPI_INT, &localR, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int* localMatrix = (int*)malloc(localR * c * sizeof(int));
    int* localResult = (int*)malloc(localR * sizeof(int));

    if (my_rank != 0) {
        vector = (int*)malloc(c * sizeof(int));
    }

    MPI_Scatterv(matrix, sendCount, sendDisplacements , MPI_INT, localMatrix, localR * c, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(vector, c, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    startTime = MPI_Wtime();

	for (int i = 0; i < localR; i++) {
        localResult[i] = 0;
        for (int j = 0; j < c; j++) {
			localResult[i] += localMatrix[i * c + j] * vector[j];
        }
    }

    endTime = MPI_Wtime();
    localComputationTime = endTime - startTime;

    MPI_Gatherv(localResult, localR, MPI_INT, resultVector, recvCounts, recvDisplacements  , MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localComputationTime, &maxComputationTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("%f ms\n", maxComputationTime * 1000.0);
    }

	free(matrix);
	free(resultVector);
	free(sendCount);
	free(sendDisplacements);
	free(recvCounts);
	free(recvDisplacements);
    free(localMatrix);
    free(vector);
    free(localResult);
    MPI_Finalize();
    return 0;
}