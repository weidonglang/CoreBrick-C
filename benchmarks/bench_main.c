#include <stdio.h>
#include "cb_timer.h"

void bench_vector(void);
void bench_hash(void);
void bench_file(void);
void bench_timer_overhead(void);

int main(void) {
    printf("=== CoreBrick-C Benchmark ===\n\n");

    bench_vector();
    bench_hash();
    bench_file();
    bench_timer_overhead();

    return 0;
}