#include <time.h>
#include <stdio.h>

#include "common.h"
#include "utils.h"

#include "arr_queue.h"
#include "list_queue.h"
#include "ops.h"

#define MAX_INPUT_VALUE 99999
#define REPEATS_COUNT 1000

#define checked_input(__var__, __a__, __b__)                                               \
    if (input_interval(&__var__, __a__, __b__))                                            \
    {                                                                                      \
        printf(ANSI_RED "Введено недопустимое значение! Повторите попытку.\n" ANSI_RESET); \
        continue;                                                                          \
    }

void welcome();

char input_interval(int *const num, const int left, const int right);

int main(void)
{
    srand(time(0));
    welcome();
    int cmd = 1;

    while (cmd)
    {
        checked_input(cmd, 0, 4);

        switch (cmd)
        {
        case 1:
        {
            int capacity;

            printf("%s\n", "Введите емкость очереди:");
            checked_input(capacity, 0, MAX_INPUT_VALUE);

            int t_1_1, t_1_2;

            printf("%s\n", "Введите интервал обработки заявки в 1ой очереди (2 целых числа через пробел):");
            checked_input(t_1_1, 0, MAX_INPUT_VALUE);
            checked_input(t_1_2, 0, MAX_INPUT_VALUE);

            if (t_1_2 < t_1_1)
            {
                printf(ANSI_RED "%s\n" ANSI_RESET,
                       "Правая граница должна быть больше левой! Повторите попытку.");

                continue;
            }

            int t_2_1, t_2_2;

            printf("%s\n", "Введите интервал обработки заявки во 2ой очереди (2 целых числа через пробел):");
            checked_input(t_2_1, 0, MAX_INPUT_VALUE);
            checked_input(t_2_2, 0, MAX_INPUT_VALUE);

            if (t_2_2 < t_2_1)
            {
                printf(ANSI_RED "%s\n" ANSI_RESET,
                       "Правая граница должна быть больше левой! Повторите попытку.");

                continue;
            }

            arr_t *queue1 = NULL, *queue2 = NULL;

            create_queuearr(&queue1, capacity);
            create_queuearr(&queue2, capacity);

            for (int i = 0; i < capacity; ++i)
            {
                task_t task = {.time1 = get_rand_double(t_1_1, t_1_2),
                               .time2 = get_rand_double(t_2_1, t_2_2)};
                enqueuearr(queue1, task);
            }
            array_model(queue1, queue2, t_1_1, t_1_2, t_2_1, t_2_2);

            freequeuearr(queue2);
            freequeuearr(queue1);

            break;
        }
        case 2:
        {
            arr_t *test = NULL;
            create_queuearr(&test, REPEATS_COUNT);
            uint64_t start, end;

            start = tick();
            for (int i = 0; i < REPEATS_COUNT; ++i)
            {
                task_t task;
                enqueuearr(test, task);
            }
            end = tick();

            printf(ANSI_MAGENTA "%s %ju\n" ANSI_RESET,
                   "Добавление элементов в очередь на основе массива:",
                   (end - start) / REPEATS_COUNT);

            start = tick();
            for (int i = 0; i < REPEATS_COUNT; ++i)
            {
                task_t *temp = NULL;
                dequeuearr(test, &temp);
            }
            end = tick();

            freequeuearr(test);

            printf(ANSI_MAGENTA "%s %ju\n" ANSI_RESET,
                   "Удаление элементов из очереди на основе массива:",
                   (end - start) / REPEATS_COUNT);
            break;
        }
        case 3:
        {
            int capacity;

            printf("%s\n", "Введите емкость очереди:");
            checked_input(capacity, 0, MAX_INPUT_VALUE);

            int t_1_1, t_1_2;

            printf("%s\n", "Введите интервал обработки заявки в 1ой очереди (2 целых числа через пробел):");
            checked_input(t_1_1, 0, MAX_INPUT_VALUE);
            checked_input(t_1_2, 0, MAX_INPUT_VALUE);

            if (t_1_2 < t_1_1)
            {
                printf(ANSI_RED "%s\n" ANSI_RESET,
                       "Правая граница должна быть больше левой! Повторите попытку.");

                continue;
            }

            int t_2_1, t_2_2;

            printf("%s\n", "Введите интервал обработки заявки во 2ой очереди (2 целых числа через пробел):");
            checked_input(t_2_1, 0, MAX_INPUT_VALUE);
            checked_input(t_2_2, 0, MAX_INPUT_VALUE);

            if (t_2_2 < t_2_1)
            {
                printf(ANSI_RED "%s\n" ANSI_RESET,
                       "Правая граница должна быть больше левой! Повторите попытку.");

                continue;
            }

            int output_mem_list = 0;
            printf("%s\n", "Вывести использованные адреса памяти? (0, 1)");
            checked_input(output_mem_list, 0, 1);

            list_t *queue1 = NULL, *queue2 = NULL;

            create_queuelist(&queue1, capacity);
            create_queuelist(&queue2, capacity);

            f_arr_t *f_mem = NULL;

            create_array(&f_mem, capacity * 10);

            for (int i = 0; i < capacity; ++i)
            {
                task_t task = {.time1 = get_rand_double(t_1_1, t_1_2),
                               .time2 = get_rand_double(t_2_1, t_2_2)};

                enqueuelist(queue1, task);
            }

            list_model(queue1, queue2, f_mem, t_1_1, t_1_2, t_2_1, t_2_2);

            freequeuelist(queue2, f_mem);
            freequeuelist(queue1, f_mem);

            if (output_mem_list)
                output_array(f_mem);
            free_array(&f_mem);

            break;
        }
        case 4:
        {
            list_t *test = NULL;
            create_queuelist(&test, REPEATS_COUNT);

            f_arr_t *fmem_test = NULL;
            create_array(&fmem_test, REPEATS_COUNT);

            uint64_t start, end;

            start = tick();
            for (int i = 0; i < REPEATS_COUNT; ++i)
            {
                task_t task;
                enqueuelist(test, task);
            }
            end = tick();

            printf(ANSI_MAGENTA "%s %ju\n" ANSI_RESET,
                   "Добавление элементов в очередь на основе списка:",
                   (end - start) / REPEATS_COUNT);

            start = tick();
            for (int i = 0; i < REPEATS_COUNT; ++i)
            {
                task_t *task = NULL;
                dequeuelist(test, &task, fmem_test);
                free(task);
            }
            end = tick();

            freequeuelist(test, fmem_test);

            free_array(&fmem_test);

            printf(ANSI_MAGENTA "%s %ju\n" ANSI_RESET,
                   "Удаление элементов из очереди на основе списка:",
                   (end - start) / REPEATS_COUNT);
        }
        default:
            break;
        }
    }
    return OK;
}

