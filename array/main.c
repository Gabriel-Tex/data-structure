#include <stdio.h>
#include <stdlib.h>

#include "./include/dynamicArray.h"

int main()
{
    // array estático
    int A[] = {1, 6, 35, 2, 21, 7};

    printf("\nElemento no índice 3 do array estático: %d\n", A[3]);

    // array dinâmico
    DynamicArray arr;
    initialize(&arr, 1);

    insert(&arr, 1);
    insert(&arr, 6);
    insert(&arr, 35);
    insert(&arr, 2);
    insert(&arr, 21);
    insert(&arr, 7);

    printf("\nElemento no índice 3 do array dinâmico: %d\n", access(&arr, 3));

    freeArray(&arr);
    return 0;
}
