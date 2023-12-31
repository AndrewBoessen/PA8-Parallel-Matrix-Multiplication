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
void matmul(const double * const a, const double * const b, double * const result, int start, int n, int m) {
    // General matrix multiplication for n x m matrix

    for (int i = start; i < n; i++) {
        for (int j = 0; j < m; j++) {
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

int verify(const double * const m1, double *m2, int dim) {
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

void multiply_serial(const double * const a, const double * const b, double * const c, const int dim, const int num_workers) {
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
void multiply_chunk(const double * const a, const double * const b, double * const c, int dim, int row_start, int chunk_size) {
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

void multiply_parallel_processes(const double * const a, const double * const b, double * const c, const int dim, const int num_workers) {
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
            multiply_chunk(a, b, result_matrix, dim, row_start, chunk_size);
            exit(EXIT_SUCCESS);
        }
        row_start += chunk_size;
    }

    // Parent process
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

/* Thread Based Implementation */
void *task(void * arg) {
    Args * args = (Args *) arg;
    multiply_chunk(args->a, args->b, args->c, args->dim, args->row_start, args->chunk_size);
    return NULL;
}

void multiply_parallel_threads(const double * const a, const double * const b, double * const c, const int dim, const int num_workers) {
    int num_threads = num_workers - 1;
    pthread_t threads[num_threads];
    Args arg_set[num_workers];

    int chunk_size = dim / num_workers;
    int row_start = 0;

    // Initialize thread arguments
    for (int i = 0; i < num_workers; ++i) {
        arg_set[i].a = (double *) a;
        arg_set[i].b = (double *) b;
        arg_set[i].c = c;
        arg_set[i].dim = dim;
        arg_set[i].chunk_size = chunk_size;
    }

    // Create threads
    for (int i = 0; i < num_threads; ++i) {
        arg_set[i].row_start = row_start;
        pthread_create(&threads[i], NULL, task, (void *)&arg_set[i]);
        row_start += chunk_size;
    }

    // Main thread's task
    arg_set[num_threads].row_start = row_start;
    arg_set[num_threads].chunk_size = dim - row_start;
    task((void *)&arg_set[num_threads]);

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
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

void print_elapsed_time(struct timeval *start, struct timeval *end, const char * const legend) {
    struct timeval elapsed = time_diff(start, end);
    printf("Time elapsed for %s: %ld second%s, %ld microsecond%s\n",
           legend,
           (long)elapsed.tv_sec, (elapsed.tv_sec == 1) ? "" : "s",
           (long)elapsed.tv_usec, (elapsed.tv_usec == 1) ? "" : "s");
}

void print_verification(const double * const m1, double * const m2, const int dim, const char * const name) {
    int result = verify(m1, m2, dim);
    printf(
        "Verification for %s: %s\n", 
        name, 
        result == SUCCESS ? "success" : "failed"
    );
}

/* Run and Time */
void run_and_time(
    multiply_function multiply_matrices,
    const double * const a,
    const double * const b,
    double * const c,
    const double * const gold,
    const int dim,
    const char * const name,
    const int num_workers,
    const bool verify
) {
    struct timeval start, end;

    printf(
        "Algorithm: %s with %d worker%s\n", 
        name, 
        num_workers, num_workers == 1 ? "" : "s"
    );

    gettimeofday(&start, NULL);
    multiply_matrices(a, b, c, dim, num_workers);
    gettimeofday(&end, NULL);

    print_elapsed_time(&start, &end, name);

    if (verify) {
        print_verification(gold, c, dim, name);
    }
}
