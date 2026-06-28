#include <stdio.h>
#include "cb_timer.h"

void bench_timer_overhead(void) {
    CB_Timer timer;
    CB_Timer overhead_timer;

    cb_timer_start(&overhead_timer);
    for (int i = 0; i < 100000; i++) {
        cb_timer_start(&timer);
        cb_timer_elapsed_ns(&timer);
    }
    double elapsed = cb_timer_elapsed_ms(&overhead_timer);
    printf("Timer overhead: %.3f ms (100000 start/elapsed pairs)\n", elapsed);
}