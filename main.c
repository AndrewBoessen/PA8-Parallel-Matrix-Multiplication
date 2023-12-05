/*
 * main.c
 * Driver for demonstration of parallelized matrix multiplication.
 * Author: Amittai Aviram - aviram@bc.edu
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

    //puts("\nMatrix A");
    //print_matrix(matrix_a, DIM);

    //puts("\nMatrix B");
    //print_matrix(matrix_b, DIM);

    gettimeofday(&start, NULL);
    multiply_serial(matrix_a, matrix_b, matrix_c, DIM, 1);
    gettimeofday(&end, NULL);
    print_elapsed_time(&start, &end, "Serial");

    //puts("\nResult");

    //print_matrix(matrix_c, DIM);

    gettimeofday(&start, NULL);
    multiply_parallel_processes(matrix_a, matrix_b, matrix_d, DIM, NUM_WORKERS);
    gettimeofday(&end, NULL);
    print_elapsed_time(&start, &end, "Multiprocessing");

    //puts("\nResult: Multiprocessing");
    //print_matrix(matrix_d, DIM);

    print_verification(matrix_c, matrix_d, DIM, "Multiprocessing");

    gettimeofday(&start, NULL);
    multiply_parallel_threads(matrix_a, matrix_b, matrix_e, DIM, NUM_WORKERS);
    gettimeofday(&end, NULL);
    print_elapsed_time(&start, &end, "Threads");

    print_verification(matrix_c, matrix_e, DIM, "Threads");

    return EXIT_SUCCESS;
}

