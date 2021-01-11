#ifndef __IO_H__
#define __IO_H__

#include "data_types.h"

#define IO_OK 0
#define IO_IN_BETWEEN_ERROR 1
#define IO_INVALID_INPUT_ERROR 2
#define IO_FILE_NOT_EXIST_ERR 3
#define IO_EMPTY_FILE_ERR 4
#define IO_WRITE_ERR 5

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_RESET "\x1b[0m"

#define I ANSI_GREEN "|" ANSI_RESET
#define ___ ANSI_GREEN "-" ANSI_RESET

int input_string(char *string);

int input_number(int *const, const int, const int);

void welcome_print(void);

void menu_print(void);

int upload_table_from_bin(const char *const, aio_table_t *const);

int save_table_to_bin(const char *const, const aio_table_t *const);

int upload_table_from_csv(const char *const, aio_table_t *const);

int save_table_to_csv(const char *const, const aio_table_t *const);

void output_main_table(const aio_table_t *const);

void output_main_table_by_keys(const aio_table_t *const table);

void output_key_table(const aio_table_t *const);

#endif
