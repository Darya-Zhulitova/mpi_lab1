#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265358979323846

int main(int argc, char** argv) {
    long long n = 0;

    if (argc > 1) {
        n = atoll(argv[1]);
    }

    int my_rank;
	int comm_sz;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    srand(time(NULL) + my_rank);

    double mpi_start_time = MPI_Wtime();

    long long process_n = n / comm_sz;
    long long process_circle_points = 0;
    long long local_points = process_n;

	// Корректируем количество точек для последнего процесса (если n не делится нацело)
    if (my_rank == comm_sz - 1) {
        local_points = n - (comm_sz - 1) * process_n;
    }

    for (long long i = 0; i < local_points; i++) {
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            process_circle_points++;
        }
    }

    long long total_hits;
    MPI_Reduce(&process_circle_points, &total_hits, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double mpi_end_time = MPI_Wtime();
    double mpi_time = (mpi_end_time - mpi_start_time) * 1000;

    if (my_rank == 0) {
        srand(time(NULL));

        long long single_points = n;
        long long single_circle_points = 0;
        
        double single_start = MPI_Wtime();
        
        for (long long i = 0; i < single_points; i++) {
            double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
            double y = (double)rand() / RAND_MAX * 2.0 - 1.0;
            
            if (x * x + y * y <= 1.0) {
                single_circle_points++;
            }
        }
        
        double single_end = MPI_Wtime();
        double single_time = (single_end - single_start) * 1000;
        
        double pi_result = 4.0 * (double)total_hits / (double)n;
		double hit_ratio = (double)total_hits / n;
		double expected_ratio = PI / 4.0;
        double speedup_ratio = single_time / mpi_time;
        double mpi_efficiency = speedup_ratio / comm_sz;
        
        printf("Total points: %lld\n", n);
        printf("Points in circle: %lld\n", total_hits);
        printf("Hit ratio: %.10f\n", hit_ratio);
        printf("Expected ratio (pi/4): %.10f\n", expected_ratio);
        printf("Expiremental pi: %.10f\n", pi_result);
        printf("Single process time: %.6f ms\n", single_time);
        printf("Mpi time: %.6f ms\n", mpi_time);
        printf("Speedup ratio: %.6f\n", speedup_ratio);
        printf("Efficiency: %.6f\n", mpi_efficiency);
    }

    MPI_Finalize();
    return 0;
}