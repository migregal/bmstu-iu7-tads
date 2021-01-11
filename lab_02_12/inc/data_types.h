#ifndef _DATA_TYPES_H_
#define _DATA_TYPES_H_

#include <stdbool.h>

#define TABLE_SIZE 10000
#define MAX_STRING_FIELD_SIZE 64

typedef union address_t
{
    struct
    {
        char street[MAX_STRING_FIELD_SIZE];
        unsigned short house_num;
        unsigned short flat_num;
    } personal;

    struct dormitory
    {
        unsigned short dormitory_num;
        unsigned short room_num;
    } dormitory;
} address_t;

typedef struct student_t
{
    char surname[MAX_STRING_FIELD_SIZE];
    char name[MAX_STRING_FIELD_SIZE];
    char group[MAX_STRING_FIELD_SIZE];
    bool sex;
    unsigned short age;
    float avg_mark;
    short unsigned day, month, year;

    bool is_dormitory;
    address_t address;
} student_t;

typedef struct student_key_t
{
    int student_table_index;
    float avg_mark;
} student_key_t;

typedef struct
{
    student_t main_table[TABLE_SIZE];
    student_key_t key_table[TABLE_SIZE];
    int size_of_table;
} aio_table_t;

#endif
