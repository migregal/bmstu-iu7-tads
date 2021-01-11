#ifndef LONG_DOUBLE_H
#define LONG_DOUBLE_H

#define OK 0
#define EMPTY_INPUT 1
#define WRONG_MANTISS_FORMAT 2
#define EMPTY_MANTISS 3
#define MANTISS_OVERFLOW 4
#define WRONG_EXPONENT_FORMAT 5
#define NON_FREE_PTR_ERROR 6
#define MULTIPLICATION_ERROR 7
#define CREATING_NEW_OBJ_ERROR 8

#define MEMORY_ERROR 100

typedef struct LongDouble
{
    void *value;
    int (*toString)(struct LongDouble *, char **);
    int (*multiply)(struct LongDouble *, struct LongDouble *, struct LongDouble *);
} LongDouble;

int newLongDouble(LongDouble **ptr, char *, int);

int newEmptyLongDouble(LongDouble **);

void deleteLongDouble(LongDouble **);

#endif
