#include <x86intrin.h>

#include "../inc/timer.h"

uint64_t tick(void)
{
    return __rdtsc();
}
