#include <stdlib.h>
#include <string.h>
#include "longdouble.h"

#include <stdio.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MANTISS_MAX_SIZE 30
#define MAX_EXPONENT_VALUE 99999

typedef struct
{
    char sign;
    unsigned char *digits;
    int digits_size;
    int exponent;
} longDoubleValue;

static int toString(LongDouble *this, char **str)
{
    longDoubleValue *val = ((longDoubleValue *)(this->value));

    int len = val->digits_size;

    if (*str != NULL)
        return 1;

    if (val->digits[0] == 0)
    {
        *str = (char *)malloc(2 * sizeof(char));

        if (*str == NULL)
            return MEMORY_ERROR;

        (*str)[0] = '0';
        (*str)[1] = '\0';
        return OK;
    }

    *str = (char *)malloc((len + 11) * sizeof(char));

    if (*str == NULL)
        return MEMORY_ERROR;

    (*str)[0] = val->sign;
    (*str)[1] = '0';
    (*str)[2] = '.';

    for (int i = 0; i < len; i++)
    {
        (*str)[i + 3] = '0' + val->digits[i];
    }

    (*str)[len + 3] = 'E';
    (*str)[len + 4] = val->exponent >= 0 ? '+' : '-';

    int exp = abs(val->exponent), size = 1;
    while (exp / 10 > 0)
    {
        exp /= 10;
        size *= 10;
    }

    exp = abs(val->exponent);
    int i = 5;
    for (; size; i++)
    {
        (*str)[len + i] = '0' + exp / size;
        exp %= size;
        size /= 10;
    }
    (*str)[len + i] = '\0';

    return OK;
}

int removeZeros(longDoubleValue *this)
{
    while (this->digits_size > 1 && this->digits[0] == 0)
    {
        for (int i = 0; i < this->digits_size - 1; i++)
            this->digits[i] = this->digits[i + 1];
        this->digits_size--;

        this->exponent--;
    }

    while (this->digits_size > 1 && this->digits[this->digits_size - 1] == 0)
        this->digits_size--;

    if (this->digits[0] == 0)
    {
        this->exponent = 1;
        this->sign = '+';
    }

    unsigned char *temp = (unsigned char *)realloc(this->digits, this->digits_size * sizeof(char));
    if (temp == NULL)
        return MEMORY_ERROR;

    this->digits = temp;

    return OK;
}

static int rounding(longDoubleValue *val)
{
    int len = val->digits_size;

    for (int i = len - 1; i > 0; i--)
        if (val->digits[i] > 5)
        {
            val->digits[i] = 0;
            val->digits[i - 1] += 1;
        }

    for (int i = len - 1; i > 0; i--)
    {
        val->digits[i - 1] += val->digits[i] / 10;
        val->digits[i] %= 10;
    }

    val->digits_size = MIN(MANTISS_MAX_SIZE, val->digits_size);

    unsigned char *temp = (unsigned char *)realloc(val->digits, val->digits_size * sizeof(char));
    if (temp == NULL)
        return MEMORY_ERROR;

    val->digits = temp;

    return OK;
}

static int multiplication(struct LongDouble *this, struct LongDouble *second, struct LongDouble *res)
{
    longDoubleValue *val_a = (longDoubleValue *)(this->value);
    longDoubleValue *val_b = (longDoubleValue *)(second->value);
    longDoubleValue *res_val = ((longDoubleValue *)(res->value));

    int len = val_a->digits_size + val_b->digits_size + 1;

    if ((val_a->sign == '+' && val_b->sign == '+') || (val_a->sign == '-' && val_b->sign == '-'))
        res_val->sign = '+';
    else
        res_val->sign = '-';

    res_val->exponent = val_a->exponent + val_b->exponent + 1;

    res_val->digits_size = len;

    unsigned char *temp = (unsigned char *)realloc(res_val->digits, res_val->digits_size * sizeof(char));
    if (temp == NULL)
        return MEMORY_ERROR;

    res_val->digits = temp;

    for (int i = 0; i < res_val->digits_size; i++)
        res_val->digits[i] = 0;

    for (int i = 0; i < val_a->digits_size; i++)
        for (int j = 0; j < val_b->digits_size; j++)
            res_val->digits[i + j + 2] += val_a->digits[i] * val_b->digits[j];

    for (int i = len - 1; i > 0; i--)
    {
        res_val->digits[i - 1] += res_val->digits[i] / 10;
        res_val->digits[i] %= 10;
    }

    rounding(res_val);

    removeZeros(res_val);

    if (abs(res_val->exponent) > MAX_EXPONENT_VALUE)
        return MULTIPLICATION_ERROR;

    if (res_val->digits_size <= MANTISS_MAX_SIZE)
        return OK;

    temp = (unsigned char *)realloc(res_val->digits, MANTISS_MAX_SIZE * sizeof(char));
    if (temp == NULL)
        return MEMORY_ERROR;

    res_val->digits = temp;

    return OK;
}

