#ifndef __LIST_QUEUE_H__
#define __LIST_QUEUE_H__

#include "data_structures.h"

typedef struct node node_t;

struct node
{
    task_t *task;
    node_t *next;
};

typedef struct
{
    size_t cap, size;
    node_t *front, *rear;
} list_t;

char create_queuenode(const task_t task, node_t **node);

char create_queuelist(list_t **queue, size_t capacity);

char enqueuelist(list_t *const queue, const task_t task);

char toplist(list_t *const queue, task_t **task);

char dequeuelist(list_t *const queue, task_t **task, f_arr_t *const fmem);

void freequeuelist(list_t *queue, f_arr_t *const fmem);

int check_rear(list_t *queue, f_arr_t *fmem);

#endif
