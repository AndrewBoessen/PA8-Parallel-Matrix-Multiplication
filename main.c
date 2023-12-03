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


int main() {
    int size = DIM * DIM;
    double * matrix_a = calloc(size, sizeof(double));
    double * matrix_b = calloc(size, sizeof(double));
    double * matrix_c = calloc(size, sizeof(double));
    double * matrix_d = calloc(size, sizeof(double));
    init_matrix(matrix_a, DIM);
    init_matrix(matrix_b, DIM);

    puts("\nMatrix A");
    print_matrix(matrix_a, DIM);

    puts("\nMatrix B");
    print_matrix(matrix_b, DIM);

    multiply_serial(matrix_a, matrix_b, matrix_c, DIM);

    puts("\nResult");
    print_matrix(matrix_c, DIM);

    multiply_parallel_processes(matrix_a, matrix_b, matrix_d, DIM, NUM_WORKERS);
    puts("\nResult: Multiprocessing");
    print_matrix(matrix_d, DIM);

    printf("\nVerification: %s\n", verify(matrix_c, matrix_d, DIM) == 0 ? "SUCCESS" : "FAILURE");

    return EXIT_SUCCESS;
}

