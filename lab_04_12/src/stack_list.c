#include <string.h>

#include "stack_common.h"
#include "stack_list.h"

struct node_t
{
    void *value;
    struct node_t *next;
};

struct l_stack_t
{
    struct node_t *head;
    size_t len, cap;
    size_t elem_size;
};

int l_stack_init(struct l_stack_t **stackptr, const size_t cap, const size_t elem_size)
{
    *stackptr = malloc(sizeof(struct l_stack_t));
    (*stackptr)->elem_size = elem_size;
    (*stackptr)->len = 0;
    (*stackptr)->cap = cap;
    (*stackptr)->head = NULL;

    return STACK_OK;
}

int l_stack_is_empty(struct l_stack_t *stack)
{
    return stack->len == 0;
}

size_t l_stack_len(struct l_stack_t *stack)
{
    return stack->len;
}

void l_stack_free(struct l_stack_t **stack)
{
    while ((*stack)->head)
    {
        struct node_t *next = (*stack)->head->next;

        free((*stack)->head->value);
        free((*stack)->head);

        (*stack)->head = next;
    }

    free(*stack);
    *stack = NULL;
}

int l_stack_push_front(struct l_stack_t *stack, const void *const value)
{
    if (stack == NULL)
        return NON_INIT_STACK_ERR;

    if (stack->len == stack->cap)
        return STACK_OVERFLOW_ERR;

    struct node_t *node = malloc(sizeof(*node));

    if (!node)
        return MEM_ALLOC_ERR;

    if (!(node->value = malloc(stack->elem_size)))
    {
        free(node);
        return MEM_ALLOC_ERR;
    }

    memcpy(node->value, value, stack->elem_size);

    node->next = stack->head;

    stack->head = node;
    stack->len++;

    return STACK_OK;
}

int l_stack_top(struct l_stack_t *stack, void *const value, void **ptr)
{
    if (stack == NULL)
        return NON_INIT_STACK_ERR;

    if (l_stack_is_empty(stack))
        return MEM_ACCESS_ERR;

    memcpy(value, stack->head->value, stack->elem_size);
    *ptr = stack->head;

    return STACK_OK;
}

int l_stack_pop_front(struct l_stack_t *stack, void *const value)
{
    if (stack == NULL)
        return NON_INIT_STACK_ERR;

    if (l_stack_is_empty(stack))
        return MEM_ACCESS_ERR;

    memcpy(value, stack->head->value, stack->elem_size);

    struct node_t *node = stack->head;

    stack->head = stack->head->next;
    stack->len--;

    free(node->value);
    free(node);

    return STACK_OK;
}

struct l_stack_iterator
{
    l_stack_t *stack;
    struct node_t *cur;
};

int l_iterator_init(l_stack_t *stack, l_iterator **iter)
{
    *iter = malloc(sizeof(l_iterator));

    if (!*iter)
        return MEM_ALLOC_ERR;

    (*iter)->stack = stack;
    (*iter)->cur = (*iter)->stack->head;

    return STACK_OK;
}

int l_next(l_iterator *iter, void *value, void **ptr)
{
    if (l_end(iter))
        return MEM_ACCESS_ERR;

    memcpy(
        value,
        iter->cur->value,
        sizeof(iter->stack->elem_size));
    *ptr = iter->cur + 1;

    iter->cur = iter->cur->next;

    return STACK_OK;
}

int l_end(l_iterator *iter)
{
    return NULL == iter->cur;
}

void l_iterator_free(l_iterator **iter)
{
    (*iter)->cur = NULL;
    (*iter)->stack = NULL;
    free(*iter);
}
