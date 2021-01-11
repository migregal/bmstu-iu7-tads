#include <string.h>

#include "stack_common.h"
#include "stack_arr.h"

#define initial_size 1
#define size_coef 2

#define ELEM(__stack__, __i__) \
    ((char *)(__stack__->arr) + (__i__)*__stack__->elem_size)

struct a_stack_t
{
    void *arr;
    size_t len, cap;
    size_t elem_size;
};

int a_stack_init(a_stack_t **stackptr, const size_t cap, const size_t elem_size)
{
    *stackptr = malloc(sizeof(a_stack_t));
    (*stackptr)->elem_size = elem_size;
    (*stackptr)->len = 0;
    (*stackptr)->cap = cap;
    (*stackptr)->arr = calloc(initial_size, sizeof(elem_size));

    return STACK_OK;
}

int a_stack_is_empty(a_stack_t *stack)
{
    return stack->len == 0;
}

size_t a_stack_len(a_stack_t *stack)
{
    return stack->len;
}

void a_stack_free(a_stack_t **stack)
{
    free((*stack)->arr);
    free(*stack);
    (*stack)->cap = 0;
    (*stack)->len = 0;
    *stack = NULL;
}

int a_stack_push_front(a_stack_t *stack, const void *const value)
{
    if (stack == NULL)
        return NON_INIT_STACK_ERR;

    if (stack->len == stack->cap)
        return STACK_OVERFLOW_ERR;

    // void *temp = realloc(
    //     stack->arr,
    //     stack->elem_size * (stack->cap *= 2));

    // if (!temp)
    //     return MEM_ALLOC_ERR;

    // stack->arr = temp;

    memcpy(ELEM(stack, stack->len++), value, stack->elem_size);

    return STACK_OK;
}

int a_stack_top(a_stack_t *stack, void *const value, void **ptr)
{
    if (stack == NULL)
        return NON_INIT_STACK_ERR;

    if (a_stack_is_empty(stack))
        return MEM_ACCESS_ERR;

    memcpy(value, ELEM(stack, stack->len - 1), stack->elem_size);

    *ptr = ELEM(stack, stack->len - 1);

    return STACK_OK;
}

int a_stack_pop_front(a_stack_t *stack, void *const value)
{
    if (stack == NULL)
        return NON_INIT_STACK_ERR;

    if (a_stack_is_empty(stack))
        return MEM_ACCESS_ERR;

    memcpy(value, ELEM(stack, --stack->len), stack->elem_size);

    // if ((stack->len) < stack->cap / 2 && stack->cap > 1)
    // {
    //     void *temp = realloc(
    //         stack->arr,
    //         stack->elem_size * (stack->cap /= 2));

    //     if (!temp)
    //         return MEM_ALLOC_ERR;

    //     stack->arr = temp;
    // }

    return STACK_OK;
}

struct a_stack_iterator
{
    a_stack_t *stack;
    size_t cur;
};

int a_iterator_init(a_stack_t *stack, a_iterator **iter)
{
    if (!stack->len)
        return MEM_ACCESS_ERR;

    *iter = malloc(sizeof(a_iterator));

    if (!*iter)
        return MEM_ALLOC_ERR;

    (*iter)->stack = stack;
    (*iter)->cur = (*iter)->stack->len;

    return STACK_OK;
}

int a_next(a_iterator *iter, void *value, void **ptr)
{
    if (a_end(iter))
        return MEM_ACCESS_ERR;

    memcpy(
        value,
        (void *)ELEM(iter->stack, iter->cur - 1),
        sizeof(iter->stack->elem_size));

    *ptr = (double *)ELEM(iter->stack, iter->cur - 1);
    iter->cur--;

    return STACK_OK;
}

int a_end(a_iterator *iter)
{
    return !iter->cur;
}

void a_iterator_free(a_iterator **iter)
{
    (*iter)->cur = 0;
    (*iter)->stack = NULL;
    free(*iter);
}

#undef ELEM
