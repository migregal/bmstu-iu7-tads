#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "my_menu.h"
#include "io.h"
#include "table.h"

#define SORT_REPEATS 10000

#define RES_IS_EMPTY "Вывод пуст."
#define TABLE_IS_EMPTY "Таблица пуста."

void print_header(void)
{
    welcome_print();
}

void print_menu(void)
{
    menu_print();
}

bool input_cmd(char *cmd)
{
    if (input_string(cmd) != IO_OK)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, "Введена недопустимая команда! Повторите попытку.");
        return false;
    }

    return true;
}

void load_file_csv(aio_table_t *const table)
{
    char filename[MAX_STRING_FIELD_SIZE];

    printf(ANSI_YELLOW "%s" ANSI_RESET, "Введите имя файла: ");
    input_string(filename);

    aio_table_t temp;

    switch (upload_table_from_csv(filename, &temp))
    {
    case IO_FILE_NOT_EXIST_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Файл не существует.");
        break;

    case IO_EMPTY_FILE_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Файл пуст.");
        break;

    case IO_INVALID_INPUT_ERROR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Данные в файле некорректны.");
        break;

    default:
        memcpy(table, &temp, sizeof(aio_table_t));
        printf(ANSI_GREEN "%s\n" ANSI_RESET, "Файл успешно загружен.");
        break;
    }
}

void save_file_csv(const aio_table_t *const table)
{
    if (table->size_of_table == 0)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, TABLE_IS_EMPTY);
        return;
    }

    char filename[MAX_STRING_FIELD_SIZE];

    printf(ANSI_YELLOW "%s" ANSI_RESET, "Введите имя файла: ");
    input_string(filename);

    switch (save_table_to_csv(filename, table))
    {
    case IO_FILE_NOT_EXIST_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Не удалось открыть файл для сохранения.");
        break;

    case IO_WRITE_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка записи в файл.");
        break;

    default:
        printf(ANSI_GREEN "%s\n" ANSI_RESET, "Файл успешно сохранен.");
        printf(ANSI_GREEN "Сохранено %d строк\n" ANSI_RESET, table->size_of_table);
        break;
    }
}

void add_record(aio_table_t *const table)
{
    int add_res = add(table);

    switch (add_res)
    {
    case TABLE_OK:
        printf(ANSI_GREEN "%s\n" ANSI_RESET, "Запись успешно добавлена.");
        break;

    case TABLE_OVERFLOW_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Достигнут максимальный размер таблицы.");
        break;
    case SURNAME_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода фамилии.");
        break;
    case NAME_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода имени.");
        break;
    case GROUP_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода группы.");
        break;
    case SEX_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода пола.");
        break;
    case AVG_MARK_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода среднего балла за сессию.");
        break;
    case DATE_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода даты поступления.");
        break;
    case TYPE_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода типа жилья.");
        break;
    case DORMITORY_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода номера общежития.");
        break;
    case ROOM_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода номера комнаты.");
        break;
    case STREET_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода улицы.");
        break;
    case HOUSE_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода номера дома.");
        break;
    case FLAT_INPUT_ERR:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода номера квартиры.");
        break;
    default:
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода. Повторите попытку.");
        break;
    }
}

void list(const aio_table_t *const table)
{
    if (table->size_of_table == 0)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, TABLE_IS_EMPTY);
        return;
    }

    printf(ANSI_YELLOW "%s" ANSI_RESET, "Введите год поступления: ");

    int year;
    if (input_number(&year, 1900, 2020) != IO_OK)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, "Введен неверный год. Допустимы с 1900 по 2020");
        return;
    }

    aio_table_t temp;
    temp.size_of_table = 0;

    for (int i = 0; i < table->size_of_table; ++i)
        if (table->main_table[i].year == year)
            temp.main_table[temp.size_of_table++] = table->main_table[i];

    print(&temp);
}

void delete_record(aio_table_t *const table)
{
    if (table->size_of_table == 0)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, TABLE_IS_EMPTY);
        return;
    }

    float mark;
    printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите средний балл для удаления: ");

    if (scanf("%f", &mark) != 1)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, "Ошибка ввода");
        return;
    }

    int deletions_count = delete_record_by_avg_mark(table, mark);

    printf(ANSI_GREEN
           "%s%d%s\n" ANSI_RESET,
           "Удалено(-а) ", deletions_count, " записей(-ь)");
}

