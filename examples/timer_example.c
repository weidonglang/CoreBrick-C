#include <stdio.h>
#include "cb_timer.h"

int main(void) {
    printf("=== Timer Example ===\n");

    CB_Timer timer;
    cb_timer_start(&timer);

    /* Simulate work */
    volatile unsigned long long sum = 0;
    for (int i = 0; i < 1000000; i++) {
        sum += (unsigned long long)i;
    }

    double elapsed = cb_timer_elapsed_ms(&timer);
    printf("Sum: %llu\n", sum);
    printf("Elapsed: %.3f ms\n", elapsed);

    return 0;
}