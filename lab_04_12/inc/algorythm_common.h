#ifndef __ALGORYTH_COMMON_H__
#define __ALGORYTH_COMMON_H__

#define OK 0;

#define NUM_LEN "10"
#define ACCURACY "6"

#define delimiters " =\n"
#define operation_len 4

int is_operation(const char *token);

int operation_prior(const char *operation);

int process_operation(double a, double b, const char *operation, double *res);

#endif