void k_sort(aio_table_t *const table)
{
    if (table->size_of_table == 0)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, TABLE_IS_EMPTY);
        return;
    }

    aio_table_t temp;

    uint64_t result = 0;

    for (int i = 0; i < SORT_REPEATS; ++i)
    {
        memcpy(&temp, table, sizeof(aio_table_t));

        uint64_t start, end;

        start = tick();
        bubble_sort_table(&temp, false);
        end = tick();

        result += end - start;
        printf("\r%d/%d", i, SORT_REPEATS);
    }

    result /= SORT_REPEATS;

    printf(ANSI_GREEN
           "\r%s%ju\n" ANSI_RESET,
           "Время сортировки таблицы ключей"
           " при помощи сортировки пузырьком = ",
           result);

    print_k(&temp);
}

void k_sort_q(aio_table_t *const table)
{
    if (table->size_of_table == 0)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, TABLE_IS_EMPTY);
        return;
    }

    aio_table_t temp;

    uint64_t result = 0;

    for (int i = 0; i < SORT_REPEATS; ++i)
    {
        memcpy(&temp, table, sizeof(aio_table_t));

        uint64_t start, end;

        start = tick();
        quick_sort_table(&temp, false);
        end = tick();

        result += end - start;
        printf("\r%d/%d", i, SORT_REPEATS);
    }

    result /= SORT_REPEATS;

    printf(ANSI_GREEN
           "\r%s%ju\n" ANSI_RESET,
           "Время сортировки таблицы ключей"
           " при помощи быстрой сортировки = ",
           result);

    print_k(&temp);
}

void sort(aio_table_t *const table)
{
    if (table->size_of_table == 0)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, TABLE_IS_EMPTY);
        return;
    }

    aio_table_t temp;

    uint64_t result = 0;

    for (int i = 0; i < SORT_REPEATS; ++i)
    {
        memcpy(&temp, table, sizeof(aio_table_t));

        uint64_t start, end;

        start = tick();
        bubble_sort_table(&temp, true);
        end = tick();

        result += end - start;

        printf("\r%d/%d", i, SORT_REPEATS);
    }

    result /= SORT_REPEATS;

    printf(ANSI_GREEN "\r%s%ju\n" ANSI_RESET,
           "Время сортировки таблицы при помощи сортировки пузырьком = ",
           result);

    print(&temp);
}

void sort_q(aio_table_t *const table)
{
    if (table->size_of_table == 0)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, TABLE_IS_EMPTY);
        return;
    }

    aio_table_t temp;

    uint64_t result = 0;

    for (int i = 0; i < SORT_REPEATS; ++i)
    {
        memcpy(&temp, table, sizeof(aio_table_t));

        uint64_t start, end;

        start = tick();
        quick_sort_table(&temp, true);
        end = tick();

        result += end - start;

        printf("\r%d/%d", i, SORT_REPEATS);
    }

    result /= SORT_REPEATS;

    printf(ANSI_GREEN "\n%s%ju\n" ANSI_RESET,
           "Время сортировки таблицы при помощи быстрой сортировки = ",
           result);

    print(&temp);
}

void keys(const aio_table_t *const table)
{
    if (table->size_of_table == 0)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, TABLE_IS_EMPTY);
        return;
    }

    aio_table_t temp;
    memcpy(&temp, table, sizeof(aio_table_t));

    quick_sort_table(&temp, false);

    output_key_table(&temp);
}

void print(const aio_table_t *const table)
{
    if (table->size_of_table == 0)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, RES_IS_EMPTY);
        return;
    }

    output_main_table(table);
}

void print_k(const aio_table_t *const table)
{
    if (table->size_of_table == 0)
    {
        printf(ANSI_RED "%s\n" ANSI_RESET, RES_IS_EMPTY);
        return;
    }

    output_main_table_by_keys(table);
}

