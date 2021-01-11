#ifndef __COMMON_H__
#define __COMMON_H__

#define OK 0

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_RESET "\x1b[0m"

#define CLEAN_INPUT_THREAD                                  \
    do                                                      \
    {                                                       \
        int __c__;                                          \
        while ('\n' != (__c__ = getchar()) && EOF != __c__) \
            ;                                               \
    } while (0)

#define WRONG_CMD_MSG \
    printf(ANSI_RED "Неверная команда, попробуйте еще раз\n" ANSI_RESET)

#define INPUT_STACK_CAP_MSG \
    printf("Введите емкость стека:\n\t")

#define INPUT_STACK_SIZE_MSG \
    printf("Введите кол-во элементов стека:\n\t")

#define INPUT_STACK_ELEMS_MSG \
    printf("Введите элементы стека:\n")

#define INPUT_STACK_ELEM_MSG \
    printf("Введите значение элемента:\n\t")

#define INPUT_STACK_SUCCESS_MSG \
    printf(ANSI_GREEN "Стек сохранен\n" ANSI_RESET)

#define VAL_ERR_MSG \
    printf(ANSI_RED "Что-то не так с введенным элементом\n" ANSI_RESET)

#define STACK_PARAMS_ERR_MSG \
    printf(ANSI_RED "Некорректные параметры стека\n" ANSI_RESET)

#define NON_INIT_STACK_ERR_MSG \
    printf(ANSI_RED "Похоже, что стек не инициализирован\n" ANSI_RESET)

#define EMPTY_STACK_MSG \
    printf(ANSI_RED "Похоже, что стек пуст\n" ANSI_RESET)

#define STACK_OVERFLOW_ERR_MSG \
    printf(ANSI_RED "Стек переполнен\n" ANSI_RESET)

#define INTERNAL_ERR_MSG \
    printf(ANSI_RED "Произошла внутренняя ошибка, похоже, ничего не вышло\n" ANSI_RESET)

#define WTFII_MSG \
    printf(ANSI_RED "Пошло не так то, о чем мы даже не знаем. Перезапусти, от греха\n" ANSI_RESET)

#define OUTPUT_ELEM_MSG(__fmt__, __val__) \
    printf(ANSI_GREEN "Результат : " __fmt__ ANSI_RESET "\n", __val__)

#define OUTPUT_TICKS_MSG(__fmt__, __val__) \
    printf(ANSI_GREEN "Время выполнения : " __fmt__ ANSI_RESET "\n", __val__)

#define CONVERTATION_ERR_MSG \
    printf(ANSI_RED "Не получилось перевести выражеение в польскую нотацию\n" ANSI_RESET)

#define CALCULATION_ERR_MSG \
    printf(ANSI_RED "Не получилось вычислить выражеение\n" ANSI_RESET)

#endif
