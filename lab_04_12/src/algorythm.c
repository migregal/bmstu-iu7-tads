#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>

#include "algorythm.h"

#include "algorythm_list.h"
#include "algorythm_arr.h"
#include "func_io.h"

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

void calculate_list(void)
{
    char *string = NULL;
    size_t len = 0;

    expression_format();
    if (-1 == getline(&string, &len, stdin))
    {
        free(string);
        return;
    }

    // uint64_t start = tick();

    if (convert_to_rpn_list(string, strlen(string), 1e-6))
    {
        free(string);
        CONVERTATION_ERR_MSG;
        return;
    }

    double result = 0.0;

    if (calculate_rpn_list(string, strlen(string), 1e-6, &result))
    {
        free(string);
        CALCULATION_ERR_MSG;
        return;
    }
    // uint64_t end = tick();

    // OUTPUT_TICKS_MSG("%llu", end - start);
    OUTPUT_ELEM_MSG("%10.6g", result);
}

void calculate_arr(void)
{
    char *string = NULL;
    size_t len = 0;

    expression_format();
    if (-1 == getline(&string, &len, stdin))
    {
        free(string);
        return;
    }

    // uint64_t start = tick();

    if (convert_to_rpn_arr(string, strlen(string), 1e-6))
    {
        free(string);
        CONVERTATION_ERR_MSG;
        return;
    }
    double result = 0.0;

    if (calculate_rpn_arr(string, strlen(string), 1e-6, &result))
    {
        free(string);
        CALCULATION_ERR_MSG;
        return;
    }

    // uint64_t end = tick();

    // OUTPUT_TICKS_MSG("%llu", end - start);
    OUTPUT_ELEM_MSG("%10.6g", result);
}
