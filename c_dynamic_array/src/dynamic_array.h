#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>
#include <string.h>

#define ARRAY_DEFAULT_CAPACITY 4
#define ARRAY_REALLOC_COEF 2

void* make_array(size_t length, size_t item_size);
#define MAKE_ARRAY(LENGTH, TYPE) ((TYPE*)make_array((LENGTH), sizeof(TYPE)))
#define MAKE_EMPTY_ARRAY(TYPE) MAKE_ARRAY(0, TYPE)
void delete_array(void *array);

size_t array_length(const void *array);
size_t array_capacity(const void *array);
size_t array_item_size(const void *array);

void* array_realloc(void *array, size_t new_capacity);
#define ARRAY_REALLOC(ARRAY, NEW_CAPACITY) ((ARRAY) = array_realloc((ARRAY), (NEW_CAPACITY)))
#define ARRAY_SHRINK(ARRAY) ARRAY_REALLOC((ARRAY), array_length(ARRAY))
void* array_resize(void *array, size_t new_length);
#define ARRAY_RESIZE(ARRAY, NEW_LENGTH) ((ARRAY) = array_resize((ARRAY), (NEW_LENGTH)))
#define ARRAY_CLEAR(ARRAY) ARRAY_RESIZE((ARRAY), 0)

void* array_add_item(void *array, const void *item);
#define ARRAY_ADD_ITEM(ARRAY, ITEM) ((ARRAY) = array_add_item((ARRAY), &(ITEM)))
#define ARRAY_ADD_CAST_ITEM(ARRAY, ITEM, TYPE) ARRAY_ADD_ITEM(ARRAY, (TYPE){(ITEM)})
void* array_remove_item(void *array);
#define ARRAY_REMOVE_ITEM(ARRAY) ((ARRAY) = array_remove_item((ARRAY)))

void* array_fill(void *array, const void *value);
#define ARRAY_FILL(ARRAY, VALUE) array_fill((ARRAY), &(VALUE));
#define ARRAY_FILL_CAST(ARRAY, VALUE, TYPE) ARRAY_FILL((ARRAY), (TYPE){(VALUE)})

void* array_copy(const void *array);
void* array_set(void *array_dest, const void *array_src);
#define ARRAY_SET(ARRAY_DEST, ARRAY_SRC) ((ARRAY_DEST) = array_set((ARRAY_DEST), (ARRAY_SRC)))

#endif