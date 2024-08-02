#include "matrix_operations.h"

Matrix matrix_add(Matrix matrix1, const Matrix matrix2)
{
    if (!MATRIX_EQUAL_SIZE(matrix1, matrix2))
        return NULL;

    for (size_t i = 0; i < matrix_rows(matrix1); ++i)
        for (size_t j = 0; j < matrix_cols(matrix1); ++j)
            matrix1[i][j] += matrix2[i][j];
    return matrix1;
}

Matrix matrix_sub(Matrix matrix1, const Matrix matrix2)
{
    if (!MATRIX_EQUAL_SIZE(matrix1, matrix2))
        return NULL;

    for (size_t i = 0; i < matrix_rows(matrix1); ++i)
        for (size_t j = 0; j < matrix_cols(matrix1); ++j)
            matrix1[i][j] -= matrix2[i][j];
    return matrix1;
}

Matrix matrix_neg(Matrix matrix)
{
    for (size_t i = 0; i < matrix_rows(matrix); ++i)
        for (size_t j = 0; j < matrix_cols(matrix); ++j)
            matrix[i][j] = -matrix[i][j];
    return matrix;
}

Matrix matrix_mul_elements(Matrix matrix1, const Matrix matrix2)
{
    if (!MATRIX_EQUAL_SIZE(matrix1, matrix2))
        return NULL;

    for (size_t i = 0; i < matrix_rows(matrix1); ++i)
        for (size_t j = 0; j < matrix_cols(matrix1); ++j)
            matrix1[i][j] *= matrix2[i][j];
    return matrix1;
}

Matrix matrix_div_elements(Matrix matrix1, const Matrix matrix2)
{
    if (!MATRIX_EQUAL_SIZE(matrix1, matrix2))
        return NULL;

    for (size_t i = 0; i < matrix_rows(matrix1); ++i)
        for (size_t j = 0; j < matrix_cols(matrix1); ++j)
            matrix1[i][j] /= matrix2[i][j];
    return matrix1;
}

Matrix matrix_pow_elements(Matrix matrix, double power)
{
    for (size_t i = 0; i < matrix_rows(matrix); ++i)
        for (size_t j = 0; j < matrix_cols(matrix); ++j)
            matrix[i][j] = pow(matrix[i][j], power);
    return matrix;
}

Matrix matrix_mul_value(Matrix matrix, matrix_element_t value)
{
    for (size_t i = 0; i < matrix_rows(matrix); ++i)
        for (size_t j = 0; j < matrix_cols(matrix); ++j)
            matrix[i][j] *= value;
    return matrix;
}

Matrix matrix_div_value(Matrix matrix, matrix_element_t value)
{
    for (size_t i = 0; i < matrix_rows(matrix); ++i)
        for (size_t j = 0; j < matrix_cols(matrix); ++j)
            matrix[i][j] /= value;
    return matrix;
}

Matrix matrix_add_diagonal(Matrix matrix, matrix_element_t value)
{
    if (!MATRIX_IS_SQUARE(matrix))
        return NULL;

    for (size_t i = 0; i < matrix_rows(matrix); ++i)
        matrix[i][i] += value;
    return matrix;
}

Matrix matrix_sub_diagonal(Matrix matrix, matrix_element_t value)
{
    if (!MATRIX_IS_SQUARE(matrix))
        return NULL;

    for (size_t i = 0; i < matrix_rows(matrix); ++i)
        matrix[i][i] -= value;
    return matrix;
}

bool matrix_equal(const Matrix matrix1, const Matrix matrix2)
{
    if (!MATRIX_EQUAL_SIZE(matrix1, matrix2))
        return false;

    for (size_t i = 0; i < matrix_rows(matrix1); ++i)
        for (size_t j = 0; j < matrix_cols(matrix1); ++j)
            if (!EQUAL(matrix1[i][j], matrix2[i][j]))
                return false;
    return true;
}

