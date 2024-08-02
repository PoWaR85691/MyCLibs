#include <stdio.h>
#include "list.h"

void print_list(int *tail)
{
    while (tail)
    {
        printf("%d ", *tail);
        LIST_MOVE_NEXT(tail);
    }
    printf("\n");
}

int main(void)
{
    int *tail = MAKE_LIST(int);
    int *head = tail;

    *head = 0;
    for (int i = 1; i < 10; ++i)
    {
        *LIST_INSERT_HEAD(head) = i*i;
    }

    print_list(tail);


    int *node = list_prev(head);
    LIST_MOVE_PREV(node);
    LIST_MOVE_PREV(node);
    list_remove_node(node);

    print_list(tail);

    delete_list(tail);

    return 0;
}