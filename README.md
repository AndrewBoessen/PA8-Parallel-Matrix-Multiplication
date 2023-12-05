# Matrix Multiplication with Parallel Computing

This project showcases the performance benefits of parallel computing through matrix multiplication. Three algorithms are implemented and compared for matrix multiplication: serial, parallel using concurrent separate processes, and parallel using threads within the same process.

![example output](https://github.com/AndrewBoessen/PA8-Parallel-Matrix-Multiplication/blob/main/matmul.png)

## File Structure

- **matrix_mult.c:** Main source code file.
- **matrix_mult.h:** Contains exported symbolic constant definitions, type definitions, and forward declarations.
- **main.c:** Executable entry-point function `main` for testing purposes.

# Matrix Multiplication Library Manual

matrix_multiplication - Library for parallel matrix multiplication in C
```c
#include "matrix_multiplication.h"
```

## DESCRIPTION

The `matrix_multiplication` library provides functions for parallel matrix multiplication in C. It includes implementations using parallel processes, parallel threads, and a serial approach. The library is designed to perform matrix multiplication efficiently, especially for large matrices.

## FUNCTIONS

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

## EXAMPLE USAGE

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include "matrix_multiplication.h"

int main() {
    // Set the matrix dimension
    int dim = DIM;

    // Set the number of worker processes or threads
    int num_workers = NUM_WORKERS;

    // Allocate memory for matrices A, B, and C
    double *a = (double *)malloc(dim * dim * sizeof(double));
    double *b = (double *)malloc(dim * dim * sizeof(double));
    double *c = (double *)malloc(dim * dim * sizeof(double));
    
    // Initialize matrices A and B with random values
    init_matrix(a, dim);
    init_matrix(b, dim);

    // Perform matrix multiplication using parallel processes
    run_and_time(multiply_parallel_processes, a, b, c, NULL, dim, "Parallel Processes", num_workers, true);

    // Print the result matrix C
    printf("\nMatrix C (Parallel Processes):\n");
    print_matrix(c, dim);

    // Optionally, perform matrix multiplication using parallel threads
    // run_and_time(multiply_parallel_threads, a, b, c, NULL, dim, "Parallel Threads", num_workers, true);

    // Optionally, perform matrix multiplication using a serial approach
    // run_and_time(multiply_serial, a, b, c, NULL, dim, "Serial", num_workers, true);

    // Free allocated memory
    free(a);
    free(b);
    free(c);

    return 0;
}
```

## SEE ALSO

- [C Standard Library](https://en.cppreference.com/w/c/header)

## AUTHOR

Andrew Boessen - boessena@bc.edu

## COPYRIGHT

This software is released under the MIT License. See the LICENSE file for details.
