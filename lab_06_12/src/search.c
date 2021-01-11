#include "../inc/hash_utils.h"
#include "../inc/search.h"

type_t **fill_words(FILE *f)
{
    rewind(f);

    unsigned long flen = file_len(f);

    type_t **words = malloc(sizeof(type_t) * flen);
    for (int i = 0; i < flen; ++i)
        words[i] = (type_t *)malloc(sizeof(char));
    int ind = 0;

    type_t val = 0;
    while (1 == fscanf(f, ELEM_FMT, &val))
    {
        memcpy(words[ind], &val, sizeof(type_t));
        ind++;
    }
    rewind(f);

    return words;
}

void free_words(type_t **words, unsigned long flen)
{
    for (int i = 0; i < flen; ++i)
        free(words[i]);

    free(words);
}

int search_tree(type_t *to_find, tree_node root)
{
    int i = 0;
    tree_node *p = &root;

    while (p != NULL)
    {
        i++;
        if (*(p->val) == *to_find)
            return i;

        if (*(p->val) > *to_find)
            p = p->right;
        else
            p = p->left;
    }

    return NOT_FOUND;
}

double search_tree_av(tree_node root, type_t **words, unsigned long flen)
{
    uint64_t time = tick();
    for (int i = 0; i < flen; ++i)
        search_tree(words[i], root);
    time = tick() - time;

    return (double)time / flen;
}

int search_hashtable(type_t *to_find, list_t *hash_list, int n, int (*hash)(type_t *, int))
{
    int ec = deepcmp(to_find, hash_list[hash(to_find, n)]);
    if (ec > 0)
        return ec;

    return NOT_FOUND;
}

double search_hashtable_av(list_t *hash_list, int n, int (*hash)(type_t *, int),
                           type_t **words, unsigned long flen)
{
    uint64_t time = tick();
    for (int i = 0; i < flen; ++i)
        search_hashtable(words[i], hash_list, n, hash);

    time = tick() - time;

    return (double)time / flen;
}

int search_file(type_t *to_find, FILE *f)
{
    rewind(f);

    int i = 0;
    type_t val = 0;
    while (1 == fscanf(f, ELEM_FMT, &val))
    {
        i++;

        if (*to_find == val)
            return i;
    }

    rewind(f);

    return NOT_FOUND;
}

double search_file_av(FILE *f, type_t **words, unsigned long flen)
{
    uint64_t time = tick();
    for (int i = 0; i < flen; ++i)
        search_file(words[i], f);
    time = tick() - time;

    return (double)time / flen;
}
