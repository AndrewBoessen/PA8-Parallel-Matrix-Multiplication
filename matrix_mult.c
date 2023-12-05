/*
 * Author: Andrew Boessen - boessena@bc.edu
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "matrix_mult.h"

/* Helper Functions */
void matmul(double *a, double *b, double * result, int start, int n, int m) {
    // General matrix multiplication for n x m matrix

    for (int i = start; i < n; i++) {
        for (int j = 0; j < m; j++) {
            result[i * m + j] = 0.0;
            for (int k = 0; k < m; k++) {
                result[i * m + j] += a[i * m + k] * b[k * m + j];
            }
        }
    }
}

void print_matrix(double *matrix, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%f", matrix[i * dim + j]);

            // Print space between columns except for the last column
            if (j < dim - 1) {
                printf(" ");
            }
        }

        // Print a new line after each row
        printf("\n");
    }
}

int verify(double *m1, double *m2, int dim) {
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            if (fabs(m1[i * dim + j] - m2[i * dim + j]) > 1e-9) {
                // Elements do not match
                return FAILURE;
            }
        }
    }
    // All elements match
    return SUCCESS;
}

/* Serial Matrix Multiplication */
void init_matrix(double * matrix, int dim) {
    int k = 1;

    for (int i = 0; i < dim * dim; i++) {
        matrix[i] = k;
        k++;
    }
}

void multiply_serial(double *a, double *b, double *c, int dim) {
    // Assuming c is initialized to 0.0 using calloc

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            for (int k = 0; k < dim; k++) {
                c[i * dim + j] += a[i * dim + k] * b[k * dim + j];
            }
        }
    }
}

/* Process Based Parallel Implementation */
void multiply_chunk(double *a, double *b, double *c, int dim, int row_start, int chunk_size) {
    matmul(a, b, c, row_start, row_start + chunk_size, dim);
}

void mmap_checked(void **addr, size_t length, int prot, int flags, int fd, off_t offset) {
    *addr = mmap(NULL, length, prot, flags, fd, offset);
    if (*addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
}

void munmap_checked(void *addr, size_t length) {
    if (munmap(addr, length) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
}

pid_t fork_checked(void) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return pid;
}

void multiply_parallel_processes(double *a, double *b, double *c, int dim, int num_workers) {
    int chunk_size = dim / num_workers;
    int row_start = 0;
    size_t matrix_size = dim * dim * sizeof(double);

    // Memory map for the result matrix
    double *result_matrix;
    mmap_checked((void **)&result_matrix, matrix_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Spawn child processes
    for (int i = 0; i < num_workers - 1; ++i) {
        pid_t pid = fork_checked();

        if (pid == 0) {
            // Child process
            row_start += chunk_size;
            multiply_chunk(a, b, result_matrix, dim, row_start, chunk_size);
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process
    printf("Parent row_start: %d\n", row_start);
    multiply_chunk(a, b, result_matrix, dim, row_start, dim - row_start);
    
    // Wait for all child processes to finish
    while (wait(NULL) != -1) {
        // Continue waiting until all child processes have finished
    }

    // Copy result back to the original matrix c
    for (int i = 0; i < dim * dim; ++i) {
        c[i] = result_matrix[i];
    }
    
    // Unmap the shared memory
    munmap_checked(result_matrix, matrix_size);
}

/* Timing and Printing Results */
struct timeval time_diff(struct timeval *start, struct timeval *end) {
    struct timeval result;
    result.tv_sec = end->tv_sec - start->tv_sec;
    result.tv_usec = end->tv_usec - start->tv_usec;

    // Adjust if the microseconds are negative
    if (result.tv_usec < 0) {
        result.tv_sec--;
        result.tv_usec += 1000000;
    }

    return result;
}

void print_elapsed_time(struct timeval *start, struct timeval *end, char * legend) {
    struct timeval elapsed = time_diff(start, end);
    printf("%s: %ld second%s, %ld microsecond%s\n",
           legend,
           (long)elapsed.tv_sec, (elapsed.tv_sec == 1) ? "" : "s",
           (long)elapsed.tv_usec, (elapsed.tv_usec == 1) ? "" : "s");
}

void print_verification(double *m1, double *m2, int dim, char * name) {
    int result = verify(m1, m2, dim);
    if (result == SUCCESS) {
        printf("%s: Verification Passed\n", name);
    } else {
        printf("%s: Verification Failed\n", name);
    }
}
