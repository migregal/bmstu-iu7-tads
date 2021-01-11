#include <stdio.h>

#include "data_structures.h"

char create_array(f_arr_t **arr, const int capacity)
{

    if (!((*arr) = (f_arr_t *)malloc(sizeof(f_arr_t))))
        return MEMORY_ALLOC_ERR;

    (*arr)->cap = capacity;
    (*arr)->ind = -1;
    (*arr)->arr = (size_t *)malloc((*arr)->cap * sizeof(size_t));

    return OK;
}

void output_array(const f_arr_t *arr)
{
    if (-1 == arr->ind)
    {
        printf("%s\n", "Массив пуст\n");
        return;
    }

    printf("\nСписок использованных адресов:\n");
    for (int i = 0; i < arr->ind + 1; ++i)
    {
        printf("%zx ", arr->arr[i]);
        if (i && !((i + 1) % 5))
            printf("\n");
    }
    printf("\n");
}

void free_array(f_arr_t **arr)
{
    if (!(*arr))
        return;

    if ((*arr)->arr)
        free((*arr)->arr);

    free((*arr));
}
