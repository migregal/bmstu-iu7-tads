#ifndef __LIST_UTILS_H__
#define __LIST_UTILS_H__

#include <stdlib.h>
#include <string.h>

#include "rc.h"
#include "macro.h"
#include "data_types.h"

typedef struct list
{
    type_t *value;
    struct list *next;
} list_t;

int linsert(list_t *arr, type_t *val);

int deepcmp(type_t *s, list_t l);

int list_occupation(list_t *arr, int size);

void free_list_arr(list_t *arr, int size);

#endif
