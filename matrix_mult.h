/*
 * Author: Andrew Boessen - boessena@bc.edu
 */

#include <stdbool.h>
#include <sys/time.h>

#define DIM 1024
#define NUM_WORKERS 8
#define SUCCESS 0
#define FAILURE -1

typedef void (* multiply_function)(double *, double *, double *, const int, const int);

typedef struct {
    double * a;
    double * b;
    double * c;
    int dim;
    int row_start;
    int chunk_size;
} Args;

/* Function forward declarations */
void multiply_parallel_processes(double * a, double * b, double * c, int dim, int num_workers);

void multiply_parallel_threads(double * a, double * b, double * c, int dim, int num_workers);

void init_matrix(double * matrix, int dim);

void multiply_serial(double * a, double * b, double * c, int dim, int num_workers);

void print_matrix(double * matrix, int dim);

int verify(double * m1, double * m2, int dim);

void print_elapsed_time(struct timeval * start, struct timeval * end, char * legend);

void print_verification(double * m1, double * m2, int dim, char * name);

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
        );