void deleteLongDouble(LongDouble **ptr)
{
    if (*ptr == NULL)
        return;

    free(((longDoubleValue *)((*ptr)->value))->digits);
    free((*ptr)->value);
    free(*ptr);
}

int newLongDouble(LongDouble **ptr, char *str, int strict)
{
    if (*ptr != NULL)
        return NON_FREE_PTR_ERROR;

    if (strlen(str) == 0)
        return EMPTY_INPUT;

    *ptr = (LongDouble *)malloc(sizeof(LongDouble));

    if (*ptr == NULL)
        return MEMORY_ERROR;

    (*ptr)->value = (longDoubleValue *)malloc(sizeof(longDoubleValue));

    if ((*ptr)->value == NULL)
        return MEMORY_ERROR;

    (*ptr)->toString = &toString;
    (*ptr)->multiply = &multiplication;

    longDoubleValue *val = ((longDoubleValue *)((*ptr)->value));

    val->digits = (unsigned char *)malloc(MANTISS_MAX_SIZE * sizeof(char));

    if (val->digits == NULL)
        return MEMORY_ERROR;

    val->sign = '+';
    val->digits_size = 0;
    val->exponent = 0;

    char dot_pos = -1;

    short i = 0;
    for (char c = str[0];
         (i < strlen(str)) &&
         (c != 'e') && (c != 'E') &&
         (c != '\n');
         c = str[++i])
    {

        if (('0' <= c) && (c <= '9'))
        {
            val->digits[val->digits_size++] = c - '0';
            continue;
        }

        if ((val->digits_size != 0 || i > 0) && (c == '+' || c == '-'))
        {
            deleteLongDouble(ptr);
            return WRONG_MANTISS_FORMAT;
        }

        if (c == '-')
        {
            val->sign = '-';
            continue;
        }

        if (c == '+')
            continue;

        if (c == '.' && dot_pos == -1 && strict == 0)
            dot_pos = i;
        else
        {
            deleteLongDouble(ptr);
            return WRONG_MANTISS_FORMAT;
        }
    }

    if (val->digits_size == 0)
    {
        deleteLongDouble(ptr);
        return EMPTY_MANTISS;
    }

    if (val->digits_size > MANTISS_MAX_SIZE)
    {
        deleteLongDouble(ptr);
        return MANTISS_OVERFLOW;
    }

    if (strict)
    {
        if (dot_pos != -1)
        {
            deleteLongDouble(ptr);
            return WRONG_MANTISS_FORMAT;
        }

        if (i != strlen(str))
        {
            deleteLongDouble(ptr);
            return WRONG_MANTISS_FORMAT;
        }

        val->exponent = i;

        removeZeros(val);

        return OK;
    }

    if (dot_pos == -1)
        dot_pos = val->digits_size;

    if ((str[i] != 'E') && (str[i] != 'e'))
    {
        deleteLongDouble(ptr);
        return WRONG_EXPONENT_FORMAT;
    }

    int sign = 1;
    if (str[i + 1] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i + 1] == '+')
        i++;

    short flag = 0;
    for (int j = 1;; j++)
    {

        if (!('0' <= str[i + j] && str[i + j] <= '9'))
        {

            if (str[i + j] == '\n' || str[i + j] == '\0')
                break;

            deleteLongDouble(ptr);
            return WRONG_EXPONENT_FORMAT;
        }

        if (j > 5)
        {
            deleteLongDouble(ptr);
            return WRONG_EXPONENT_FORMAT;
        }

        val->exponent = val->exponent * 10 + (str[i + j] - '0');
        flag = 1;
    }

    val->exponent *= sign;

    if (MAX_EXPONENT_VALUE < abs(val->exponent) || flag == 0)
    {
        deleteLongDouble(ptr);
        return WRONG_EXPONENT_FORMAT;
    }

    val->exponent += dot_pos;

    removeZeros(val);

    return OK;
}

int newEmptyLongDouble(LongDouble **ptr)
{
    if (*ptr != NULL)
        return NON_FREE_PTR_ERROR;

    *ptr = (LongDouble *)malloc(sizeof(LongDouble));

    if (*ptr == NULL)
        return MEMORY_ERROR;

    (*ptr)->value = (longDoubleValue *)malloc(sizeof(longDoubleValue));

    longDoubleValue *val = ((longDoubleValue *)((*ptr)->value));
    val->digits = (unsigned char *)malloc(MANTISS_MAX_SIZE * sizeof(char));

    if (val->digits == NULL)
        return MEMORY_ERROR;

    val->digits_size = 0;
    val->exponent = 0;
    val->sign = '+';

    (*ptr)->toString = &toString;
    (*ptr)->multiply = &multiplication;

    return OK;
}
