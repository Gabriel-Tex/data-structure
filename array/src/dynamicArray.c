#include "../include/dynamicArray.h"

void initialize(DynamicArray *arr, int initialCapacity)
{
    arr->data = malloc(initialCapacity * sizeof(int));
    arr->size = 0;
    arr->capacity = initialCapacity;
}

void insert(DynamicArray *arr, int value)
{
    if (arr->size >= arr->capacity)
    {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(int));
    }

    arr->data[arr->size] = value;
    arr->size++;
}

int access(DynamicArray *arr, int index)
{
    if (index < 0 || index >= arr->size)
    {
        printf("Índice inválido!\n");
        exit(1);
    }
    return arr->data[index];
}

void freeArray(DynamicArray *arr)
{
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}