Matrix matrix_mul(const Matrix matrix1, const Matrix matrix2)
{
    if (matrix_cols(matrix1) != matrix_rows(matrix2))
        return NULL;

    Matrix result = make_matrix(matrix_rows(matrix1), matrix_cols(matrix2));
    if (!result)
        return NULL;

    for (size_t i = 0; i < matrix_rows(matrix1); ++i)
        for (size_t j = 0; j < matrix_cols(matrix2); ++j)
        {
            matrix_element_t value = MATRIX_ZERO;
            for (size_t k = 0; k < matrix_cols(matrix1); ++k)
                value += matrix1[i][k] * matrix2[k][j];
            result[i][j] = value;
        }
    return result;
}

Matrix matrix_pow(const Matrix matrix, unsigned power)
{
    if (!MATRIX_IS_SQUARE(matrix))
        return NULL;

    if (power == 0)
        return MAKE_EYE_MATRIX(matrix_rows(matrix));

    Matrix result = NULL;
    Matrix sqr = matrix_copy(matrix);
    if (!sqr)
        return NULL;

    unsigned to_sqr = power;
    while (true)
    {
        if (to_sqr & 1)
        {
            if (result)
            {
                Matrix temp = matrix_mul(result, sqr);
                delete_matrix(result);

                if (!temp)
                {
                    if (sqr != result)
                        delete_matrix(sqr);
                    return NULL;
                }

                result = temp;
            }
            else
                result = sqr;
        }

        to_sqr >>= 1;
        if (to_sqr)
        {
            Matrix temp = matrix_mul(sqr, sqr);
            if (sqr != result)
                delete_matrix(sqr);

            if (!temp)
            {
                delete_matrix(result);
                return NULL;
            }

            sqr = temp;
        }
        else
        {
            if (sqr != result)
                delete_matrix(sqr);
            return result;
        }
    }
}

Matrix matrix_trans(const Matrix matrix)
{
    Matrix result = make_matrix(matrix_cols(matrix), matrix_rows(matrix));
    if (!result)
        return NULL;

    for (size_t i = 0; i < matrix_rows(result); ++i)
        for (size_t j = 0; j < matrix_cols(result); ++j)
            result[i][j] = matrix[j][i];
    return result;
}

// **********************************************************
static size_t gauss_max_col_element_i(const Matrix matrix, size_t i)
{
    matrix_element_t max_col_element = ABS(matrix[i][i]);
    size_t max_i = i;
    for (size_t k = i + 1; k < matrix_rows(matrix); ++k)
        if (ABS(matrix[k][i]) > max_col_element)
        {
            max_col_element = ABS(matrix[k][i]);
            max_i = k;
        }
    return max_i;
}
// **********************************************************

Matrix matrix_inv(const Matrix matrix)
{
    if (!MATRIX_IS_SQUARE(matrix))
        return NULL;

    Matrix copy = matrix_copy(matrix);
    if (!copy)
        return NULL;

    Matrix inv = MAKE_EYE_MATRIX(matrix_rows(copy));
    if (!inv)
    {
        delete_matrix(copy);
        return NULL;
    }

    for (size_t i = 0; i < matrix_rows(copy); ++i)
    {
        size_t max_i = gauss_max_col_element_i(copy, i);
        SWAP(copy[i], copy[max_i], matrix_element_t*);
        SWAP(inv[i], inv[max_i], matrix_element_t*);
        
        if (copy[i][i] == MATRIX_ZERO)
        {
            delete_matrix(copy);
            delete_matrix(inv);
            return NULL;
        }

        for (size_t k = i + 1; k < matrix_rows(copy); ++k)
        {
            if (copy[k][i] == MATRIX_ZERO)
                continue;

            for (size_t j = i + 1; j < matrix_cols(copy); ++j)
                copy[k][j] -= copy[i][j] * copy[k][i]/copy[i][i];
            for (size_t j = 0; j < matrix_cols(inv); ++j)
                inv[k][j] -= inv[i][j] * copy[k][i]/copy[i][i];
        }
    }

    for (size_t i = matrix_rows(copy) - 1; ; --i)
    {
        for (size_t j = 0; j < matrix_cols(inv); ++j)
            inv[i][j] /= copy[i][i];
        for (size_t k = 0; k < i; ++k)
        {
            if (copy[k][i] == MATRIX_ZERO)
                continue;
                
            for (size_t j = 0; j < matrix_cols(inv); ++j)
                inv[k][j] -= inv[i][j] * copy[k][i];
        }

        if (i == 0)
            break;
    }

    delete_matrix(copy);
    return inv;
}

