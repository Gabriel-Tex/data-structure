#define DYNAMICARRAY
#ifdef DYNAMICARRAY

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int *data;
    int size;
    int capacity;
} DynamicArray;

void initialize(DynamicArray *arr, int initialCapacity);

void insert(DynamicArray *arr, int value);

int access(DynamicArray *arr, int index);

void freeArray(DynamicArray *arr);

#endif