#include <stdio.h>
#include <string.h>

#include "data_structures.h"
#include "defines.h"

void welcome()
{
    printf("%s",
           ANSI_COLOR_MAGENTA
           "Данная программа позволяет сравнить время, затраченное на сложение \n"
           "двух разреженных матриц, хранимых в стандартном виде и в разреженном \n"
           "столбцовом видах.\n\n" ANSI_COLOR_RESET);
    printf("%s",
           ANSI_COLOR_YELLOW
           "Выберите действие из списка, введя соответствуюший номер:\n" ANSI_COLOR_RESET);
    printf("%s",
           ANSI_COLOR_GREEN
           "1. Ввести матрицы вручную (в формате Matrix Market).\n"
           "2. Сгенерировать матрицы случайно (по проценту заполненности).\n"
           "3. Сложить матрицы, используя стандартный способ хранения.\n"
           "4. Сложить матрицы, используя разреженный столбцовый вид хранения.\n"
           "5. Вывести исходные и результирующую матрицы в стандартном виде.\n"
           "6. Вывести исходные и результирующую матрицы в разреженном столбцовом виде.\n"
           "7. Очистить экран.\n"
           "0. Выход из программы.\n\n" ANSI_COLOR_RESET);
}

void clean_input_stream()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int input_interval(type_t *const num, const type_t left, const type_t right)
{
    if (scanf(type_spec, num) != GOT_ARG)
    {
        clean_input_stream();
        return INVALID_INT_INPUT_ERROR;
    }

    if (*num < left || *num > right)
    {
        clean_input_stream();
        return INVALID_INTERVAL_ERROR;
    }

    return OK;
}
