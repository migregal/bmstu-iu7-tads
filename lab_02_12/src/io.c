#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "io.h"
#include "data_types.h"

#define SURNAME_LEN 27
#define NAME_LEN 13
#define GROUP_LEN 10
#define SEX_LEN 5
#define AVG_MARK_LEN 10
#define DATE_LEN 12
#define STREET_LEN 21
#define HOUSE_LEN 7
#define FLAT_LEN 7
#define DORMITORY_LEN 11
#define ROOM_LEN 9

#define KEY_TABLE_CELL_LEN 5

#define MAX_INPUT_STRING_SIZE 512

void welcome_print(void)
{
    printf("%s",
           ANSI_MAGENTA
           "Данная программа позволяет:\n"
           "1. Сравнить время, затраченное на сортировку таблицы\n"
           "   сортировками с асимптотическими сложностями O(n^2) и O(n*log(n));\n"
           "2. Добавить в конец таблицы запись, введенную пользователем;\n"
           "3. Удалить из таблицы запись по значению указанного поля;\n"
           "4. Сохранить таблицу в csv файлл;\n"
           "5. Загрузить таблицу из csv файл;\n"
           "6. Просмотреть отсортированную таблицу ключей при несортированной исходной таблице;\n"
           "7. Вывести упорядоченную исходную таблицу;\n"
           "8. Вывести исходную таблицу в упорядоченном виде, используя упорядоченную таблицу ключей;\n"
           "9. Вывести результаты использования различных алгоритмов сортировки.\n\n" ANSI_RESET);
}

void menu_print(void)
{
    printf("\n\n%s",
           ANSI_YELLOW
           "Выберите действие из списка, введя соответствующую команду:\n\n" ANSI_RESET);
    printf("%s",
           ANSI_GREEN
           "menu" ANSI_RESET
           "     -  Показать основное меню\n\n" ANSI_GREEN
           "load" ANSI_RESET
           " -  Загрузить таблицу из файла.\n" ANSI_GREEN
           "save" ANSI_RESET
           " -  Сохранить таблицу в файл.\n\n" ANSI_GREEN
           "add" ANSI_RESET
           "      -  Добавить запись в таблицу.\n" ANSI_GREEN
           "list" ANSI_RESET
           "     -  Вывести список студентов указанного года поступления, живущих в общежитии.\n" ANSI_GREEN
           "delete" ANSI_RESET
           "   -  Удалить запись из таблицы по фамилии и имени студента.\n\n" ANSI_GREEN
           "k_sort" ANSI_RESET
           "   -  Отсортировать таблицу ключей сортировкой O(n^2).\n" ANSI_GREEN
           "k_sort_q" ANSI_RESET
           " -  Отсортировать таблицу ключей сортировкой O(n*log(n)).\n" ANSI_GREEN
           "sort" ANSI_RESET
           "     -  Отсортировать исходную таблицу по ср. баллу (O(n^2)).\n" ANSI_GREEN
           "sort_q" ANSI_RESET
           "   -  Отсортировать исходную таблицу по ср. баллу (O(n*log(n))).\n\n" ANSI_GREEN
           "keys" ANSI_RESET
           "     -  Вывести таблицу ключей.\n" ANSI_GREEN
           "print" ANSI_RESET
           "    -  Вывести на экран таблицу.\n" ANSI_GREEN
           "print_k" ANSI_RESET
           "  -  Вывести исходную таблицу, используя упорядоченную таблицу ключей.\n\n" ANSI_GREEN
           "cmp" ANSI_RESET
           "      -  Вывести сравнение времени сортировки таблицы сортировками со\n"
           "            сложностями O(n^2) и O(n*log(n)) и аналогичных сортировок таблицей ключей.\n\n" ANSI_GREEN
           "clear" ANSI_RESET
           "    -  Очистить экран.\n" ANSI_GREEN
           "exit" ANSI_RESET
           "     -  Выход из программы.\n\n");
}

#define PRINT_REPEAT(s, n)          \
    do                              \
    {                               \
        for (int i = 0; i < n; i++) \
            printf("%s", s);        \
    } while (0)

void clean_input_stream()
{
    for (int c = getchar(); c != '\n' && c != EOF; c = getchar());
}

int input_string(char *string)
{
    if (scanf("%s", string) != 1)
    {
        clean_input_stream();
        return IO_INVALID_INPUT_ERROR;
    }

    clean_input_stream();
    return IO_OK;
}

int input_number(int *const number, const int left_border, const int right_border)
{
    if (scanf("%d", number) != 1)
    {
        clean_input_stream();
        return IO_INVALID_INPUT_ERROR;
    }

    if (*number < left_border || *number > right_border)
    {
        clean_input_stream();
        return IO_IN_BETWEEN_ERROR;
    }

    return IO_OK;
}

