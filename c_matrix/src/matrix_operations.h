#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include "matrix.h"
#include <stdbool.h>
#include <math.h>
#include <float.h>

#define SWAP(A, B, TYPE) do { TYPE t__ = (A); (A) = (B); (B) = t__; } while (false)

typedef matrix_element_t* vector_t;

#define ABS(VALUE) ((VALUE) >= MATRIX_ZERO ? (VALUE) : -(VALUE))
#define MAX(VALUE1, VALUE2) ((VALUE1) > (VALUE2) ? (VALUE1) : (VALUE2))
#define MIN(VALUE1, VALUE2) ((VALUE1) < (VALUE2) ? (VALUE1) : (VALUE2))
#define EQUAL(VALUE1, VALUE2) (ABS((VALUE1) - (VALUE2)) <= DBL_EPSILON * MAX(ABS(VALUE1), ABS(VALUE2)))

Matrix matrix_add(Matrix matrix1, const Matrix matrix2);
Matrix matrix_sub(Matrix matrix1, const Matrix matrix2);
Matrix matrix_neg(Matrix matrix);

Matrix matrix_mul_elements(Matrix matrix1, const Matrix matrix2);
Matrix matrix_div_elements(Matrix matrix1, const Matrix matrix2);
Matrix matrix_pow_elements(Matrix matrix, double power);

Matrix matrix_mul_value(Matrix matrix, matrix_element_t value);
Matrix matrix_div_value(Matrix matrix, matrix_element_t value);

Matrix matrix_add_diagonal(Matrix matrix, matrix_element_t value);
Matrix matrix_sub_diagonal(Matrix matrix, matrix_element_t value);

bool matrix_equal(const Matrix matrix1, const Matrix matrix2);

Matrix matrix_mul(const Matrix matrix1, const Matrix matrix2);
Matrix matrix_pow(const Matrix matrix, unsigned power);

Matrix matrix_trans(const Matrix matrix);
Matrix matrix_inv(const Matrix matrix);

matrix_element_t matrix_det(const Matrix matrix);
vector_t matrix_linsolve(const Matrix matrix_a, const vector_t vector_b);

#endif