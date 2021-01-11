#ifndef __LIST_STACK_H__
#define __LIST_STACK_H__

#include <stdlib.h>

struct l_stack_t;

typedef struct l_stack_t l_stack_t;

int l_stack_init(l_stack_t **, const size_t, const size_t);

int l_stack_push_front(l_stack_t *, const void *const);

int l_stack_top(struct l_stack_t *, void *const, void **);

int l_stack_pop_front(struct l_stack_t *, void *const);

int l_stack_is_empty(struct l_stack_t *);

size_t l_stack_len(struct l_stack_t *);

void l_stack_free(l_stack_t **);

// Iterator

struct l_stack_iterator;

typedef struct l_stack_iterator l_iterator;

int l_iterator_init(l_stack_t *, l_iterator **);

int l_next(l_iterator *, void *, void **);

int l_end(l_iterator *);

void l_iterator_free(l_iterator **);

#endif