void clean_input_stream()
{
    for (char c = 0; (c = getchar()) != '\n' && c != EOF;)
    {
    }
}

char input_interval(int *const num, const int left, const int right)
{
    if (scanf("%d", num) != 1)
    {
        clean_input_stream();
        return INVALID_INPUT;
    }

    if (*num < left || *num > right)
    {
        clean_input_stream();
        return INVALID_INPUT;
    }

    return OK;
}

void welcome()
{
    printf("%s",
           ANSI_MAGENTA
           "Данная программа позволяет сравнить время, затраченное на обработку \n"
           "очереди, реализованной на основе массива и односвязного линейного списка. \n"
           "Под обработкой подразумевается обработка деятельности обслуживающего аппарата. \n\n" ANSI_RESET);
    printf("%s",
           ANSI_YELLOW
           "Выберите действие из списка, введя соответствуюший номер:\n" ANSI_RESET);
    printf("%s",
           ANSI_GREEN
           "1. МАССИВ: Ввести данные обслуживающего аппарата и вывести статистику работы.\n"
           "2. МАССИВ: Вывести количественную характеристику выполнения операций над очередью.\n\n"
           "3. СПИСОК: Ввести данные обслуживающего аппарата и вывести статистику работы.\n"
           "4. СПИСОК: Вывести количественную характеристику выполнения операций над очередью.\n\n"
           "0. Выход из программы.\n\n" ANSI_RESET);
}
