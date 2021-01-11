#include <limits.h>
#include "graph.h"
#include "utils.h"

#define REPEATS 1

int main()
{
    int rc;

    welcome();

    printf(ANSI_GREEN
           "Введите номер структуры для представления графа:\n"
           "    0. Матрица смежности\n"
           "    1. Список смежности\n"
           "    " ANSI_RESET);

    int type = 0;
    if ((rc = rnginput(&type, 0, 1)))
    {
        printf(ANSI_RED
               "Введено недопустимое значение! Повторите попытку.\n" ANSI_RESET);
        return rc;
    }

    if (0 == type)
    {
        printf(ANSI_GREEN
               "Выбрана матрица смежности!\n\n" ANSI_RESET);

        printf(ANSI_GREEN
               "Введите число вершин графа: " ANSI_RESET);

        int size;
        if ((rc = rnginput(&size, 1, INT_MAX)))
        {
            printf(ANSI_RED
                   "Введено недопустимое значение! Повторите попытку.\n" ANSI_RESET);
            return rc;
        }

        adjmat_t *matrix = amcreate(size);
        printf(ANSI_GREEN
                "Введите связи в графе (в формате 'номер_вершины номер_вершины').\n"
                "Для завершения ввода введите '-1' (нумерация вершин начинается с 0):\n" ANSI_RESET);
        if ((rc = FILL(stdin, matrix)))
        {
            printf(ANSI_RED
                   "Введено недопустимое значение! Повторите попытку.\n" ANSI_RESET);
            amfree(matrix);
            return rc;
        }

        uint64_t start, end, time = 0;

        int *groups = (type_t *)calloc(matrix->size, sizeof(type_t));

        for (int i = 0; i < REPEATS; i++)
        {
            start = tick();
            rc = IS_CONN(*matrix, &groups);
            end = tick();

            time += end - start;
        }

        time /= REPEATS;

        if (!rc)
            printf(ANSI_YELLOW
                   "Несвязные подграфы отмечены различными цветами!\n" ANSI_RESET);
        else
            printf(ANSI_YELLOW "Граф связен!\n" ANSI_RESET);

        printf(ANSI_YELLOW
               "Время выполнения алгоритма: %ju\n" ANSI_RESET,
               time);

        GVEXPORT(*matrix, groups);

        free(groups);
        FREE(matrix);
    }
    else
    {
        printf(ANSI_GREEN
               "Выбран список смежности!\n\n" ANSI_RESET);

        printf(ANSI_GREEN
               "Введите число вершин графа: " ANSI_RESET);

        int size;
        if ((rc = rnginput(&size, 1, INT_MAX)))
        {
            printf(ANSI_RED
                   "Введено недопустимое значение! Повторите попытку.\n" ANSI_RESET);
            return rc;
        }

        listmat_t *matrix = lmcreate(size);
        printf(ANSI_GREEN
               "Введите связи в графе (в формате 'номер_вершины номер_вершины').\n"
               "Для завершения ввода введите '-1' (нумерация вершин начинается с 0):\n" ANSI_RESET);
        if ((rc = FILL(stdin, matrix)))
        {
            printf(ANSI_RED
                   "Введено недопустимое значение! Повторите попытку.\n" ANSI_RESET);
            lmfree(matrix);
            return rc;
        }

        for (int i = 0; i < matrix->size; i++)
            remove_duplicates(&(matrix->matrix[i]), cmp);

        uint64_t start, end, time = 0;

        type_t *groups = (type_t *)calloc(matrix->size, sizeof(type_t));

        for (int i = 0; i < REPEATS; i++)
        {
            start = tick();
            rc = IS_CONN(*matrix, &groups);
            end = tick();

            time += end - start;
        }

        time /= REPEATS;

        if (!rc)
            printf(ANSI_YELLOW
                   "Несвязные подграфы отмечены различными цветами!\n" ANSI_RESET);
        else
            printf(ANSI_YELLOW "Граф связен!\n" ANSI_RESET);

        printf(ANSI_YELLOW
               "Время выполнения алгоритма: %ju\n" ANSI_RESET,
               time);

        GVEXPORT(*matrix, groups);

        free(groups);
        FREE(matrix);
    }

    return EOK;
}
