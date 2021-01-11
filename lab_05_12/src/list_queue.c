#include <string.h>

#include "list_queue.h"

char create_queuenode(const task_t task, node_t **node)
{
    (*node) = malloc(sizeof(node_t));
    if (!(*node))
        return MEMORY_ALLOC_ERR;

    if (!((*node)->task = malloc(sizeof(task_t))))
    {
        free((*node));
        return MEMORY_ALLOC_ERR;
    }

    memcpy((*node)->task, &task, sizeof(task_t));
    (*node)->next = NULL;

    return OK;
}

char create_queuelist(list_t **queue, size_t capacity)
{
    if (!((*queue) = malloc(sizeof(list_t))))
        return MEMORY_ALLOC_ERR;

    (*queue)->rear = NULL;
    (*queue)->front = NULL;
    (*queue)->size = 0;
    (*queue)->cap = capacity;

    return OK;
}

char enqueuelist(list_t *const queue, const task_t task)
{
    node_t *node = NULL;

    create_queuenode(task, &node);

    if (queue->size == queue->cap)
        return QUEUE_OVERFLOW_ERR;

    if (NULL == queue->rear)
        queue->front = node;
    else
        queue->rear->next = node;

    queue->rear = node;
    queue->size++;

    return OK;
}

char dequeuelist(list_t *const queue, task_t **task, f_arr_t *const fmem)
{
    if (NULL == queue->front)
        return QUEUE_EMPTY_ERR;

    node_t *node = queue->front;
    fmem->arr[++fmem->ind] = (size_t)node;
    (*task) = node->task;

    if (queue->front == queue->rear)
    {
        queue->rear = NULL;
        queue->front = NULL;
    }
    else
    {
        queue->front = queue->front->next;
    }

    free(node);

    queue->size--;

    return OK;
}

char toplist(list_t *const queue, task_t **task)
{
    if (NULL == queue->front)
        return QUEUE_EMPTY_ERR;

    *task = queue->front->task;
    return OK;
}

void freequeuelist(list_t *queue, f_arr_t *const fmem)
{
    if (queue == NULL)
        return;

    while (queue->rear)
    {
        task_t *task = NULL;
        dequeuelist(queue, &task, fmem);
        free(task);
    }

    free(queue);
}

int check_rear(list_t *queue, f_arr_t *fmem)
{
    size_t top = (size_t)queue->rear;
    int is_found = 0;

    for (int i = 0; i < fmem->ind; ++i)
    {
        if (top != fmem->arr[i])
            continue;

        is_found = 1;
        for (int j = i; j < fmem->ind - 1; ++j)
        {
            size_t temp = fmem->arr[j];
            fmem->arr[j] = fmem->arr[j + 1];
            fmem->arr[j + 1] = temp;
        }
        break;
    }

    fmem->ind -= (1 == is_found);

    return is_found;
}