matrix_element_t matrix_det(const Matrix matrix)
{
    if (!MATRIX_IS_SQUARE(matrix))
        return NAN;

    Matrix copy = matrix_copy(matrix);
    if (!copy)
        return NAN;

    bool sign = true;

    for (size_t i = 0; i < matrix_rows(copy); ++i)
    {
        size_t max_i = gauss_max_col_element_i(copy, i);
        if (max_i != i)
        {
            SWAP(copy[i], copy[max_i], matrix_element_t*);
            sign = !sign;
        }
        
        if (copy[i][i] == MATRIX_ZERO)
        {
            delete_matrix(copy);
            return MATRIX_ZERO;
        }

        for (size_t k = i + 1; k < matrix_rows(copy); ++k)
        {
            if (copy[k][i] == MATRIX_ZERO)
                continue;

            for (size_t j = i + 1; j < matrix_cols(copy); ++j)
                copy[k][j] -= copy[i][j] * copy[k][i]/copy[i][i];
        }
    }

    matrix_element_t det = (sign ? MATRIX_ONE : -MATRIX_ONE);
    for (size_t i = 0; i < matrix_rows(copy); ++i)
        det *= copy[i][i];

    delete_matrix(copy);
    return det;
}

vector_t matrix_linsolve(const Matrix matrix_a, const vector_t vector_b)
{
    if (!MATRIX_IS_SQUARE(matrix_a))
        return NULL;

    Matrix a_copy = matrix_copy(matrix_a);
    if (!a_copy)
        return NULL;

    vector_t b_copy = malloc(matrix_rows(a_copy)*sizeof(matrix_element_t));
    if (!b_copy)
    {
        delete_matrix(a_copy);
        return NULL;
    }
    for (size_t i = 0; i < matrix_rows(a_copy); ++i)
        b_copy[i] = vector_b[i];

    for (size_t i = 0; i < matrix_rows(a_copy); ++i)
    {
        size_t max_i = gauss_max_col_element_i(a_copy, i);
        SWAP(a_copy[i], a_copy[max_i], matrix_element_t*);
        SWAP(b_copy[i], b_copy[max_i], matrix_element_t);
        
        if (a_copy[i][i] == MATRIX_ZERO)
        {
            delete_matrix(a_copy);
            free(b_copy);
            return NULL;
        }

        for (size_t k = i + 1; k < matrix_rows(a_copy); ++k)
        {
            if (a_copy[k][i] == MATRIX_ZERO)
                continue;

            for (size_t j = i + 1; j < matrix_cols(a_copy); ++j)
                a_copy[k][j] -= a_copy[i][j] * a_copy[k][i]/a_copy[i][i];
            b_copy[k] -= b_copy[i] * a_copy[k][i]/a_copy[i][i];
        }
    }

    for (size_t i = matrix_rows(a_copy) - 1; ; --i)
    {
        for (size_t j = i + 1; j < matrix_cols(a_copy); ++j)
            b_copy[i] -= b_copy[j] * a_copy[i][j];
        b_copy[i] /= a_copy[i][i];

        if (i == 0)
            break;
    }

    delete_matrix(a_copy);
    return b_copy;
}