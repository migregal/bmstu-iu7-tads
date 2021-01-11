#ifndef __OPS_H__
#define __PS_H__

#include "list_queue.h"
#include "arr_queue.h"

#define TASKS_COUNT 1000

void list_model(list_t *const queu1,
                list_t *const queue2,
                f_arr_t *const fmem,
                const int t_1_1, const int t_1_2,
                const int t_2_1, const int t_2_2);

void array_model(arr_t *const queue1,
                 arr_t *const queue2,
                 const int t_1_1, const int t_1_2,
                 const int t_2_1, const int t_2_2);

#endif
