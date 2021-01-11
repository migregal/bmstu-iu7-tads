#include <string.h>
#include <math.h>

#include "algorythm_common.h"

int is_operation(const char *operation)
{
    return !strcmp(operation, "*") ||
           !strcmp(operation, "/") ||
           !strcmp(operation, "mod") ||
           !strcmp(operation, "+") ||
           !strcmp(operation, "-") ||
           !strcmp(operation, "^");
}

int operation_prior(const char *operation)
{
    if (!strcmp(operation, "^"))
        return 3;

    if (!strcmp(operation, "*") ||
        !strcmp(operation, "/") ||
        !strcmp(operation, "mod"))
        return 2;

    if (!strcmp(operation, "+") || !strcmp(operation, "-"))
        return 1;

    return -1;
}

int process_operation(double a, double b, const char *operation, double *res)
{
    if (!strcmp(operation, "+"))
    {
        *res = b + a;
        return OK;
    }

    if (!strcmp(operation, "-"))
    {
        *res = b - a;
        return OK;
    }

    if (!strcmp(operation, "*"))
    {
        *res = b * a;
        return OK;
    }

    if (!strcmp(operation, "/"))
    {
        *res = b / a;
        return OK;
    }

    if (!strcmp(operation, "mod"))
    {
        *res = fmod(b, a);
        if (*res < 0 && a > 0)
            *res += a;

        return OK;
    }

    if (!strcmp(operation, "^"))
    {
        *res = pow(b, a);
        return OK;
    }

    return 1;
}
