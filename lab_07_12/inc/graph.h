#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "io.h"
#include "data_structures.h"

#define IS_CONN(__matrix__, __groups__) _Generic((__matrix__),  \
                                                 adjmat_t       \
                                                 : adj_is_conn, \
                                                   listmat_t    \
                                                 : list_is_conn)(__matrix__, __groups__)

#define GVEXPORT(__matrix__, __groups__) _Generic((__matrix__),   \
                                                  adjmat_t        \
                                                  : adj_gvexport, \
                                                    listmat_t     \
                                                  : list_gvexport)(__matrix__, __groups__)

int adj_is_conn(adjmat_t matrix, int **groups);

void adj_gvexport(adjmat_t matrix, int *groups);

//! ---------------------------------------- !//

int list_is_conn(listmat_t matrix, int **groups);

void list_gvexport(listmat_t matrix, int *groups);

#endif