void cmp(aio_table_t *const table)
{
    aio_table_t temp;

    uint64_t result = 0;

    for (int i = 0; i < SORT_REPEATS; ++i)
    {
        memcpy(&temp, table, sizeof(aio_table_t));

        uint64_t start, end;

        start = tick();
        bubble_sort_table(&temp, true);
        end = tick();

        result += end - start;

        printf("\r%d/%d", i, SORT_REPEATS);
    }

    result /= SORT_REPEATS;

    printf(ANSI_GREEN
           "\r%s%ju\n" ANSI_RESET,
           "Время сортировки основной таблицы"
           " при помощи сортировки пузырьком             = ",
           result);

    for (int i = 0; i < SORT_REPEATS; ++i)
    {
        memcpy(&temp, table, sizeof(aio_table_t));

        uint64_t start, end;

        start = tick();
        quick_sort_table(&temp, true);
        end = tick();

        result += end - start;

        printf("\r%d/%d", i, SORT_REPEATS);
    }

    result /= SORT_REPEATS;

    printf(ANSI_GREEN
           "\r%s%ju\n" ANSI_RESET,
           "Время сортировки основной таблицы"
           " при помощи быстрой сортировки               = ",
           result);

    for (int i = 0; i < SORT_REPEATS; ++i)
    {
        memcpy(&temp, table, sizeof(aio_table_t));

        uint64_t start, end;

        start = tick();
        bubble_sort_table(&temp, false);
        end = tick();

        result += end - start;

        printf("\r%d/%d", i, SORT_REPEATS);
    }

    result /= SORT_REPEATS;

    printf(ANSI_GREEN
           "\r%s%ju\n" ANSI_RESET,
           "Время сортировки и вывода основной таблицы"
           " при помощи таблицы ключей (bubble) = ",
           result);

    for (int i = 0; i < SORT_REPEATS; ++i)
    {
        memcpy(&temp, table, sizeof(aio_table_t));

        uint64_t start, end;

        start = tick();
        quick_sort_table(&temp, false);
        end = tick();

        result += end - start;

        printf("\r%d/%d", i, SORT_REPEATS);
    }

    result /= SORT_REPEATS;

    printf(ANSI_GREEN
           "\r%s%ju\n" ANSI_RESET,
           "Время сортировки и вывода основной таблицы"
           " при помощи таблицы ключей (quick)  = ",
           result);
}

void unknown_cmd(void)
{
    printf(ANSI_RED "%s\n" ANSI_RESET, "Введена недопустимая команда. Повторите попытку");
}

bool process_menu(aio_table_t *table)
{
    char cur_command[10];

    static bool showed;

    if (!showed)
    {
        system("clear");
        print_header();
        print_menu();
        showed = true;
    }

    if (!input_cmd(cur_command))
        return true;

    if (!strcmp(cur_command, "menu"))
    {
        print_menu();
        return true;
    }

    if (!strcmp(cur_command, "load"))
    {
        load_file_csv(table);
        return true;
    }

    if (!strcmp(cur_command, "save"))
    {
        save_file_csv(table);
        return true;
    }

    if (!strcmp(cur_command, "add"))
    {
        add_record(table);
        return true;
    }

    if (!strcmp(cur_command, "list"))
    {
        list(table);
        return true;
    }

    if (!strcmp(cur_command, "delete"))
    {
        delete_record(table);
        return true;
    }

    if (!strcmp(cur_command, "k_sort"))
    {
        k_sort(table);
        return true;
    }

    if (!strcmp(cur_command, "k_sort_q"))
    {
        k_sort_q(table);
        return true;
    }

    if (!strcmp(cur_command, "sort"))
    {
        sort(table);
        return true;
    }

    if (!strcmp(cur_command, "sort_q"))
    {
        sort_q(table);
        return true;
    }

    if (!strcmp(cur_command, "keys"))
    {
        keys(table);
        return true;
    }

    if (!strcmp(cur_command, "print"))
    {
        print(table);
        return true;
    }

    if (!strcmp(cur_command, "cmp"))
    {
        cmp(table);
        return true;
    }

    if (!strcmp(cur_command, "clear"))
    {
        system("clear");
        print_menu();
        return true;
    }

    if (!strcmp(cur_command, "exit"))
        return false;

    unknown_cmd();

    return true;
}
