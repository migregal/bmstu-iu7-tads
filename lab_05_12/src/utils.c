#include <x86intrin.h>

#include "utils.h"

double get_rand_double(const int llim, const int rlim)
{
    double rnd = (double)rand() / (double)RAND_MAX;

    return (double)(rlim - llim) * rnd + llim;
}

uint64_t tick(void)
{
    return __rdtsc();
}
