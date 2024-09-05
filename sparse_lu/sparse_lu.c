#include <stdio.h>
#include <stdlib.h>
#include "sparse_lu.h"

/* Вспомогательное */
int find_or_next_binary(const int *begin, const int *end, int value, int **found)
{
    const int *left = begin;
    const int *right = end - 1;

    while (left <= right)
    {
        const int *center = left + (right - left) / 2;

        if (*center < value) // Right
        {
            left = center + 1;
        }
        else if (*center > value) // Left
        {
            right = center - 1;
        }
        else // Found
        {
            *found = (int*)center;
            return 1;
        }
    }

    // Not found
    *found = (int*)left; // Next
    return 0;
}

int get_line_begin(struct Sparse *sparse, int line)
{
    if (line == 0)
    {
        return 0;
    }
    else
    {
        return sparse->line_end[line - 1];
    }
}

double sum_lu_ij(struct Sparse *l, struct Sparse *u, int i, int j)
{
    double sum = 0.0;

    int l_curr_pos, u_curr_pos;
    l_curr_pos = get_line_begin(l, i);
    u_curr_pos = get_line_begin(u, j);

    while ((l_curr_pos < l->line_end[i]) && (u_curr_pos < u->line_end[j]))
    {
        if (l->index[l_curr_pos] < u->index[u_curr_pos])
        {
            l_curr_pos++;
        }
        else if (l->index[l_curr_pos] > u->index[u_curr_pos])
        {
            u_curr_pos++;
        }
        else // ==
        {
            sum += l->value[l_curr_pos] * u->value[u_curr_pos];
            l_curr_pos++;
            u_curr_pos++;
        }
    }

    return sum;
}

double check_ij(struct Sparse *sparse, int *curr_pos, int i, int j)
{
    double value;
    if ((*curr_pos < sparse->line_end[i]) && (sparse->index[*curr_pos] == j))
    {
        value = sparse->value[*curr_pos];
        (*curr_pos)++;
    }
    else
    {
        value = 0.0;
    }

    return value;
}

void add_end(struct Sparse *sparse, int *end_pos, int index, double value)
{
    sparse->index[*end_pos] = index;
    sparse->value[*end_pos] = value;
    (*end_pos)++;
}

/* Основное */
void matrix_to_sparse(double *matrix, int size, struct Sparse *sparse, enum Sparse_type type)
{
    int i, j;

    sparse->type = type;
    sparse->size = size;

    int not_zeros = 0;
    for (i = 0; i < size * size; i++)
    {
        if (matrix[i] != 0.0)
        {
            not_zeros++;
        }
    }

    sparse->line_end = (int*)malloc(size * sizeof(int));
    sparse->index = (int*)malloc(not_zeros * sizeof(int));
    sparse->value = (double*)malloc(not_zeros * sizeof(double));

    if (type == CSR)
    {
        int end_pos = 0;
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                if (matrix[i * size + j] != 0.0)
                {
                    add_end(sparse, &end_pos, j, matrix[i * size + j]);
                }
            }
            // End line
            sparse->line_end[i] = end_pos;
        }
    }
    else // CSC
    {
        int end_pos = 0;
        for (j = 0; j < size; j++)
        {
            for (i = 0; i < size; i++)
            {
                if (matrix[i * size + j] != 0.0)
                {
                    add_end(sparse, &end_pos, i, matrix[i * size + j]);
                }
            }
            // End line
            sparse->line_end[j] = end_pos;
        }
    }
}

void sparse_to_matrix(struct Sparse *sparse, double *matrix)
{
    int i, j;
    int size = sparse->size;

    if (sparse->type == CSR)
    {
        int curr_pos = 0;
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                if ((curr_pos < sparse->line_end[i]) && (sparse->index[curr_pos] == j))
                {
                    matrix[i * size + j] = sparse->value[curr_pos];
                    curr_pos++;
                }
                else
                {
                    matrix[i * size + j] = 0.0;
                }
            }
        }
    }
    else // CSC
    {
        int curr_pos = 0;
        for (j = 0; j < size; j++)
        {
            for (i = 0; i < size; i++)
            {
                if ((curr_pos < sparse->line_end[j]) && (sparse->index[curr_pos] == i))
                {
                    matrix[i * size + j] = sparse->value[curr_pos];
                    curr_pos++;
                }
                else
                {
                    matrix[i * size + j] = 0.0;
                }
            }
        }
    }
}

