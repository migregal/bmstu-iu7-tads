#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <string.h>

#include "common.h"
#include "stack.h"
#include "algorythm.h"
#include "func_io.h"

#define TIME_REPEATS 10

#define GET_TIME(__func__)                        \
    do                                            \
    {                                             \
        uint64_t res = 0, start, end;             \
        for (size_t i = 0; i < TIME_REPEATS; i++) \
        {                                         \
            start = tick();                       \
            __func__;                             \
            end = tick();                         \
            res += end - start;                   \
        }                                         \
        res /= TIME_REPEATS;                      \
        printf("%llu\n", res);                    \
    } while (0)

int main(void)
{
    system("clear");
    welcome();
    int cur_command = 0;
    char symbol;

    a_stack_t *arr = NULL;

    l_stack_t *list = NULL;

    a_stack_t *freed_mem = NULL;
    INIT_STACK(&freed_mem, 1000, sizeof(void *));

    double temp = 0.0;
    do
    {
        if (2 != scanf("%d%c", &cur_command, &symbol) || '\n' != symbol)
        {
            CLEAN_INPUT_THREAD;
            WRONG_CMD_MSG;
            continue;
        }

        if (!cur_command)
            break;

        switch (cur_command)
        {
        case 1:
            READ_STACK(arr, double, "%lf");
            break;
        case 2:
        {
            INPUT_STACK_ELEM_MSG;

            if (1 != scanf("%lf", &temp))
            {
                CLEAN_INPUT_THREAD;
                VAL_ERR_MSG;
                break;
            }

            // GET_TIME(PUSH(arr, &temp));

            switch (PUSH(arr, &temp))
            {
            case STACK_OK:
                INPUT_STACK_SUCCESS_MSG;
                continue;
            case NON_INIT_STACK_ERR:
                NON_INIT_STACK_ERR_MSG;
                continue;
            case STACK_OVERFLOW_ERR:
                STACK_OVERFLOW_ERR_MSG;
                continue;
            default:
                INTERNAL_ERR_MSG;
                continue;
            }
        }
        case 3:
        {
            // GET_TIME(POP(arr, &temp));

            switch (POP(arr, &temp))
            {
            case STACK_OK:
                OUTPUT_ELEM_MSG("%.6lf", temp);
                continue;
            case NON_INIT_STACK_ERR:
                NON_INIT_STACK_ERR_MSG;
                continue;
            case MEM_ACCESS_ERR:
                EMPTY_STACK_MSG;
                continue;
            case MEM_ALLOC_ERR:
                INTERNAL_ERR_MSG;
                continue;
            default:
                WTFII_MSG;
                continue;
            }
        }
        case 4:
            calculate_arr();
            break;
        case 5:
            PRINT_STACK(arr, a_iterator, double, "%.6lf");
            break;
        case 6:
            READ_STACK(list, double, "%lf");
            break;
        case 7:
        {
            INPUT_STACK_ELEM_MSG;

            if (1 != scanf("%lf", &temp))
            {
                CLEAN_INPUT_THREAD;
                VAL_ERR_MSG;
                break;
            }

            // GET_TIME(PUSH(list, &temp));

            switch (PUSH(list, &temp))
            {
            case STACK_OK:
                INPUT_STACK_SUCCESS_MSG;
                continue;
            case NON_INIT_STACK_ERR:
                NON_INIT_STACK_ERR_MSG;
                continue;
            case STACK_OVERFLOW_ERR:
                STACK_OVERFLOW_ERR_MSG;
                continue;
            default:
                INTERNAL_ERR_MSG;
                continue;
            }
        }
        case 8:
        {
            void *ptr = NULL;
            if (TOP(list, &temp, &ptr))
            {
                EMPTY_STACK_MSG;
                continue;
            }

            // GET_TIME(POP(list, &temp));

            PUSH(freed_mem, ptr);

            switch (POP(list, &temp))
            {
            case STACK_OK:
                OUTPUT_ELEM_MSG("%.6lf", temp);
                continue;
            case NON_INIT_STACK_ERR:
                NON_INIT_STACK_ERR_MSG;
                continue;
            case MEM_ACCESS_ERR:
                EMPTY_STACK_MSG;
                continue;
            case MEM_ALLOC_ERR:
                INTERNAL_ERR_MSG;
                continue;
            default:
                WTFII_MSG;
                continue;
            }
        }
        case 9:
            PRINT_STACK(freed_mem, a_iterator, void *, "%p");
            break;
        case 10:
            calculate_list();
            break;
        case 11:
            PRINT_STACK(list, l_iterator, double, "%.6lf");
            break;
        default:
            WRONG_CMD_MSG;
            break;
        }
    } while (1);

    FREE(&list);
    FREE(&arr);
    FREE(&freed_mem);

    return OK;
}
