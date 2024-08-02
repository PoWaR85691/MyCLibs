#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

typedef double matrix_element_t;
#define MATRIX_ZERO 0.0
#define MATRIX_ONE 1.0

typedef matrix_element_t** Matrix;

Matrix make_matrix(size_t rows, size_t cols);
#define MAKE_SQUARE_MATRIX(SIZE) make_matrix((SIZE), (SIZE))
#define MAKE_COL_VECTOR(SIZE) make_matrix((SIZE), 1)
#define MAKE_ROW_VECTOR(SIZE) make_matrix(1, (SIZE))
Matrix make_diagonal_matrix(size_t size, matrix_element_t value);
#define MAKE_EYE_MATRIX(SIZE) make_diagonal_matrix((SIZE), MATRIX_ONE)
void delete_matrix(Matrix matrix);

size_t matrix_rows(const Matrix matrix);
size_t matrix_cols(const Matrix matrix);
#define MATRIX_IS_SQUARE(MATRIX) (matrix_rows(MATRIX) == matrix_cols(MATRIX))
#define MATRIX_EQUAL_SIZE(MATRIX1, MATRIX2) (matrix_rows(MATRIX1) == matrix_rows(MATRIX2) && \
                                             matrix_cols(MATRIX1) == matrix_cols(MATRIX2))

Matrix matrix_fill(Matrix matrix, matrix_element_t value);

Matrix matrix_copy(const Matrix matrix);
Matrix matrix_set(Matrix matrix_dest, const Matrix matrix_src);
Matrix matrix_set_array_values(Matrix matrix, const matrix_element_t *array);

#endif