#include <stdio.h>
#include "cb_vector.h"
#include "cb_timer.h"

void bench_vector(void) {
    CB_Vector vec;
    cb_vector_init(&vec, sizeof(int), 4);

    CB_Timer timer;
    cb_timer_start(&timer);

    for (int i = 0; i < 100000; i++) {
        cb_vector_push(&vec, &i);
    }

    double elapsed = cb_timer_elapsed_ms(&timer);
    printf("Vector push: %.3f ms (100000 elements)\n", elapsed);

    cb_vector_free(&vec);
}