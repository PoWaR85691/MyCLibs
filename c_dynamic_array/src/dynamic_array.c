#include "dynamic_array.h"

// **********************************************************
typedef struct
{
    size_t length;
    size_t capacity;
    size_t item_size;
} dynamic_array_param;

static void* param_to_array(dynamic_array_param *param)
{
    return ((void*)param + sizeof(dynamic_array_param));
}

static dynamic_array_param* array_to_param(void *array)
{
    return (array - sizeof(dynamic_array_param));
}
// **********************************************************

void* make_array(size_t length, size_t item_size)
{
    dynamic_array_param *param_data = malloc(sizeof(dynamic_array_param) + length*item_size);
    if (!param_data)
        return NULL;
    param_data->length = length;
    param_data->capacity = length;
    param_data->item_size = item_size;

    return param_to_array(param_data);
}

void delete_array(void *array)
{
    if (array)
        free(array_to_param(array));
}

size_t array_length(const void *array)
{
    return array_to_param((void*)array)->length;
}

size_t array_capacity(const void *array)
{
    return array_to_param((void*)array)->capacity;
}

size_t array_item_size(const void *array)
{
    return array_to_param((void*)array)->item_size;
}

void* array_realloc(void *array, size_t new_capacity)
{
    dynamic_array_param *param_data = array_to_param(array);
    param_data = realloc(param_data, 
                         sizeof(dynamic_array_param) + new_capacity*param_data->item_size);
    if (!param_data)
        return NULL;
    param_data->capacity = new_capacity;
    if (param_data->length > new_capacity)
        param_data->length = new_capacity;
    return (param_to_array(param_data));
}

void* array_resize(void *array, size_t new_length)
{
    if (array_capacity(array) < new_length)
    {
        ARRAY_REALLOC(array, new_length);
        if (!array)
            return NULL;
    } 
    array_to_param(array)->length = new_length;
    return array;
}

void* array_add_item(void *array, const void *item)
{
    if (array_capacity(array) < array_length(array) + 1)
    {
        size_t new_capacity = array_capacity(array)*ARRAY_REALLOC_COEF;
        if (new_capacity < ARRAY_DEFAULT_CAPACITY)
            new_capacity = ARRAY_DEFAULT_CAPACITY;
        ARRAY_REALLOC(array, new_capacity);
        if (!array)
            return NULL;
    }
    memcpy(array + array_length(array)*array_item_size(array),
           item, array_item_size(array));
    array_to_param(array)->length += 1;
    return array;
}

void* array_remove_item(void *array)
{
    if (array_length(array) == 0)
        return NULL;
    array_to_param(array)->length -= 1;
    return array;
}

void* array_fill(void *array, const void *value)
{
    for (size_t i = 0; i < array_length(array); ++i)
        memcpy((array + i*array_item_size(array)), value, array_item_size(array));
    return array;
}

void* array_copy(const void *array)
{
    void *copy = make_array(array_length(array), array_item_size(array));
    if (!copy)
        return NULL;
    memcpy(copy, array, array_length(array)*array_item_size(array));
    return copy;
}

void* array_set(void *array_dest, const void *array_src)
{
    if (!array_dest)
        return array_copy(array_src);
    if (array_item_size(array_dest) != array_item_size(array_src))
        return NULL;
    ARRAY_RESIZE(array_dest, array_length(array_src));
    if (!array_dest)
        return NULL;
    memcpy(array_dest, array_src, array_length(array_src)*array_item_size(array_src));
    return array_dest;
}