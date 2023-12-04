# Matrix Multiplication with Parallel Computing

This project showcases the performance benefits of parallel computing through matrix multiplication. Three algorithms are implemented and compared for matrix multiplication: serial, parallel using concurrent separate processes, and parallel using threads within the same process.

![example output](https://github.com/AndrewBoessen/PA8-Parallel-Matrix-Multiplication/blob/main/matmul.png)

## File Structure

- **matrix_mult.c:** Main source code file.
- **matrix_mult.h:** Contains exported symbolic constant definitions, type definitions, and forward declarations.
- **main.c:** Executable entry-point function `main` for testing purposes.

## Constants and Types

In `matrix_mult.h`, the following symbolic constants are defined:

```c
#define DIM           // The dimension of both the height and width of matrices.
#define NUM_WORKERS   // Number of parallel processes or threads used in the parallel algorithms.
#define SUCCESS       // Return value from functions to indicate success.
#define FAILURE       // Return value showing failure.

