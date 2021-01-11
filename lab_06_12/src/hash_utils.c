#include "../inc/hash_utils.h"

int is_prime(unsigned int num)
{
    if (num <= 1)
        return 0;

    if (num % 2 == 0 && num > 2)
        return 0;

    for (int i = 3; i < floor(sqrt(num)); i += 2)
        if (num % i == 0)
            return 0;

    return 1;
}

unsigned int next_prime(int n)
{
    unsigned int prime = n + 1;
    while (!is_prime(prime))
        prime++;

    return prime;
}

int safe_hash(type_t *s, int n)
{
    ull key = *s;

    key = (~key) + (key << 21);
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8);
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4);
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key % n;

    // Knuth's metod ("The Art of Computer Programming", section 6.4)
    // return ((ull)*s * 2654435761 % ((ull)1 << 32)) % n;
}

int unsafe_hash(type_t *s, const int n)
{
    return (*s) % (n / 2);
}

int file_len(FILE *f)
{
    rewind(f);

    int lines = 0;

    type_t val = 0;
    while (1 == fscanf(f, ELEM_FMT, &val))
        lines++;

    rewind(f);

    return lines;
}

int fbytes(FILE *f)
{
    rewind(f);
    fseek(f, 0, SEEK_END);
    unsigned size = ftell(f);
    rewind(f);

    return size;
}

int build_hash_table(list_t **arr, int size, FILE *f, int (*hash)(type_t *, int))
{
    type_t val;
    int max_collisions = 0;

    while (1 == fscanf(f, ELEM_FMT, &val))
    {
        int cur_collisions = 0;

        if (deepcmp(&val, (*arr)[hash(&val, size)]) == 0)
            cur_collisions = linsert(&(*arr)[hash(&val, size)], &val);

        if (cur_collisions > max_collisions)
            max_collisions = cur_collisions;
    }

    rewind(f);

    return max_collisions;
}

void print_hash_table(list_t *arr, int size)
{
    printf(ANSI_GREEN
           "\n------------------------------------------\n"
           "  ХЕШ | СООТВЕТСТВУЮЩИЕ ДАННЫЕ\n"
           "------------------------------------------\n" ANSI_RESET);
    list_t *entry;

    int f = 0;

    for (int i = 0; i < size; i++)
    {
        f = 0;

        entry = &arr[i];
        if (entry->value != NULL)
        {
            printf("%5d | ", i);
            f = 1;
        }
        while (entry != NULL)
        {
            if (entry->value != NULL)
                printf(ELEM_FMT " ", *(entry->value));
            entry = entry->next;
        }
        if (f)
            printf("\n------------------------------------------\n");
    }
}
