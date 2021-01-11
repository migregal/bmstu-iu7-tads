#include <stdio.h>
#include <math.h>

#include "common.h"
#include "utils.h"
#include "ops.h"

#define DOUBLE_FMT "%.3lf"
#define P 0.7

#define max(__a__, __b__) \
    (__a__ > __b__ ? __a__ : __b__)

static inline void avg_stat(unsigned in, unsigned out, double t)
{
    if (out % 100 == 0)
    {
        printf(ANSI_YELLOW
               "\n---\n"
               "Вошло в 1ый автомат:    %u заявок.\n"
               "Вышло из 2ого автомата: %u заявок.\n"
               "Рабочее время :         " DOUBLE_FMT "\n"
               "---\n" ANSI_RESET,
               in, out, t);
    }
}

void list_model(
    list_t *const queue1,
    list_t *const queue2,
    f_arr_t *const fmem,
    const int t_1_1, const int t_1_2,
    const int t_2_1, const int t_2_2)
{
    double service_time_1 = 0,
           service_time_2 = 0;
    double hold_time_1 = 0,
           hold_time_2 = 0;

    unsigned in_tasks = -1,
             out_tasks = 0,
             failed_tasks_1 = 0,
             failed_tasks_2 = 0;

    unsigned reusedmem = 0,
             newmem = 0;

    task_t *task1 = NULL, *task2 = NULL;
    while (TASKS_COUNT >= out_tasks)
    {
        in_tasks++;

        if (dequeuelist(queue1, &task1, fmem))
        {
            while (!dequeuelist(queue2, &task2, fmem))
            {
                hold_time_1 += task2->time2;
                service_time_2 += task2->time2;
                out_tasks++;
                avg_stat(in_tasks, out_tasks, max(service_time_1, service_time_2));

                free(task2), task2 = NULL;
            }
            break;
        }

        double time1 = task1->time1;
        service_time_1 += task1->time1;
        avg_stat(in_tasks, out_tasks, max(service_time_1, service_time_2));
        while (time1 > 0)
        {

            if (toplist(queue2, &task2))
            {
                hold_time_2 += time1;
                task2 = NULL;
                break;
            }

            if (time1 > task2->time2)
            {
                dequeuelist(queue2, &task2, fmem);
                time1 -= task2->time2;
                service_time_2 += task2->time2;
                out_tasks++;
                avg_stat(in_tasks, out_tasks, max(service_time_1, service_time_2));

                free(task2), task2 = NULL;
                continue;
            }

            task2->time2 -= time1;
            service_time_2 += time1;

            task2 = NULL;
            break;
        }

        if (get_rand_double(0, 1) > P)
        {
            if (enqueuelist(queue1, *task1))
                failed_tasks_1++;
            else if (check_rear(queue1, fmem))
                reusedmem++;
            else
                newmem++;

            free(task1), task1 = NULL;
            continue;
        }

        if (enqueuelist(queue2, *task1))
            failed_tasks_2++;
        else if (check_rear(queue2, fmem))
            reusedmem++;
        else
            newmem++;

        free(task1), task1 = NULL;
    }

    double expected_time = (double)(t_1_1 + t_1_2) / 2 * in_tasks;
    double delta = fabs(service_time_1 - expected_time) / expected_time * 100;
    double service_time = service_time_1;

    if (expected_time < (double)(t_2_1 + t_2_2) / 2 * out_tasks)
    {
        expected_time = (double)(t_2_1 + t_2_2) / 2 * out_tasks;
        delta = fabs(service_time_2 - expected_time) / expected_time * 100;
        service_time = service_time_2;
    }

    printf(ANSI_GREEN
           "Рабочее время автомата: " DOUBLE_FMT " (ожидаемое рабочее время: " DOUBLE_FMT ", погрешность: " DOUBLE_FMT "%%)\n"
           "Число вошедших заявок: %d\n"
           "Число вышедших заявок: %d\n"
           "Количество адресов, взятых из использованной памяти: %d\n"
           "Количество адресов, взятых из новой памяти: %d\n" ANSI_RESET,
           service_time, expected_time, delta,
           in_tasks, out_tasks, reusedmem, newmem);
    printf(ANSI_GREEN
           "\nПервый автомат:\n"
           "Число необработанных заявок: %d\n"
           "Время простоя автомата: " DOUBLE_FMT "\n" ANSI_RESET,
           failed_tasks_1, hold_time_1);
    printf(ANSI_GREEN
           "\nВторой автомат:\n"
           "Число необработанных заявок: %d\n"
           "Время простоя автомата: " DOUBLE_FMT "\n" ANSI_RESET,
           failed_tasks_2, hold_time_2);
}

