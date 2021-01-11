#include <string.h>

#include "arr_queue.h"

char create_queuearr(arr_t **queue, const unsigned capacity)
{
    if (!((*queue) = malloc(sizeof(arr_t))))
    {
        (*queue) = NULL;
        return MEMORY_ALLOC_ERR;
    }

    (*queue)->cap = capacity;
    (*queue)->front = 0;
    (*queue)->size = 0;
    (*queue)->rear = capacity - 1;
    (*queue)->arr = malloc((*queue)->cap * sizeof(task_t));

    if (!(*queue)->arr)
    {
        free((*queue));
        (*queue) = NULL;
        return MEMORY_ALLOC_ERR;
    }

    return OK;
}

char enqueuearr(arr_t *const queue, const task_t task)
{
    if (queue->size == queue->cap)
        return QUEUE_OVERFLOW_ERR;

    queue->rear = (queue->rear + 1) % queue->cap;
    queue->arr[queue->rear] = task;
    queue->size++;

    return OK;
}

char dequeuearr(arr_t *const queue, task_t **task)
{
    if (queue->size == 0)
        return QUEUE_EMPTY_ERR;

    *task = queue->arr + queue->front;
    queue->front = (queue->front + 1) % queue->cap;
    queue->size--;

    return OK;
}

char toparr(arr_t *const queue, task_t **task)
{
    if (queue->size == 0)
        return QUEUE_EMPTY_ERR;

    *task = queue->arr + queue->front;

    return OK;
}

void freequeuearr(arr_t *queue)
{
    if (!queue)
        return;

    if (queue->arr)
        free(queue->arr);

    free(queue);
}