int get_csv_field(char *const line, int field_num, char *result)
{
    int pos = 0;

    for (; line[pos] != '\n' && field_num; pos++)
        if (line[pos] == ';' && field_num--);

    if (field_num)
        return IO_INVALID_INPUT_ERROR;

    int i = 0;
    for (; line[pos] != '\n' && line[pos] != ';'; pos++, i++)
        result[i] = line[pos];

    result[i] = '\0';

    return IO_OK;
}

int upload_table_from_csv(const char *const filename, aio_table_t *const table)
{

    FILE *fin = fopen(filename, "r");

    if (!fin)
        return IO_FILE_NOT_EXIST_ERR;

    if (fgetc(fin) == EOF)
        return IO_EMPTY_FILE_ERR;

    fseek(fin, 0, SEEK_SET);

    table->size_of_table = 0;

    char line[MAX_INPUT_STRING_SIZE], tmp[MAX_INPUT_STRING_SIZE];

    for (int i = 0; i < TABLE_SIZE &&
                    fscanf(fin, "%[^\n]s", line) &&
                    fgetc(fin) && !feof(fin);
         ++i, table->size_of_table++)
    {
        if (get_csv_field(line, 0, tmp))
            return IO_INVALID_INPUT_ERROR;

        strcpy(table->main_table[i].surname, tmp);

        if (get_csv_field(line, 1, tmp))
            return IO_INVALID_INPUT_ERROR;

        strcpy(table->main_table[i].name, tmp);

        if (get_csv_field(line, 2, tmp))
            return IO_INVALID_INPUT_ERROR;

        strcpy(table->main_table[i].group, tmp);

        if (get_csv_field(line, 3, tmp))
            return IO_INVALID_INPUT_ERROR;

        table->main_table[i].sex = strcmp(tmp, "male") == 0 ? true : false;

        if (get_csv_field(line, 4, tmp))
            return IO_INVALID_INPUT_ERROR;

        table->main_table[i].age = (unsigned short)strtol(tmp, NULL, 10);

        if (get_csv_field(line, 5, tmp))
            return IO_INVALID_INPUT_ERROR;

        table->main_table[i].avg_mark = strtof(tmp, NULL);
        table->key_table[table->size_of_table].student_table_index = table->size_of_table;
        table->key_table[table->size_of_table].avg_mark = table->main_table[i].avg_mark;

        if (get_csv_field(line, 6, tmp))
            return IO_INVALID_INPUT_ERROR;

        table->main_table[i].day = (unsigned short)strtol(tmp, NULL, 10);

        if (get_csv_field(line, 7, tmp))
            return IO_INVALID_INPUT_ERROR;

        table->main_table[i].month = (unsigned short)strtol(tmp, NULL, 10);

        if (get_csv_field(line, 8, tmp))
            return IO_INVALID_INPUT_ERROR;

        table->main_table[i].year = (unsigned short)strtol(tmp, NULL, 10);

        if (get_csv_field(line, 9, tmp))
            return IO_INVALID_INPUT_ERROR;

        table->main_table[i].is_dormitory = strcmp(tmp, "yes") == 0 ? true : false;

        if (table->main_table[i].is_dormitory)
        {
            if (get_csv_field(line, 10, tmp))
                return IO_INVALID_INPUT_ERROR;

            table->main_table[i].address.dormitory.dormitory_num =
                (unsigned short)strtol(tmp, NULL, 10);

            if (get_csv_field(line, 11, tmp))
                return IO_INVALID_INPUT_ERROR;

            table->main_table[i].address.dormitory.room_num =
                (unsigned short)strtol(tmp, NULL, 10);

            continue;
        }

        if (get_csv_field(line, 10, tmp))
            return IO_INVALID_INPUT_ERROR;

        strcpy(table->main_table[i].address.personal.street, tmp);

        if (get_csv_field(line, 11, tmp))
            return IO_INVALID_INPUT_ERROR;

        table->main_table[i].address.personal.house_num =
            (unsigned short)strtol(tmp, NULL, 10);

        if (get_csv_field(line, 12, tmp))
            return IO_INVALID_INPUT_ERROR;

        table->main_table[i].address.personal.flat_num =
            (unsigned short)strtol(tmp, NULL, 10);
    }

    fclose(fin);

    return IO_OK;
}

