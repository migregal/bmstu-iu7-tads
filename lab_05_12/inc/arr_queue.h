#ifndef __ARR_QUEUE_H__
#define __ARR_QUEUE_H__

#include "data_structures.h"

typedef struct
{
    size_t cap, size;
    size_t front, rear;
    task_t *arr;
} arr_t;

char create_queuearr(arr_t **queue, const unsigned capacity);

char enqueuearr(arr_t *const queue, const task_t task);

char dequeuearr(arr_t *const queue, task_t **task);

char toparr(arr_t *const queue, task_t **task);

void freequeuearr(arr_t *queue);

#endif
