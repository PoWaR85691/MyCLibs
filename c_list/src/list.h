#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

void* make_list(size_t item_size);
#define MAKE_LIST(TYPE) ((TYPE*)make_list(sizeof(TYPE)))

void* list_insert_before(void *node, size_t item_size);
void* list_insert_after(void *node, size_t item_size);
#define LIST_INSERT_BEFORE(NODE) list_insert_before((NODE), sizeof(*(NODE)))
#define LIST_INSERT_AFTER(NODE) list_insert_after((NODE), sizeof(*(NODE)))
#define LIST_INSERT_TAIL(TAIL) ((TAIL) = LIST_INSERT_BEFORE(TAIL))
#define LIST_INSERT_HEAD(HEAD) ((HEAD) = LIST_INSERT_AFTER(HEAD))

void* list_prev(const void *node);
void* list_next(const void *node);
#define LIST_MOVE_PREV(NODE) ((NODE) = list_prev(NODE))
#define LIST_MOVE_NEXT(NODE) ((NODE) = list_next(NODE))

void list_remove_node(void *node);
void delete_list(void *list_node);

#endif