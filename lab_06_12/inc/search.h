#ifndef __SEARCH_H__
#define __SEARCH_H__

#include <stdio.h>

#include "rc.h"
#include "macro.h"

#include "tree_utils.h"
#include "list_utils.h"
#include "timer.h"

type_t **fill_words(FILE *f);

void free_words(type_t **words, unsigned long flen);

int search_tree(type_t *to_find, tree_node root);

double search_tree_av(tree_node root, type_t **words, unsigned long flen);

int search_hashtable(type_t *to_find, list_t *hash_list, int n, int (*hash)(type_t *, int));

double search_hashtable_av(list_t *hash_list, int n, int (*hash)(type_t *, int), type_t **words, unsigned long flen);

int search_file(type_t *to_find, FILE *f);

double search_file_av(FILE *f, type_t **words, unsigned long flen);

#endif
