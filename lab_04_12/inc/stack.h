#ifndef __STACK_H__
#define __STACK_H__

#include "stack_common.h"
#include "stack_list.h"
#include "stack_arr.h"

#define INIT_STACK(__stack__, __capacity__, __size__) _Generic((__stack__),    \
                                                               a_stack_t * *   \
                                                               : a_stack_init, \
                                                                 l_stack_t * * \
                                                               : l_stack_init)(__stack__, __capacity__, __size__)

#define TOP(__stack__, __val__, __ptr__) _Generic((__stack__),   \
                                                  a_stack_t *    \
                                                  : a_stack_top, \
                                                    l_stack_t *  \
                                                  : l_stack_top)(__stack__, __val__, __ptr__)

#define PUSH(__stack__, __val__) _Generic((__stack__),          \
                                          a_stack_t *           \
                                          : a_stack_push_front, \
                                            l_stack_t *         \
                                          : l_stack_push_front)(__stack__, __val__)

#define POP(__stack__, __res__) _Generic((__stack__),         \
                                         a_stack_t *          \
                                         : a_stack_pop_front, \
                                           l_stack_t *        \
                                         : l_stack_pop_front)(__stack__, __res__)

#define FREE(__stack__) _Generic((__stack__),    \
                                 a_stack_t * *   \
                                 : a_stack_free, \
                                   l_stack_t * * \
                                 : l_stack_free)(__stack__)

#define ITER_INIT(__stack__, __iter__) _Generic((__stack__),       \
                                                a_stack_t *        \
                                                : a_iterator_init, \
                                                  l_stack_t *      \
                                                : l_iterator_init)(__stack__, __iter__)

#define ITER_NEXT(__iter__, __res__, __ptr__) _Generic((__iter__),    \
                                                       a_iterator *   \
                                                       : a_next,      \
                                                         l_iterator * \
                                                       : l_next)(__iter__, __res__, __ptr__)

#define ITER_FREE(__iter__) _Generic((__iter__),        \
                                     a_iterator * *     \
                                     : a_iterator_free, \
                                       l_iterator * *   \
                                     : l_iterator_free)(__iter__)

#endif

#define PRINT_STACK(__stack__, __iter_t__, __elem_t__, __elem_fmt__) \
  do                                                                 \
  {                                                                  \
    __iter_t__ *__iter__;                                            \
    if (ITER_INIT(__stack__, &__iter__))                             \
    {                                                                \
      EMPTY_STACK_MSG;                                               \
      break;                                                         \
    }                                                                \
                                                                     \
    __elem_t__ __temp__;                                             \
    void *__ptr__ = NULL;                                            \
    size_t __i__ = 0;                                                \
    while (!ITER_NEXT(__iter__, &__temp__, &__ptr__) && (++__i__))   \
      printf("%zu : "__elem_fmt__                                    \
             " [%p]"                                                 \
             "\n",                                                   \
             __i__,                                                  \
             __temp__, __ptr__);                                     \
                                                                     \
    ITER_FREE(&__iter__);                                            \
    if (!__i__)                                                      \
      EMPTY_STACK_MSG;                                               \
  } while (0)

#define READ_STACK(__stack__, __elem_t__, __elem_fmt__)          \
  do                                                             \
  {                                                              \
    INPUT_STACK_CAP_MSG;                                         \
    long long __cap__ = 0;                                       \
    if (1 != scanf("%lld", &__cap__) || __cap__ < 0)             \
    {                                                            \
      CLEAN_INPUT_THREAD;                                        \
      VAL_ERR_MSG;                                               \
      break;                                                     \
    }                                                            \
                                                                 \
    INPUT_STACK_SIZE_MSG;                                        \
    long long __size__ = 0;                                      \
    if (1 != scanf("%lld", &__size__) || __size__ < 0)           \
    {                                                            \
      CLEAN_INPUT_THREAD;                                        \
      VAL_ERR_MSG;                                               \
      break;                                                     \
    }                                                            \
                                                                 \
    if (__cap__ < __size__)                                      \
    {                                                            \
      STACK_PARAMS_ERR_MSG;                                      \
      break;                                                     \
    }                                                            \
                                                                 \
    if (__stack__ != NULL)                                       \
      FREE(&__stack__);                                          \
    INIT_STACK(&__stack__, (size_t)__cap__, sizeof(__elem_t__)); \
                                                                 \
    if (!__size__)                                               \
    {                                                            \
      INPUT_STACK_SUCCESS_MSG;                                   \
      break;                                                     \
    }                                                            \
                                                                 \
    INPUT_STACK_ELEMS_MSG;                                       \
                                                                 \
    __elem_t__ __temp__ = 0.0;                                   \
    size_t __i__ = 0;                                            \
    for (; __i__ < (size_t)__size__; ++__i__)                    \
    {                                                            \
      if (1 != scanf(__elem_fmt__, &__temp__))                   \
      {                                                          \
        CLEAN_INPUT_THREAD;                                      \
        VAL_ERR_MSG;                                             \
        FREE(&__stack__);                                        \
        break;                                                   \
      }                                                          \
                                                                 \
      switch (PUSH(__stack__, &__temp__))                        \
      {                                                          \
      case STACK_OK:                                             \
        continue;                                                \
      default:                                                   \
        INTERNAL_ERR_MSG;                                        \
        FREE(&__stack__);                                        \
        __i__ = (size_t)__size__ + 1;                            \
      }                                                          \
    }                                                            \
    if (__i__ == (size_t)__size__)                               \
      INPUT_STACK_SUCCESS_MSG;                                   \
  } while (0)
