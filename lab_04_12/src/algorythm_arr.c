#include <stdio.h>
#include <string.h>

#include "algorythm_common.h"
#include "stack_arr.h"
#include "algorythm_arr.h"

int convert_to_rpn_arr(char *const string, size_t len, double eps)
{
    char *temp = malloc((len + 1) * sizeof(char));
    memcpy(temp, string, (len + 1) * sizeof(char));
    size_t strpos = 0;

    a_stack_t *operations;
    a_stack_init(&operations, strlen(string), operation_len * sizeof(char));

    const char *token = strtok((char *)temp, delimiters);

    char symbol[operation_len];
    while (token != NULL)
    {
        if (!strcmp(token, "("))
        {
            a_stack_push_front(operations, token);

            goto tokenization;
        }

        if (!strcmp(token, ")"))
        {
            while (!a_stack_pop_front(operations, symbol) && strcmp(symbol, "("))
            {
                for (size_t i = 0; i < strlen(symbol); ++i)
                    *(temp + strpos++) = *(symbol + i);
                *(temp + strpos++) = ' ';
            }

            if (strcmp(symbol, "("))
                return 1;

            goto tokenization;
        }

        if (is_operation(token))
        {

            void *ptr;
            while (!a_stack_top(operations, symbol, &ptr))
            {
                if (!strcmp(symbol, "(") || operation_prior(token) > operation_prior(symbol))
                    break;

                a_stack_pop_front(operations, symbol);

                for (size_t i = 0; i < strlen(symbol); ++i)
                    *(temp + strpos++) = *(symbol + i);
                *(temp + strpos++) = ' ';
            }

            a_stack_push_front(operations, token);

            goto tokenization;
        }

        double val = strtod(token, NULL);
        if (eps > val && -eps < val)
            return 2;

        for (size_t i = 0; i < strlen(token);)
            *(temp + strpos++) = *(token + i++);
        *(temp + strpos++) = ' ';

    tokenization:
        token = strtok(NULL, delimiters);
    }

    while (!a_stack_pop_front(operations, symbol))
    {
        if (!strcmp(symbol, "("))
            return 1;

        for (size_t i = 0; i < strlen(symbol); ++i)
            *(temp + strpos++) = *(symbol + i);
        *(temp + strpos++) = ' ';
    }
    *(temp + strpos) = '\0';

    memcpy(string, temp, strpos * sizeof(char));

    *(string + strpos) = '\0';

    a_stack_free(&operations);
    free(temp);

    return OK;
}

int calculate_rpn_arr(char *const string, size_t len, double eps, double *result)
{
    char *temp = malloc((len + 1) * sizeof(char));
    memcpy(temp, string, (len + 1) * sizeof(char));

    struct a_stack_t *values;
    a_stack_init(&values, strlen(string), sizeof(double));

    const char *token = strtok((char *)temp, delimiters);

    *result = 0.0;

    double a = 0.0, b = 0.0, res = 0.0;
    while (token != NULL)
    {
        if (is_operation(token))
        {
            if (a_stack_len(values) < 2)
            {
                a_stack_free(&values);
                free(temp);
                return 3;
            }

            a_stack_pop_front(values, &a);
            a_stack_pop_front(values, &b);

            printf("%-" NUM_LEN "." ACCURACY "g %s %-" NUM_LEN "." ACCURACY "g = ", b, token, a);

            process_operation(a, b, token, &res);

            printf("%-" NUM_LEN "." ACCURACY "g\n", res);
            a_stack_push_front(values, &res);

            goto tokenization;
        }

        double val = strtod(token, NULL);
        if (eps > val && -eps < val)
            return 2;

        a_stack_push_front(values, &val);

    tokenization:
        token = strtok(NULL, delimiters);
    }

    a_stack_pop_front(values, result);

    a_stack_free(&values);
    free(temp);

    return OK;
}