int save_table_to_csv(const char *const filename, const aio_table_t *const table)
{
    FILE *fout = fopen(filename, "w");

    if (!fout)
        return IO_FILE_NOT_EXIST_ERR;

    for (int i = 0; i < table->size_of_table; i++)
    {
        if (fprintf(fout, "%s;%s;%s;%s;%hu;%.2f;%hu;%hi;%hu;%s;",
                    table->main_table[i].surname,
                    table->main_table[i].name,
                    table->main_table[i].group,
                    table->main_table[i].sex ? "male" : "female",
                    table->main_table[i].age,
                    table->main_table[i].avg_mark,
                    table->main_table[i].day,
                    table->main_table[i].month,
                    table->main_table[i].year,
                    table->main_table[i].is_dormitory ? "yes" : "no") < 0)
        {
            fclose(fout);
            return IO_WRITE_ERR;
        }

        if (table->main_table[i].is_dormitory)
        {
            if (fprintf(fout, ";;;%hu;%hu;\n",
                        table->main_table[i].address.dormitory.dormitory_num,
                        table->main_table[i].address.dormitory.dormitory_num) < 0)
            {
                fclose(fout);
                return IO_WRITE_ERR;
            }

            continue;
        }

        if (fprintf(fout, "%s;%hu;%hu;;;\n",
                    table->main_table[i].address.personal.street,
                    table->main_table[i].address.personal.house_num,
                    table->main_table[i].address.personal.flat_num) < 0)
        {
            fclose(fout);
            return IO_WRITE_ERR;
        }
    }

    fclose(fout);

    return IO_OK;
}

void print_table_header(void)
{
    printf(ANSI_GREEN "\n\n|" ANSI_RESET);

    PRINT_REPEAT(" ", (SURNAME_LEN - 7) / 2);
    printf("Фамилия");
    PRINT_REPEAT(" ", (SURNAME_LEN - 7) / 2);

    printf(I);
    PRINT_REPEAT(" ", (NAME_LEN - 3) / 2);
    printf("Имя");
    PRINT_REPEAT(" ", (NAME_LEN - 3) / 2);

    printf(I);
    PRINT_REPEAT(" ", (GROUP_LEN - 6) / 2);
    printf("Группа");
    PRINT_REPEAT(" ", (GROUP_LEN - 6) / 2);

    printf(I);
    PRINT_REPEAT(" ", (SEX_LEN - 3) / 2);
    printf("Пол");
    PRINT_REPEAT(" ", (SEX_LEN - 3) / 2);

    printf(I);
    PRINT_REPEAT(" ", (AVG_MARK_LEN - 8) / 2);
    printf("Ср. балл");
    PRINT_REPEAT(" ", (AVG_MARK_LEN - 8) / 2);

    printf(I);
    PRINT_REPEAT(" ", (DATE_LEN - 4) / 2);
    printf("Дата");
    PRINT_REPEAT(" ", (DATE_LEN - 4) / 2);

    printf(I);
    PRINT_REPEAT(" ", (STREET_LEN - 5) / 2);
    printf("Улица");
    PRINT_REPEAT(" ", (STREET_LEN - 5) / 2);

    printf(I);
    PRINT_REPEAT(" ", (HOUSE_LEN - 3) / 2);
    printf("Дом");
    PRINT_REPEAT(" ", (HOUSE_LEN - 3) / 2);

    printf(I);
    PRINT_REPEAT(" ", (FLAT_LEN - 3) / 2);
    printf("Кв.");
    PRINT_REPEAT(" ", (FLAT_LEN - 3) / 2);

    printf(I);
    PRINT_REPEAT(" ", (DORMITORY_LEN - 9) / 2);
    printf("Общежитие");
    PRINT_REPEAT(" ", (DORMITORY_LEN - 9) / 2);

    printf(I);
    PRINT_REPEAT(" ", (ROOM_LEN - 7) / 2);
    printf("Комната");
    PRINT_REPEAT(" ", (ROOM_LEN - 7) / 2);

    printf(ANSI_GREEN "|\n|" ANSI_RESET);

    PRINT_REPEAT(___, SURNAME_LEN);

    printf(I);
    PRINT_REPEAT(___, NAME_LEN);

    printf(I);
    PRINT_REPEAT(___, GROUP_LEN);

    printf(I);
    PRINT_REPEAT(___, SEX_LEN);

    printf(I);
    PRINT_REPEAT(___, AVG_MARK_LEN);

    printf(I);
    PRINT_REPEAT(___, DATE_LEN);

    printf(I);
    PRINT_REPEAT(___, STREET_LEN);

    printf(I);
    PRINT_REPEAT(___, HOUSE_LEN);

    printf(I);
    PRINT_REPEAT(___, FLAT_LEN);

    printf(I);
    PRINT_REPEAT(___, DORMITORY_LEN);

    printf(I);
    PRINT_REPEAT(___, ROOM_LEN);

    printf(ANSI_GREEN "|\n" ANSI_RESET);
}

void print_main_student_info(const student_t *student)
{
    printf(I "%*s", SURNAME_LEN, student->surname);
    printf(I "%*s", NAME_LEN, student->name);
    printf(I "%*s", GROUP_LEN, student->group);

    if (student->sex)
        printf(I " муж ");
    else
        printf(I " жен ");

    printf(I "%*.2f", AVG_MARK_LEN, student->avg_mark);
    printf(I " %2hu.%02hu.%-4hu ", student->day, student->month, student->year);
}

