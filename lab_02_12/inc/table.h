#ifndef __TABLE_H__
#define __TABLE_H__

#include <stdint.h>

#include "data_types.h"

#define MIN_YEAR 1900
#define MAX_YEAR 2020
#define MIN_MONTH 1
#define MAX_MONTH 12
#define MIN_DAY 1
#define MAX_DAY 31

#define TABLE_OK 0
#define TABLE_OVERFLOW_ERR 1
#define SURNAME_INPUT_ERR 2
#define NAME_INPUT_ERR 3
#define GROUP_INPUT_ERR 4
#define SEX_INPUT_ERR 5
#define AVG_MARK_INPUT_ERR 6
#define DATE_INPUT_ERR 7
#define TYPE_INPUT_ERR 8
#define DORMITORY_INPUT_ERR 9
#define ROOM_INPUT_ERR 10
#define STREET_INPUT_ERR 11
#define HOUSE_INPUT_ERR 12
#define FLAT_INPUT_ERR 13
#define TABLE_INPUT_ERR 14

int add(aio_table_t *const);

int delete_record_by_avg_mark(aio_table_t *const, const float);

void bubble_sort_table(aio_table_t *const, const bool);

void quick_sort_table(aio_table_t *const, const bool);

uint64_t tick(void);

#endif
