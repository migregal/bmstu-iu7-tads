#ifndef __ARR_STACK_H__
#define __ARR_STACK_H__

#include <stdlib.h>

#include "stack_common.h"

typedef struct a_stack_t a_stack_t;

int a_stack_init(a_stack_t **, const size_t, const size_t);

int a_stack_push_front(a_stack_t *, const void *const);

int a_stack_top(a_stack_t *, void *const, void **);

int a_stack_pop_front(a_stack_t *, void *const);

int a_stack_is_empty(a_stack_t *);

size_t a_stack_len(a_stack_t *);

void a_stack_free(a_stack_t **);

// Iterator

typedef struct a_stack_iterator a_iterator;

int a_iterator_init(a_stack_t *, a_iterator **);

int a_next(a_iterator *, void *, void **);

int a_end(a_iterator *);

void a_iterator_free(a_iterator **);

#endif
