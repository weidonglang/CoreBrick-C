#include <stdio.h>
#include "cb_vector.h"

int main(void) {
    printf("=== Vector Example ===\n");

    CB_Vector vec;
    cb_vector_init(&vec, sizeof(int), 4);

    for (int i = 0; i < 10; i++) {
        cb_vector_push(&vec, &i);
    }

    printf("Vector size: %zu, capacity: %zu\n", vec.size, vec.capacity);
    printf("Elements: ");
    for (size_t i = 0; i < vec.size; i++) {
        int val;
        cb_vector_get(&vec, i, &val);
        printf("%d ", val);
    }
    printf("\n");

    int last;
    cb_vector_pop(&vec, &last);
    printf("Popped: %d, new size: %zu\n", last, vec.size);

    cb_vector_free(&vec);
    return 0;
}