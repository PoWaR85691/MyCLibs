#include <stdio.h>
#include "dynamic_array.h"

void print_int_array(int *array)
{
    printf("length = %zu; capacity = %zu; item_size = %zu\n",
           array_length(array), array_capacity(array), array_item_size(array));
    printf("Items: { ");
    if (array_length(array) > 0)
    {
        for (size_t i = 0; i < array_length(array) - 1; ++i)
            printf("%d, ", array[i]);
        printf("%d", array[array_length(array) - 1]);
    }
    printf(" }\n");
}

void print_double_array(double *array)
{
    printf("length = %zu; capacity = %zu; item_size = %zu\n",
           array_length(array), array_capacity(array), array_item_size(array));
    printf("Items: { ");
    if (array_length(array) > 0)
    {
        for (size_t i = 0; i < array_length(array) - 1; ++i)
            printf("%g, ", array[i]);
        printf("%g", array[array_length(array) - 1]);
    }
    printf(" }\n");
}

int main(void)
{
    double *arr1 = MAKE_EMPTY_ARRAY(double);
    for (int i = 0; i < 10; ++i)
        ARRAY_ADD_CAST_ITEM(arr1, i + 0.5, double);

    print_double_array(arr1);
    
    double *arr2 = NULL;

    ARRAY_SET(arr2, arr1);
    print_double_array(arr2);

    print_double_array(arr1);
    ARRAY_FILL_CAST(arr1, 10.0, double);
    //ARRAY_FILL_CAST(arr1, 1.0, double);
    print_double_array(arr1);

    delete_array(arr1);
    delete_array(arr2);

    return 0;
}