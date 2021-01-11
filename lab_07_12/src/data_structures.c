#include "data_structures.h"

adjmat_t *amcreate(const int size)
{
    adjmat_t *matrix = (adjmat_t *)malloc(sizeof(adjmat_t));
    if (!matrix)
        return NULL;

    matrix->size = size;
    matrix->matrix = (type_t **)malloc(sizeof(type_t *) * size);
    if (!matrix->matrix)
    {
        free(matrix);
        return NULL;
    }
    for (int i = 0; i < size; ++i)
    {
        matrix->matrix[i] = (type_t *)calloc(size, sizeof(type_t));
        if (!matrix->matrix[i])
        {
            amfree(matrix);
            return NULL;
        }
    }

    return matrix;
}

void amfree(adjmat_t *matrix)
{
    for (int i = 0; i < matrix->size; ++i)
        if (matrix->matrix[i])
            free(matrix->matrix[i]);

    free(matrix->matrix);
    free(matrix);
}

int amfill(FILE *const stream, adjmat_t *const matrix)
{
    int fvertex, svertex;
    int ec;

    while (TRUE)
    {
        if ((ec = rnginput(&fvertex, -1, matrix->size - 1)))
            return ec;

        if (fvertex == EOI)
            break;

        if ((ec = rnginput(&svertex, 0, matrix->size - 1)))
            return ec;

        matrix->matrix[fvertex][svertex] = GOT_CONNECTION;
        matrix->matrix[svertex][fvertex] = GOT_CONNECTION;
    }

    return EOK;
}

//! ------------------------------------- !//

listmat_t *lmcreate(const int size)
{
    listmat_t *matrix = (listmat_t *)malloc(sizeof(listmat_t));
    if (!matrix)
        return NULL;

    matrix->size = size;
    matrix->matrix = (node_t **)calloc(sizeof(node_t *), size);
    if (!matrix->matrix)
    {
        free(matrix);
        return NULL;
    }

    return matrix;
}

void lmfree(listmat_t *m)
{
    for (int i = 0; i < m->size; i++)
    {
        if (m->matrix[i])
            free_list(&(m->matrix[i]));
    }
    free(m->matrix);
    free(m);
}

int cmp(const void *a, const void *b)
{
    if (*(type_t *)a > *(type_t *)b)
        return 1;

    if (*(type_t *)a < *(type_t *)b)
        return -1;

    return 0;
}

int lmfill(FILE *const stream, listmat_t *const matrix)
{
    int fvertex, svertex;
    int ec;

    node_t *ptr = NULL;
    while (TRUE)
    {
        if ((ec = rnginput(&fvertex, -1, matrix->size - 1)))
            return ec;

        if (fvertex == EOI)
            break;

        if ((ec = rnginput(&svertex, 0, matrix->size - 1)))
            return ec;

        create_node(&ptr, &svertex, sizeof(int));
        sorted_insert(&(matrix->matrix[fvertex]), ptr, cmp);
        ptr = NULL;

        create_node(&ptr, &fvertex, sizeof(int));
        sorted_insert(&(matrix->matrix[svertex]), ptr, cmp);
        ptr = NULL;
    }

    return EOK;
}
