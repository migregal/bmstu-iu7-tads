#ifndef __DATA_STRUCTURES_H__
#define __DATA_STRUCTURES_H__

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "io.h"

#define FREE(__matrix__) _Generic((__matrix__), \
                                  adjmat_t *    \
                                  : amfree,     \
                                    listmat_t * \
                                  : lmfree)(__matrix__)

#define FILL(__stream__, __matrix__) _Generic((__matrix__), \
                                              adjmat_t *    \
                                              : amfill,     \
                                                listmat_t * \
                                              : lmfill)(__stream__, __matrix__)
typedef int type_t;

typedef struct
{
    int size;
    type_t **matrix;
} adjmat_t;

adjmat_t *amcreate(const int size);

void amfree(adjmat_t *matrix);

int amfill(FILE *const stream, adjmat_t *const matrix);

//! ----------------------------------------- !//

typedef struct
{
  int size;
  node_t **matrix;
} listmat_t;

int cmp(const void *a, const void *b);

listmat_t *lmcreate(const int size);

void lmfree(listmat_t *matrix);

int lmfill(FILE *const stream, listmat_t *const matrix);

#endif