void array_model(arr_t *const queue1,
                 arr_t *const queue2,
                 const int t_1_1, const int t_1_2,
                 const int t_2_1, const int t_2_2)
{
    unsigned failed_tasks_1 = 0,
             failed_tasks_2 = 0;
    double service_time_1 = 0,
           service_time_2 = 0;
    double hold_time_1 = 0,
           hold_time_2 = 0;

    unsigned in_tasks = 0;
    unsigned out_tasks = 0;

    task_t *task1 = NULL, *task2 = NULL;
    while (TASKS_COUNT >= out_tasks)
    {
        in_tasks++;

        if (dequeuearr(queue1, &task1))
        {
            while (!dequeuearr(queue2, &task2))
            {
                hold_time_1 += task2->time2;
                service_time_2 += task2->time2;
                out_tasks++;
                avg_stat(in_tasks, out_tasks, max(service_time_1, service_time_2));
            }
            break;
        }

        double time1 = task1->time1;
        service_time_1 += task1->time1;
        while (time1 > 0)
        {
            if (toparr(queue2, &task2))
            {
                hold_time_2 += time1;
                break;
            }

            if (time1 > task2->time2)
            {
                dequeuearr(queue2, &task2);
                time1 -= task2->time2;
                service_time_2 += task2->time2;
                out_tasks++;
                avg_stat(in_tasks, out_tasks, max(service_time_1, service_time_2));
                continue;
            }

            task2->time2 -= time1;
            service_time_2 += time1;
            break;
        }

        if (get_rand_double(0, 1) > P)
        {
            if (enqueuearr(queue1, *task1))
                failed_tasks_1++;
        }
        else if (enqueuearr(queue2, *task1))
            failed_tasks_2++;
    }

    double expected_time = (double)(t_1_1 + t_1_2) / 2 * in_tasks;
    double delta = fabs(service_time_1 - expected_time) / expected_time * 100;
    double service_time = service_time_1;

    if (expected_time < (double)(t_2_1 + t_2_2) / 2 * out_tasks)
    {
        expected_time = (double)(t_2_1 + t_2_2) / 2 * out_tasks;
        delta = fabs(service_time_2 - expected_time) / expected_time * 100;
        service_time = service_time_2;
    }

    printf(ANSI_GREEN
           "Рабочее время: " DOUBLE_FMT " (ожидаемое рабочее время: " DOUBLE_FMT ", погрешность: " DOUBLE_FMT "%%)\n"
           "Число вошедших заявок: %d\n"
           "Число вышедших заявок: %d\n" ANSI_RESET,
           service_time, expected_time, delta,
           in_tasks, out_tasks);
    printf(ANSI_GREEN
           "\nПервый автомат:\n"
           "Число необработанных заявок: %d\n"
           "Время простоя автомата: " DOUBLE_FMT "\n" ANSI_RESET,
           failed_tasks_1, hold_time_1);
    printf(ANSI_GREEN
           "\nВторой автомат:\n"
           "Число необработанных заявок: %d\n"
           "Время простоя автомата: " DOUBLE_FMT "\n" ANSI_RESET,
           failed_tasks_2, hold_time_2);
}
