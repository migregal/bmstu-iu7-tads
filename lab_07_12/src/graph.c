#include "graph.h"

#define COLOR_COUNT 19
static char *a[COLOR_COUNT] = {
    "darkorange",
    "aliceblue",
    "aquamarine",
    "beige",
    "brown",
    "cadetblue1",
    "chartreuse",
    "chocolate",
    "cyan",
    "darkorchid",
    "deeppink",
    "gold",
    "indigo",
    "khaki",
    "lightblue1",
    "mediumseagreen",
    "orange",
    "pink",
    "sienna"};

void adj_dfs(const adjmat_t matrix, const int vertex, const int code, int *visited)
{
    visited[vertex] = code;

    for (int i = 0; i < matrix.size; i++)
        if (matrix.matrix[vertex][i] && visited[i] != code)
            adj_dfs(matrix, i, code, visited);
}

int adj_is_conn(adjmat_t matrix, int **groups)
{
    if (!*groups)
        *groups = (int *)calloc(matrix.size, sizeof(int));

    int code = 1;
    adj_dfs(matrix, 0, code, *groups);

    for (int i = 0; i < matrix.size; ++i)
    {
        if ((*groups)[i])
            continue;

        adj_dfs(matrix, i, ++code, *groups);
    }

    return 1 == code;
}

void adj_gvexport(adjmat_t matrix, int *groups)
{
    FILE *export = fopen("graph.txt", "w");
    fprintf(export, "graph {\nnode ["
                    "margin=0 fontcolor=black fontsize=16 "
                    "width=0.5 shape=circle style=filled"
                    "]\n");

    for (int i = 0; i < matrix.size; i++)
        for (int j = 0; j < matrix.size; j++)
            if (i != j && matrix.matrix[i][j])
                matrix.matrix[j][i] = 0;

    for (int i = 0; i < matrix.size; i++)
    {
        for (int j = 0; j < matrix.size; j++)
            if (matrix.matrix[i][j])
                fprintf(export, "%d -- %d;\n", i, j);

        fprintf(export, "%d [fillcolor=%s];\n", i, a[groups[i] % COLOR_COUNT]);
    }

    fprintf(export, "}\n");
    fclose(export);

    system("dot -Tpng graph.txt -o graph.png");
    system("xdg-open graph.png");
}

//! ---------------------------------------- !//

void list_dfs(const listmat_t matrix, const int vertex, const int code, int *visited)
{
    visited[vertex] = code;

    node_t *t = matrix.matrix[vertex];
    for (int i = 0; i < matrix.size; i++)
    {
        if (find(t, &i, cmp) && visited[i] != code)
            list_dfs(matrix, i, code, visited);
    }
}

#include <stdio.h>
int list_is_conn(listmat_t matrix, int **groups)
{
    if (!*groups)
        *groups = (int *)calloc(matrix.size, sizeof(int));

    int code = 1;
    list_dfs(matrix, 0, code, *groups);

    for (int i = 0; i < matrix.size; ++i)
    {
        if ((*groups)[i])
            continue;

        list_dfs(matrix, i, ++code, *groups);
    }

    return 1 == code;
}

void list_gvexport(listmat_t matrix, int *groups)
{
    FILE *export = fopen("graph.txt", "w");
    fprintf(export, "graph {\nnode ["
                    "margin=0 fontcolor=black fontsize=16 "
                    "width=0.5 shape=circle style=filled"
                    "]\n");

    for (int i = 0, j = 0; i < matrix.size; i++, j = 0)
    {
        node_t *t = matrix.matrix[i];
        while (t)
        {
            j = *(type_t *)(t->data);
            if (i <= j)
                fprintf(export, "%d -- %d;\n", i, j);

            t = t->next;
        }

        fprintf(export, "%d [fillcolor=%s];\n", i, a[groups[i] % COLOR_COUNT]);
    }

    fprintf(export, "}\n");
    fclose(export);

    system("dot -Tpng graph.txt -o graph.png");
    system("xdg-open graph.png");
}
