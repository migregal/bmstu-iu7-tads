#ifndef __DATA_STRUCTURES_H__
#define __DATA_STRUCTURES_H__

#include <stdlib.h>
#include "common.h"

typedef struct
{
    double time1, time2;
} task_t;

typedef struct
{
    size_t *arr;
    int cap, ind;
} f_arr_t;

char create_array(f_arr_t **arr, const int capacity);

void output_array(const f_arr_t *arr);

void free_array(f_arr_t **arr);

#endif
