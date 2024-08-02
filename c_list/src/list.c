#include "list.h"

typedef struct
{
    void *prev_node;
    void *next_node;
} node_param;

static void* param_to_data(node_param *param)
{
    return ((void*)param + sizeof(node_param));
}

static node_param* data_to_param(void *data)
{
    return (data - sizeof(node_param));
}
/* ***************************************************** */

void* make_list(size_t item_size)
{
    node_param *param_data = malloc(sizeof(node_param) + item_size);
    if (!param_data)
        return NULL;
    param_data->prev_node = NULL;
    param_data->next_node = NULL;
    return param_to_data(param_data);
}

void* list_insert_before(void *node, size_t item_size)
{
    if (!node)
        return make_list(item_size);

    node_param *new_param_data = malloc(sizeof(node_param) + item_size);
    if (!new_param_data)
        return NULL;

    void *prev_node = list_prev(node);
    void *new_node = param_to_data(new_param_data);

    if (prev_node)
        data_to_param(prev_node)->next_node = new_node;
    data_to_param(node)->prev_node = new_node;

    new_param_data->prev_node = prev_node;
    new_param_data->next_node = node;

    return new_node;
}

void* list_insert_after(void *node, size_t item_size)
{
    if (!node)
        return make_list(item_size);

    node_param *new_param_data = malloc(sizeof(node_param) + item_size);
    if (!new_param_data)
        return NULL;
    
    void *new_node = param_to_data(new_param_data);
    void *next_node = list_next(node);

    data_to_param(node)->next_node = new_node;
    if (next_node)
        data_to_param(next_node)->prev_node = new_node;

    new_param_data->prev_node = node;
    new_param_data->next_node = next_node;

    return new_node;
}

void* list_prev(const void *node)
{
    return data_to_param((void*)node)->prev_node;
}

void* list_next(const void *node)
{
    return data_to_param((void*)node)->next_node;
}

void list_remove_node(void *node)
{
    if (!node)
        return;

    void *prev_node = list_prev(node);
    void *next_node = list_next(node);

    if (prev_node)
        data_to_param(prev_node)->next_node = next_node;
    if (next_node)
        data_to_param(next_node)->prev_node = prev_node;
    
    free(data_to_param(node));
}

void delete_list(void *list_node)
{
    if (!list_node)
        return;

    void *backward = list_prev(list_node);
    void *forward = list_node;

    while (backward)
    {
        void *tmp_node = backward;
        LIST_MOVE_PREV(backward);
        free(data_to_param(tmp_node));
    }
    while (forward)
    {
        void *tmp_node = forward;
        LIST_MOVE_NEXT(forward);
        free(data_to_param(tmp_node));
    }
}