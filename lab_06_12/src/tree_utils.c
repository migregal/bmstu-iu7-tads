#include "../inc/tree_utils.h"

tree_node *create_node(type_t *val)
{
    tree_node *node = malloc(sizeof(tree_node));

    node->val = malloc(sizeof(type_t));
    memcpy(node->val, val, sizeof(type_t));

    node->left = NULL;
    node->right = NULL;

    return node;
}

tree_node *insert(tree_node *node, type_t *val)
{
    if (node == NULL)
        return create_node(val);

    if (*val > *(node->val))
        node->left = insert(node->left, val);
    else if (*val < *(node->val))
        node->right = insert(node->right, val);

    return node;
}

void fill_tree(tree_node **root, FILE *f)
{
    type_t val;

    fscanf(f, ELEM_FMT, &val);

    *root = insert(*root, &val);

    while (1 == fscanf(f, ELEM_FMT, &val))
        insert(*root, &val);

    rewind(f);
}

void print_tree(tree_node *root, int first_call, char *filename)
{
    if (!root)
        return;

    if (first_call)
    {
        FILE *export = fopen(filename, "w");
        fprintf(export, "graph {\nnode ["
                        "margin=0 fontcolor=black fontsize=10 "
                        "width=0.2 shape=circle style=filled"
                        "]\n");
        fclose(export);
    }

    print_tree(root->left, 0, filename);

    FILE *export = fopen(filename, "a");

    if (root->left && root->left->val)
        fprintf(export, ELEM_FMT " -- " ELEM_FMT ";\n", *(root->val), *(root->left->val));
    if (root->right && root->right->val)
        fprintf(export, ELEM_FMT " -- " ELEM_FMT ";\n", *(root->val), *(root->right->val));
    fclose(export);

    print_tree(root->right, 0, filename);

    if (first_call)
    {
        FILE *export = fopen(filename, "a");
        fprintf(export, "}\n");
        fclose(export);

        char *str = malloc((20 + 2 * strlen(filename)) * sizeof(char));

        snprintf(str, strlen(str), "dot -Tpng %s -o %s.png", filename, filename);
        system(str);

        snprintf(str, strlen(str), "xdg-open %s.png", filename);
        system(str);

        free(str);
    }
}

void tree_depth(tree_node *root, int *vrtxs, int *cmprs, int depth)
{
    if (!root)
        return;

    (*vrtxs)++;
    (*cmprs) += depth;

    tree_depth(root->right, vrtxs, cmprs, depth + 1);
    tree_depth(root->left, vrtxs, cmprs, depth + 1);
}

void push_back(dynarr_t *arr, tree_node *to_add)
{
    if (arr->size >= arr->mem_size)
    {
        arr->mem_size *= 2;
        arr->arr = realloc(arr->arr, arr->mem_size * sizeof(tree_node *));
    }

    arr->arr[arr->size++] = to_add;
}

void store_nodes(tree_node *root, dynarr_t *arr)
{
    if (root == NULL)
        return;

    store_nodes(root->left, arr);
    push_back(arr, root);
    store_nodes(root->right, arr);
}

tree_node *build_tree(dynarr_t *arr, int start, int end)
{
    if (start > end)
        return NULL;

    int mid = (start + end) / 2;
    tree_node *root = arr->arr[mid];

    root->left = build_tree(arr, start, mid - 1);
    root->right = build_tree(arr, mid + 1, end);

    return root;
}

tree_node *balance_tree(tree_node *root, int *unique)
{
    dynarr_t arr;
    arr.mem_size = 8;
    arr.size = 0;
    arr.arr = malloc(sizeof(tree_node *) * arr.mem_size);
    store_nodes(root, &arr);

    int n = arr.size;
    *unique = n;

    tree_node *r = build_tree(&arr, 0, n - 1);

    free(arr.arr);

    return r;
}

void free_tree(tree_node *node)
{
    if (node == NULL)
        return;

    free_tree(node->left);
    free_tree(node->right);

    free(node);
}
