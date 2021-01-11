#include <x86intrin.h>

#include "utils.h"

uint64_t tick(void)
{
    return __rdtsc();
}
