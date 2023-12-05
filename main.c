/*
 * main.c
 * Driver for demonstration of parallelized matrix multiplication.
 * Author: Andrew Boessen - boessena@bc.edu
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "matrix_mult.h"

typedef struct RunArgs {
    multiply_function func;
    double * product;
    const int num_workers;
    const char * const name;
    const bool verify;
} RunArgs;

struct timeval start, end;

int main() {
    int size = DIM * DIM;
    double * matrix_a = calloc(size, sizeof(double));
    double * matrix_b = calloc(size, sizeof(double));
    // Result matricies
    double * matrix_c = calloc(size, sizeof(double));
    double * matrix_d = calloc(size, sizeof(double));
    double * matrix_e = calloc(size, sizeof(double));
    // Init product matricies
    init_matrix(matrix_a, DIM);
    init_matrix(matrix_b, DIM);

    puts("Comparing Serial vs. Processes vs. Threads");

    gettimeofday(&start, NULL);
    multiply_serial(matrix_a, matrix_b, matrix_c, DIM, 1);
    gettimeofday(&end, NULL);
    print_elapsed_time(&start, &end, "Serial");

    // Processes
    gettimeofday(&start, NULL);
    multiply_parallel_processes(matrix_a, matrix_b, matrix_d, DIM, NUM_WORKERS);
    gettimeofday(&end, NULL);
    print_elapsed_time(&start, &end, "Multiprocessing");

    print_verification(matrix_c, matrix_d, DIM, "Multiprocessing");

    // Threads
    gettimeofday(&start, NULL);
    multiply_parallel_threads(matrix_a, matrix_b, matrix_e, DIM, NUM_WORKERS);
    gettimeofday(&end, NULL);
    print_elapsed_time(&start, &end, "Threads");

    print_verification(matrix_c, matrix_e, DIM, "Threads");

    return EXIT_SUCCESS;
}

