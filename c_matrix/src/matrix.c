#include "matrix.h"

// **********************************************************
typedef struct
{
    size_t rows;
    size_t cols;
} matrix_param;

static matrix_param* matrix_to_param(Matrix matrix)
{
    return ((void*)matrix - sizeof(matrix_param));
}

static Matrix param_to_matrix(matrix_param *param)
{
    return ((void*)param + sizeof(matrix_param));
}
// **********************************************************

Matrix make_matrix(size_t rows, size_t cols)
{
    if (rows == 0 || cols == 0)
        return NULL;
    matrix_param *param_data = malloc(sizeof(matrix_param) + 
                                      rows*sizeof(matrix_element_t*) + 
                                      rows*cols*sizeof(matrix_element_t));
    if (!param_data)
        return NULL;
    param_data->rows = rows;
    param_data->cols = cols;

    Matrix matrix = param_to_matrix(param_data);
    matrix[0] = (matrix_element_t*)(matrix + rows);
    for (size_t i = 1; i < rows; ++i)
        matrix[i] = matrix[i-1] + cols;

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            matrix[i][j] = MATRIX_ZERO;
    
    return matrix;
}

Matrix make_diagonal_matrix(size_t size, matrix_element_t value)
{
    Matrix matrix = MAKE_SQUARE_MATRIX(size);
    for (size_t i = 0; i < size; ++i)
        matrix[i][i] = value;
    return matrix;
}

void delete_matrix(Matrix matrix)
{
    if (matrix)
        free(matrix_to_param(matrix));
}

size_t matrix_rows(const Matrix matrix)
{
    return matrix_to_param((Matrix)matrix)->rows;
}

size_t matrix_cols(const Matrix matrix)
{
    return matrix_to_param((Matrix)matrix)->cols;
}

Matrix matrix_fill(Matrix matrix, matrix_element_t value)
{
    for (size_t i = 0; i < matrix_rows(matrix); ++i)
        for (size_t j = 0; j < matrix_cols(matrix); ++j)
            matrix[i][j] = value;
    return matrix;
}

Matrix matrix_copy(const Matrix matrix)
{
    Matrix copy = make_matrix(matrix_rows(matrix), matrix_cols(matrix));
    if (!copy)
        return NULL;
    matrix_set(copy, matrix);
    return copy;
}

Matrix matrix_set(Matrix matrix_dest, const Matrix matrix_src)
{
    if (!MATRIX_EQUAL_SIZE(matrix_dest, matrix_src))
        return NULL;
        
    for (size_t i = 0; i < matrix_rows(matrix_src); ++i)
        for (size_t j = 0; j < matrix_cols(matrix_src); ++j)
            matrix_dest[i][j] = matrix_src[i][j];
    return matrix_dest;
}

Matrix matrix_set_array_values(Matrix matrix, const matrix_element_t *array)
{
    size_t k = 0;
    for (size_t i = 0; i < matrix_rows(matrix); ++i)
        for (size_t j = 0; j < matrix_cols(matrix); ++j)
            matrix[i][j] = array[k++];
    return matrix;
}