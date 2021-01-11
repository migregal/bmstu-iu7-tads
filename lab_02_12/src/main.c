#include "my_menu.h"
#include "data_types.h"

#define OK 0

int main()
{
    aio_table_t table;
    table.size_of_table = 0;

    while (process_menu(&table))
        ;

    return OK;
}
