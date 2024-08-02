#include <stdio.h>
#include <time.h>
#include "matrix.h"
#include "matrix_operations.h"

void print_matrix(Matrix matrix)
{
    printf("rows = %zu, cols = %zu, is square: %s\n", 
           matrix_rows(matrix), matrix_cols(matrix), 
           (MATRIX_IS_SQUARE(matrix) ? "true" : "false"));
    for (size_t i = 0; i < matrix_rows(matrix); ++i)
    {
        for (size_t j = 0; j < matrix_cols(matrix); ++j)
            printf("%10.4e ", matrix[i][j]);
        putchar('\n');
    }
}

void print_vector(vector_t vector, size_t length)
{
    printf("length = %zu\n\t{ ", length);
    for (size_t i = 0; i < length; ++i)
        printf("%10.4f ", vector[i]);
    printf("}\n");
}

#define N 5

int main(void)
{
    //size_t i, j;

    double a[N][N] = {
        { 56, 24, 77, 1, 0 },
        { 9, 8, 7, 6, 3 },
        { 0, 4, 3, 11, 21 },
        { 1, 2, 3, 4, 9 },
        { 17, 9, 33, 0, 8 },
    };

    Matrix mat1 = MAKE_SQUARE_MATRIX(N);
    matrix_set_array_values(mat1, (double*)a);

    clock_t begin, end, dtime;
    begin = clock();
    for (int i = 0; i < 10; ++i)
    {
        Matrix testM = matrix_pow(mat1, 100);
        if (i == 0) print_matrix(testM);
        delete_matrix(testM);
    }
    end = clock();

    dtime = end - begin;
    printf("time: %ld.%03ld\n", dtime/CLOCKS_PER_SEC, dtime%CLOCKS_PER_SEC);

    delete_matrix(mat1);

    return 0;
}