void free_sparse(struct Sparse *sparse)
{
    free(sparse->line_end);
    free(sparse->index);
    free(sparse->value);
}

void change_sparse_type(struct Sparse *old_sparse, struct Sparse *new_sparse)
{
    int i, j;

    int size = old_sparse->size;
    if (old_sparse->type == CSR)
    {
        new_sparse->type = CSC;
    }
    else // CSC
    {
        new_sparse->type = CSR;
    }
    new_sparse->size = old_sparse->size;
    new_sparse->line_end = (int*)malloc(size * sizeof(int));
    new_sparse->index = (int*)malloc(old_sparse->line_end[size - 1] * sizeof(int));
    new_sparse->value = (double*)malloc(old_sparse->line_end[size - 1] * sizeof(double));

    int *old_line_curr_pos = (int*)malloc(size * sizeof(int));
    old_line_curr_pos[0] = 0;
    for (i = 1; i < size; i++)
    {
        old_line_curr_pos[i] = old_sparse->line_end[i - 1];
    }

    int new_end_pos = 0;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if ((old_line_curr_pos[j] < old_sparse->line_end[j]) && (old_sparse->index[old_line_curr_pos[j]] == i))
            {
                new_sparse->index[new_end_pos] = j;
                new_sparse->value[new_end_pos] = old_sparse->value[old_line_curr_pos[j]];

                old_line_curr_pos[j]++;
                new_end_pos++;
            }
        }
        // End line
        new_sparse->line_end[i] = new_end_pos;
    }

    // Free
    free(old_line_curr_pos);
}

void sparse_lu(struct Sparse *a, struct Sparse *l, struct Sparse *u)
{
    int i, j;

    int size = a->size;

    // A
    enum Sparse_type a_type = a->type;
    int a_curr_pos; // 1D
    int *a_line_curr_pos = (int*)malloc(size * sizeof(int)); // 2D

    a_line_curr_pos[0] = ((a_type == CSR) ? 0 : 1);
    for (i = 1; i < size; i++)
    {
        int is_found;
        int *ii_index;
        is_found = find_or_next_binary(a->index + a->line_end[i - 1], a->index + a->line_end[i], ((a_type == CSR) ? i : i + 1), &ii_index);
        a_line_curr_pos[i] = ii_index - a->index;
    }

    // LU
    struct Sparse tmp_l, tmp_u;
    int *l_end_pos, *u_end_pos;

    tmp_l.type = CSR;
    tmp_u.type = CSC;

    tmp_l.size = size;
    tmp_u.size = size;

    tmp_l.line_end = (int*)malloc(size * sizeof(int));
    tmp_u.line_end = (int*)malloc(size * sizeof(int));

    tmp_l.index = (int*)malloc((1 + size)*size / 2 * sizeof(int));
    tmp_u.index = (int*)malloc((1 + size)*size / 2 * sizeof(int));

    tmp_l.value = (double*)malloc((1 + size)*size / 2 * sizeof(double));
    tmp_u.value = (double*)malloc((1 + size)*size / 2 * sizeof(double));

    l_end_pos = tmp_l.line_end; *l_end_pos = 0;
    u_end_pos = tmp_u.line_end; *u_end_pos = 0;

    for (i = 0; i < size; i++)
    {
        a_curr_pos = get_line_begin(a, i);

        // L
        for (j = 0; j < i; j++)
        {
            double a_ij;
            if (a_type == CSR)
            {
                a_ij = check_ij(a, &a_curr_pos, i, j);
            }
            else // CSC
            {
                a_ij = check_ij(a, a_line_curr_pos + j, j, i);
            }

            double sum_ij = sum_lu_ij(&tmp_l, &tmp_u, i, j);
            double u_jj = tmp_u.value[tmp_u.line_end[j] - 1];

            // Save
            double l_ij = (a_ij - sum_ij) / u_jj;
            if (l_ij != 0.0)
            {
                add_end(&tmp_l, l_end_pos, j, l_ij);
            }
        }
        // l_ii
        add_end(&tmp_l, l_end_pos, i, 1.0);

        // End line
        l_end_pos++;
        if (i < size - 1)
        {
            *l_end_pos = *(l_end_pos - 1);
        }

        // U
        for (j = 0; j < i + 1; j++)
        {
            double a_ji;
            if (a_type == CSR)
            {
                a_ji = check_ij(a, a_line_curr_pos + j, j, i);
            }
            else // CSC
            {
                a_ji = check_ij(a, &a_curr_pos, i, j);
            }

            double sum_ji = sum_lu_ij(&tmp_l, &tmp_u, j, i);

            // Save
            double u_ji = a_ji - sum_ji;
            if (u_ji != 0.0)
            {
                add_end(&tmp_u, u_end_pos, j, u_ji);
            }

        }
        // End line
        u_end_pos++;
        if (i < size - 1)
        {
            *u_end_pos = *(u_end_pos - 1);
        }
    }

    // Save
    l->type = CSR;
    u->type = CSC;

    l->size = size;
    u->size = size;

    l->line_end = (int*)malloc(size * sizeof(int));
    u->line_end = (int*)malloc(size * sizeof(int));
    for (i = 0; i < size; i++)
    {
        l->line_end[i] = tmp_l.line_end[i];
        u->line_end[i] = tmp_u.line_end[i];
    }

    l->index = (int*)malloc(tmp_l.line_end[size - 1] * sizeof(int));
    u->index = (int*)malloc(tmp_u.line_end[size - 1] * sizeof(int));

    l->value = (double*)malloc(tmp_l.line_end[size - 1] * sizeof(double));
    u->value = (double*)malloc(tmp_u.line_end[size - 1] * sizeof(double));

    for (i = 0; i < tmp_l.line_end[size - 1]; i++)
    {
        l->index[i] = tmp_l.index[i];
        l->value[i] = tmp_l.value[i];
    }
    for (i = 0; i < tmp_u.line_end[size - 1]; i++)
    {
        u->index[i] = tmp_u.index[i];
        u->value[i] = tmp_u.value[i];
    }

    // Free
    // A
    free(a_line_curr_pos);

    // LU
    free(tmp_l.line_end);
    free(tmp_u.line_end);

    free(tmp_l.index);
    free(tmp_u.index);

    free(tmp_l.value);
    free(tmp_u.value);
}

