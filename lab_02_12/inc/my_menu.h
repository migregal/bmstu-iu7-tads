#ifndef __MY_MENU_H__
#define __MY_MENU_H__

#include <stdbool.h>
#include "data_types.h"

void print_header(void);

void print_menu(void);

bool input_cmd(char *);

void load_file(aio_table_t *const);

void save_file(const aio_table_t *const);

void load_file_csv(aio_table_t *const);

void save_file_csv(const aio_table_t *const);

void add_record(aio_table_t *const);

void list(const aio_table_t *const);

void delete_record(aio_table_t *const);

void k_sort(aio_table_t *const table);

void k_sort_q(aio_table_t *const table);

void sort(aio_table_t *const table);

void sort_q(aio_table_t *const table);

void keys(const aio_table_t *const);

void print(const aio_table_t *const);

void print_k(const aio_table_t *const table);

void cmp(aio_table_t *const table);

void unknown_cmd(void);

bool process_menu(aio_table_t *);

#endif