void print_student_dormitory_info(const student_t *student)
{
    printf(I);
    PRINT_REPEAT(" ", (STREET_LEN - 1) / 2);
    printf("-");
    PRINT_REPEAT(" ", (STREET_LEN - 1) / 2);

    printf(I);
    PRINT_REPEAT(" ", (HOUSE_LEN - 1) / 2);
    printf("-");
    PRINT_REPEAT(" ", (HOUSE_LEN - 1) / 2);

    printf(I);
    PRINT_REPEAT(" ", (FLAT_LEN - 1) / 2);
    printf("-");
    PRINT_REPEAT(" ", (FLAT_LEN - 1) / 2);

    printf(I "%*hu", DORMITORY_LEN, student->address.dormitory.dormitory_num);

    printf(I "%*hu", ROOM_LEN, student->address.dormitory.room_num);

    printf(I "\n");
}

void print_student_flat_info(const student_t *student)
{
    printf(I "%*s", STREET_LEN, student->address.personal.street);

    printf(I "%*hu", HOUSE_LEN, student->address.personal.house_num);

    printf(I "%*hu", FLAT_LEN, student->address.personal.flat_num);

    printf(I);
    PRINT_REPEAT(" ", (DORMITORY_LEN - 1) / 2);
    printf("-");
    PRINT_REPEAT(" ", (DORMITORY_LEN - 1) / 2);

    printf(I);
    PRINT_REPEAT(" ", (ROOM_LEN - 1) / 2);
    printf("-");
    PRINT_REPEAT(" ", (ROOM_LEN - 1) / 2);

    printf(I "\n");
}

void print_table_footer(void)
{
    printf(I);
    PRINT_REPEAT(___, SURNAME_LEN);

    printf(I);
    PRINT_REPEAT(___, NAME_LEN);

    printf(I);
    PRINT_REPEAT(___, GROUP_LEN);

    printf(I);
    PRINT_REPEAT(___, SEX_LEN);

    printf(I);
    PRINT_REPEAT(___, AVG_MARK_LEN);

    printf(I);
    PRINT_REPEAT(___, DATE_LEN);

    printf(I);
    PRINT_REPEAT(___, STREET_LEN);

    printf(I);
    PRINT_REPEAT(___, HOUSE_LEN);

    printf(I);
    PRINT_REPEAT(___, FLAT_LEN);

    printf(I);
    PRINT_REPEAT(___, DORMITORY_LEN);

    printf(I);
    PRINT_REPEAT(___, ROOM_LEN);

    printf(ANSI_GREEN "|\n\n" ANSI_RESET);
}

void output_main_table(const aio_table_t *const table)
{
    print_table_header();

    for (int i = 0; i < table->size_of_table; ++i)
    {
        const student_t *student = table->main_table + i;

        print_main_student_info(student);

        if (student->is_dormitory)
            print_student_dormitory_info(student);
        else
            print_student_flat_info(student);
    }

    print_table_footer();
}

void output_main_table_by_keys(const aio_table_t *const table)
{
    print_table_header();

    for (int i = 0; i < table->size_of_table; ++i)
    {
        const student_t *student = table->main_table + table->key_table[i].student_table_index;

        print_main_student_info(student);

        if (student->is_dormitory)
            print_student_dormitory_info(student);
        else
            print_student_flat_info(student);
    }

    print_table_footer();
}

void output_key_table(const aio_table_t *const table)
{
    printf(ANSI_GREEN "\n\n|" ANSI_RESET);

    PRINT_REPEAT(" ", KEY_TABLE_CELL_LEN);

    printf(I);
    PRINT_REPEAT(" ", (AVG_MARK_LEN - 8) / 2);
    printf("Ср. балл");
    PRINT_REPEAT(" ", (AVG_MARK_LEN - 8) / 2);

    printf(ANSI_GREEN "|\n|" ANSI_RESET);
    PRINT_REPEAT(___, KEY_TABLE_CELL_LEN);

    printf(I);
    PRINT_REPEAT(___, AVG_MARK_LEN);
    printf(ANSI_GREEN "|\n" ANSI_RESET);

    for (int i = 0; i < table->size_of_table; ++i)
    {
        printf(I "%*d", KEY_TABLE_CELL_LEN, table->key_table[i].student_table_index);

        printf(I "%*.2f", AVG_MARK_LEN, table->key_table[i].avg_mark);

        printf(ANSI_GREEN "|\n" ANSI_RESET);
    }

    printf(I);
    PRINT_REPEAT(___, KEY_TABLE_CELL_LEN);

    printf(I);
    PRINT_REPEAT(___, AVG_MARK_LEN);
    printf(ANSI_GREEN "|\n" ANSI_RESET);
}
