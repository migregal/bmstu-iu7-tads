#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "longdouble.h"
#include "io_formats.h"

#define OK 0
#define INPUT_ERROR 1
#define NUMBER_FORMAT_ERROR 2

#define TRUE 1
#define FALSE 0

int input_long_int(char **str)
{
    printf("\nВвод целого числа:\n");
    printf("Корректный формат: +77, -35, 0003\n");
    printf("1        10        20       30\n");
    printf("|--------|---------|---------|\n");

    *str = (char *)malloc(31 * sizeof(char));

    if (*str == NULL)
        return MEMORY_ERROR;

    scanf("%s", *str);

    char *t = (char *)realloc(*str, strlen(*str));
    if (t == NULL)
    {
        free(*str);
        return MEMORY_ERROR;
    }
    *str = t;

    return OK;
}

int input_long_double(char **str)
{
    printf("\nВвод вещественного числа:\n");
    printf("Обязательно вводите e/E.\n");
    printf("Корректный формат: -1e0, 0.5e-7, +.5e1\n\n");
    printf("1        10        20       30e1   5\n");
    printf("|--------|---------|---------|-|---|\n");

    *str = (char *)malloc(37 * sizeof(char));

    if (*str == NULL)
        return MEMORY_ERROR;

    scanf("%s", *str);

    char *t = (char *)realloc(*str, strlen(*str));
    if (t == NULL)
    {
        free(*str);
        return MEMORY_ERROR;
    }
    *str = t;

    return OK;
}

int main(void)
{
    char *str = NULL;

    if (input_long_int(&str) != OK)
        return INPUT_ERROR;

    LongDouble *val_a = NULL;

    if (newLongDouble(&val_a, str, TRUE) != OK)
    {
        printf(ANSI_COLOR_RED "\vНеверный формат целого числа!\n\v" ANSI_COLOR_RESET);
        free(str);

        return NUMBER_FORMAT_ERROR;
    }
    free(str);

    if (input_long_double(&str) != OK)
        return INPUT_ERROR;

    LongDouble *val_b = NULL;

    if (newLongDouble(&val_b, str, FALSE) != OK)
    {
        printf(ANSI_COLOR_RED "\vНеверный формат вещ-ого числа!\n\v" ANSI_COLOR_RESET);

        free(str);
        deleteLongDouble(&val_a);

        return NUMBER_FORMAT_ERROR;
    }
    free(str);

    LongDouble *res = NULL;
    if (newEmptyLongDouble(&res) != 0)
    {
        deleteLongDouble(&res);

        return NUMBER_FORMAT_ERROR;
    }

    if (val_b->multiply(val_a, val_b, res) != OK)
    {
        printf(ANSI_COLOR_RED "\vОшибка умножения. Переполнение порядка.\n\v" ANSI_COLOR_RESET);

        deleteLongDouble(&val_a);
        deleteLongDouble(&val_b);
        deleteLongDouble(&res);

        return NUMBER_FORMAT_ERROR;
    }

    str = NULL;
    res->toString(res, &str);
    printf(ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, str);
    free(str);

    deleteLongDouble(&val_a);
    deleteLongDouble(&val_b);
    deleteLongDouble(&res);

    return OK;
}
