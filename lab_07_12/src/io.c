#include "io.h"

/*
Welcomming print with available features.
*/
void welcome()
{
    printf("%s",
           ANSI_MAGENTA
           "Данная программа позволяет смоделировать поиск минимального по количеству \n"
           "ребер подмножества ребер, удаление которого превращает заданный граф в несвязный. \n\n" ANSI_RESET);
}

/*
Clean input stream from trash.
*/
void clinstream()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

/*
Input number in between.

Input data:
* int *const num - number of action to be done.
* const int left - left border of between.
* const int right - right border of between.
*/
int rnginput(int *const num, const int left, const int right)
{
    if (scanf("%d", num) != GOT_ARG)
    {
        clinstream();
        return EINVALIDINTEGER;
    }

    if (*num < left || *num > right)
    {
        clinstream();
        return EINVALIDRANGE;
    }

    return EOK;
}