void solve_sparse_lu(struct Sparse *l, struct Sparse *u, double *b, double *x)
{
    // LUx = b
    int i;
    int size = l->size;

    // Ly = b, y = Ux
    if (l->type == CSR)
    {
        for (i = 0; i < size; i++)
        {
            x[i] = b[i];

            int l_curr_pos;
            for (l_curr_pos = get_line_begin(l, i); l_curr_pos < l->line_end[i] - 1; l_curr_pos++) // j = 0..i-1
            {
                x[i] -= l->value[l_curr_pos] * x[l->index[l_curr_pos]];
            }
        }
    }
    else // CSC
    {
        for (i = 0; i < size; i++)
        {
            x[i] = b[i];
        }
        for (i = 0; i < size; i++)
        {
            int l_curr_pos;
            for (l_curr_pos = get_line_begin(l, i) + 1; l_curr_pos < l->line_end[i]; l_curr_pos++) // j = i+1..size-1
            {
                x[l->index[l_curr_pos]] -= l->value[l_curr_pos] * x[i];
            }
        }
    }

    // Ux = y
    if (u->type == CSR)
    {
        for (i = size - 1; i >= 0; i--)
        {
            int u_curr_pos;
            for (u_curr_pos = get_line_begin(u, i) + 1; u_curr_pos < u->line_end[i]; u_curr_pos++) // j = i+1..size-1
            {
                x[i] -= u->value[u_curr_pos] * x[u->index[u_curr_pos]];
            }

            x[i] /= u->value[get_line_begin(u, i)]; // x[i] /= u[i][i]
        }
    }
    else // CSC
    {
        for (i = size - 1; i >= 0; i--)
        {
            x[i] /= u->value[u->line_end[i] - 1]; // x[i] /= u[i][i]

            int u_curr_pos;
            for (u_curr_pos = get_line_begin(u, i); u_curr_pos < u->line_end[i] - 1; u_curr_pos++) // j = 0..i-1
            {
                x[u->index[u_curr_pos]] -= u->value[u_curr_pos] * x[i];
            }
        }
    }
}