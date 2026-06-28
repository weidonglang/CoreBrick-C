#include <stdio.h>
#include "cb_ring.h"

int main(void) {
    printf("=== RingBuffer Example ===\n");

    CB_Ring ring;
    cb_ring_init(&ring, sizeof(int), 4);

    for (int i = 1; i <= 4; i++) {
        cb_ring_push(&ring, &i);
        printf("Pushed: %d\n", i);
    }

    printf("Full: %s\n", cb_ring_is_full(&ring) ? "yes" : "no");

    int val;
    while (!cb_ring_is_empty(&ring)) {
        cb_ring_pop(&ring, &val);
        printf("Popped: %d\n", val);
    }

    cb_ring_free(&ring);
    return 0;
}