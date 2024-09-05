#include <stdio.h>
#include <stdlib.h>
#include "sparse_lu.h"
#include <time.h>
#include <sys/time.h>

void test_print_sparse(struct Sparse *sparse)
{
    int size = sparse->size;
    int len = sparse->line_end[size - 1];

    int i;
    for (i = 0; i < size; i++)
    {
        printf("%d\t", sparse->line_end[i]);
    }
    printf("\n");
    for (i = 0; i < len; i++)
    {
        printf("%d\t", sparse->index[i]);
    }
    printf("\n");
    for (i = 0; i < len; i++)
    {
        printf("%g\t", sparse->value[i]);
    }
    printf("\n");
}

void test_print_vector(double *vector, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%g\t", vector[i]);
    }
    printf("\n");
}

void test_print_matrix(double *matrix, int size)
{
    int i, j;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            printf("%g\t", matrix[i * size + j]);
        }
        printf("\n");
    }
}

void test_generate_matrix(double *matrix, int size, double sparse, int max_number)
{
    int i, j;

    srand(time(NULL));
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (1.0*rand() / RAND_MAX < sparse)
            {
                matrix[i * size + j] = 1.0 + rand()%max_number;
            }
            else
            {
                matrix[i * size + j] = 0.0;
            }
        }
    }

    for (i = 0; i < size; i++)
    {
        matrix[i * size + i] = 1.0 + rand()%max_number;
    }
}

void test_generate_vector(double *vector, int size, int max_number)
{
    int i;

    srand(time(NULL));
    for (i = 0; i < size; i++)
    {
        vector[i] = rand()%(max_number + 1);
    }
}

int main(void)
{
    int size = 5000;
    double *matrix = malloc(size * size * sizeof(double));
    /*double matrix[] = {
        1, 0, 2, 0,
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 1
    };*/

    test_generate_matrix(matrix, size, 0.0005, 100);
    //matrix[6 * size + 6] = 0.0;
    struct Sparse a, l, u;
    double *b = malloc(size * sizeof(double));
    //double b[] = { 1, 2, 3, 4 };
    double *x = malloc(size * sizeof(double));

    matrix_to_sparse(matrix, size, &a, CSR);
    //test_generate_vector(b, size, 100);

    struct timeval time_begin, time_end;
    gettimeofday(&time_begin, NULL);

    sparse_lu(&a, &l, &u);
    solve_sparse_lu(&l, &u, b, x);

    gettimeofday(&time_end, NULL);
    printf("%.6f sec\n", ((time_end.tv_sec + 1e-6*time_end.tv_usec) - (time_begin.tv_sec + 1e-6*time_begin.tv_usec)));

    //printf("A\n"); test_print_matrix(matrix, size); printf("\n");
    //printf("b\n"); test_print_vector(b, size); printf("\n");
    //printf("x\n"); test_print_vector(x, size); printf("\n");

    /*sparse_to_matrix(&l, matrix);
    printf("L\n"); test_print_matrix(matrix, size); printf("\n");
    sparse_to_matrix(&u, matrix);
    printf("U\n"); test_print_matrix(matrix, size); printf("\n");*/

    free(matrix);
    free(b);
    free(x);

    free_sparse(&a);
    free_sparse(&l);
    free_sparse(&u);

    return 0;
}