#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "table.h"
#include "io.h"

int add(aio_table_t *const table)
{
    if (table->size_of_table == TABLE_SIZE)
        return TABLE_OVERFLOW_ERR;

    student_t new_record;
    char temp_str[10];
    int temp_num;
    char t;

    printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите фамилию студента (в английской раскладке): ");

    if (scanf("%64[^\n]%c", new_record.surname, &t) != 2 || t != '\n')
        return SURNAME_INPUT_ERR;

    printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите имя студента (в английской раскладке): ");

    if (scanf("%64[^\n]%c", new_record.name, &t) != 2 || t != '\n')
        return NAME_INPUT_ERR;

    printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите группу студента (в английской раскладке): ");

    if (scanf("%64[^\n]%c", new_record.group, &t) != 2 || t != '\n')
        return GROUP_INPUT_ERR;

    printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите пол студента (male/female): ");

    if (input_string(temp_str) != TABLE_OK)
        return SEX_INPUT_ERR;

    if (strcmp(temp_str, "male") == 0)
        new_record.sex = true;
    else if (strcmp(temp_str, "female") == 0)
        new_record.sex = false;
    else
        return SEX_INPUT_ERR;

    printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите средний балл студента: ");

    if (scanf("%f%c", &new_record.avg_mark, &t) != 2 || t != '\n')
        return AVG_MARK_INPUT_ERR;

    printf(ANSI_YELLOW "%s\n" ANSI_RESET,
           "Введите дату зачисления студента (через пробел в формате ДД ММ ГГГГ): ");

    if (scanf("%hu %hu %hu%c", &(new_record.day), &(new_record.month), &(new_record.year), &t) != 4 ||
        t != '\n' ||
        new_record.year < MIN_YEAR || new_record.year > MAX_YEAR ||
        new_record.month < MIN_MONTH || new_record.month > MAX_MONTH ||
        new_record.day < MIN_DAY || new_record.day > MAX_DAY)
        return DATE_INPUT_ERR;

    printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите тип жилья (0 - дом, 1 - общежитие): ");
    if (input_number(&temp_num, 0, 1) != TABLE_OK)
        return TYPE_INPUT_ERR;

    if (temp_num == 0)
        new_record.is_dormitory = false;
    else if (temp_num == 1)
        new_record.is_dormitory = true;
    else
        return TYPE_INPUT_ERR;

    if (new_record.is_dormitory)
    {
        printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите номер общежития:");
        if (scanf("%hu", &(new_record.address.dormitory.dormitory_num)) != 1)
            return DORMITORY_INPUT_ERR;

        printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите номер комнаты");
        if (scanf("%hu", &(new_record.address.dormitory.room_num)) != 1)
            return ROOM_INPUT_ERR;
    }
    else
    {
        printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите улицу:");
        if (scanf("%s", new_record.address.personal.street) != 1)
            return STREET_INPUT_ERR;

        printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите номер дома:");
        if (scanf("%hu", &(new_record.address.personal.house_num)) != 1)
            return HOUSE_INPUT_ERR;

        printf(ANSI_YELLOW "%s\n" ANSI_RESET, "Введите номер квартиры:");
        if (scanf("%hu", &(new_record.address.personal.flat_num)) != 1)
            return FLAT_INPUT_ERR;
    }

    table->main_table[table->size_of_table] = new_record;

    table->key_table[table->size_of_table].student_table_index = table->size_of_table;
    table->key_table[table->size_of_table].avg_mark = new_record.avg_mark;

    table->size_of_table++;

    return TABLE_OK;
}

int delete_record_by_avg_mark(aio_table_t *const table, const float mark)
{
    int deletions_count = 0;

    for (int i = table->size_of_table - 1; i >= 0; --i)
    {
        if (((table->main_table[i].avg_mark - 0.00001) > mark) ||
            ((table->main_table[i].avg_mark + 0.00001) < mark))
            continue;

        for (int j = i; j < table->size_of_table - 1; ++j)
        {
            table->main_table[j] = table->main_table[j + 1];
            table->key_table[j] = table->key_table[j + 1];
        }

        deletions_count++;
        table->size_of_table--;
    }

    return deletions_count;
}

int compare_avg_mark(const void *a, const void *b)
{
    if (((student_t *)a)->avg_mark > ((student_t *)b)->avg_mark)
        return 1;

    if (((student_t *)a)->avg_mark < ((student_t *)b)->avg_mark)
        return -1;

    return 0;
}

int compare_avg_mark_k(const void *a, const void *b)
{
    if (((student_key_t *)a)->avg_mark > ((student_key_t *)b)->avg_mark)
        return 1;

    if (((student_key_t *)a)->avg_mark < ((student_key_t *)b)->avg_mark)
        return -1;

    return 0;
}

void quick_sort_table(aio_table_t *const table, const bool non_key_sort)
{
    if (non_key_sort)
    {
        qsort(table->main_table, table->size_of_table, sizeof(student_t), compare_avg_mark);
        return;
    }

    qsort(table->key_table, table->size_of_table, sizeof(student_key_t), compare_avg_mark_k);
}

void bubble_sort_table(aio_table_t *const table, const bool non_key_sort)
{
    if (non_key_sort)
    {
        for (int i = 0; i < table->size_of_table; ++i)
            for (int j = 0; j < table->size_of_table - 1; ++j)
                if (table->main_table[j].avg_mark > table->main_table[j + 1].avg_mark)
                {
                    student_t tmp = table->main_table[j];
                    table->main_table[j] = table->main_table[j + 1];
                    table->main_table[j + 1] = tmp;
                }

        return;
    }

    for (int i = 0; i < table->size_of_table; ++i)
        for (int j = 0; j < table->size_of_table - 1; ++j)
        {
            if (table->key_table[j].avg_mark > table->key_table[j + 1].avg_mark)
            {
                student_key_t tmp = table->key_table[j];
                table->key_table[j] = table->key_table[j + 1];
                table->key_table[j + 1] = tmp;
            }
        }
}

uint64_t tick(void)
{
    uint32_t high, low;
    __asm__ __volatile__(
        "rdtsc\n"
        "movl %%edx, %0\n"
        "movl %%eax, %1\n"
        : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");

    uint64_t ticks = ((uint64_t)high << 32) | low;

    return ticks;
}
