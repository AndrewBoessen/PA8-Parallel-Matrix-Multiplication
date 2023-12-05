# Matrix Multiplication with Parallel Computing

This project showcases the performance benefits of parallel computing through matrix multiplication. Three algorithms are implemented and compared for matrix multiplication: serial, parallel using concurrent separate processes, and parallel using threads within the same process.

![Parallel Matrix Multiplication](https://github.com/AndrewBoessen/PA8-Parallel-Matrix-Multiplication/blob/main/assets/parallel_matmul.jpg)

## File Structure

- **matrix_mult.c:** Main source code file.
- **matrix_mult.h:** Contains exported symbolic constant definitions, type definitions, and forward declarations.
- **main.c:** Executable entry-point function `main` for testing purposes.

# Matrix Multiplication Library Manual

matrix_multiplication - Library for parallel matrix multiplication in C
```c
#include "matrix_multiplication.h"
```

## Description

The `matrix_multiplication` library provides functions for parallel matrix multiplication in C. It includes implementations using parallel processes, parallel threads, and a serial approach. The library is designed to perform matrix multiplication efficiently, especially for large matrices.

## Functions

### `void multiply_parallel_processes(double *a, double *b, double *c, int dim, int num_workers);`

Performs matrix multiplication using parallel processes.

- `a`: Pointer to the first input matrix (A).
- `b`: Pointer to the second input matrix (B).
- `c`: Pointer to the result matrix (C).
- `dim`: Dimension of the matrices.
- `num_workers`: Number of parallel processes to use.

### `void multiply_parallel_threads(double *a, double *b, double *c, int dim, int num_workers);`

Performs matrix multiplication using parallel threads.

- `a`: Pointer to the first input matrix (A).
- `b`: Pointer to the second input matrix (B).
- `c`: Pointer to the result matrix (C).
- `dim`: Dimension of the matrices.
- `num_workers`: Number of parallel threads to use.

### `void multiply_serial(double *a, double *b, double *c, int dim, int num_workers);`

Performs matrix multiplication using a serial approach.

- `a`: Pointer to the first input matrix (A).
- `b`: Pointer to the second input matrix (B).
- `c`: Pointer to the result matrix (C).
- `dim`: Dimension of the matrices.
- `num_workers`: Number of parallel processes or threads (not used in the serial version).

### `void init_matrix(double *matrix, int dim);`

Initializes a matrix with random values.

- `matrix`: Pointer to the matrix to be initialized.
- `dim`: Dimension of the matrix.

### `void print_matrix(double *matrix, int dim);`

Prints the values of a matrix.

- `matrix`: Pointer to the matrix to be printed.
- `dim`: Dimension of the matrix.

### `int verify(double *m1, double *m2, int dim);`

Verifies if two matrices are equal.

- `m1`: Pointer to the first matrix.
- `m2`: Pointer to the second matrix.
- `dim`: Dimension of the matrices.

### `void print_elapsed_time(struct timeval *start, struct timeval *end, char *legend);`

Prints the elapsed time between two time values.

- `start`: Pointer to the start time.
- `end`: Pointer to the end time.
- `legend`: A legend or label for the printed time.

### `void print_verification(double *m1, double *m2, int dim, char *name);`

Prints the verification result of two matrices.

- `m1`: Pointer to the first matrix.
- `m2`: Pointer to the second matrix.
- `dim`: Dimension of the matrices.
- `name`: A label or name for the matrices.

### `void run_and_time(multiply_function multiply_matrices, const double *a, const double *b, double *c, const double *gold, const int dim, const char *name, const int num_workers, const bool verify);`

Runs matrix multiplication, measures the execution time, and optionally verifies the result.

- `multiply_matrices`: Function pointer to the matrix multiplication function to be used.
- `a`: Pointer to the first input matrix (A).
- `b`: Pointer to the second input matrix (B).
- `c`: Pointer to the result matrix (C).
- `gold`: Pointer to the gold standard matrix for verification (can be set to NULL).
- `dim`: Dimension of the matrices.
- `name`: A label or name for the matrix multiplication method.
- `num_workers`: Number of parallel processes or threads.
- `verify`: Boolean flag indicating whether to verify the result.

## Example Usage

```c
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
```
![Example Output](https://github.com/AndrewBoessen/PA8-Parallel-Matrix-Multiplication/blob/main/assets/example_output.png)

## Author

Andrew Boessen - boessena@bc.edu

## COPYRIGHT

This software is released under the MIT License. See the LICENSE file for details.
