#include "../inc/list_utils.h"

int linsert(list_t *arr, type_t *val)
{
    if (arr->next == NULL)
    {
        arr->next = calloc(1, sizeof(list_t));
        arr->value = malloc(sizeof(type_t));
        memcpy(arr->value, val, sizeof(type_t));

        return 1;
    }

    int depth = 1;
    depth += linsert(arr->next, val);

    return depth;
}

int deepcmp(type_t *s, list_t l)
{
    list_t *lp = &l;
    int i = 0;

    while (lp != NULL && lp->value != NULL)
    {
        i++;
        if (*s == *(lp->value))
            return i;

        lp = lp->next;
    }

    return 0;
}

int list_occupation(list_t *arr, int size)
{
    int occupation = 0;
    for (int i = 0; i < size; i++)
        if (arr[i].value != NULL)
            occupation++;

    return occupation;
}

static void free_list(list_t *node)
{
    if (node == NULL)
        return;

    free_list(node->next);

    free(node);
}

void free_list_arr(list_t *arr, int size)
{
    for (int i = 0; i < size; i++)
        free_list(arr[i].next);

    free(arr);
}
