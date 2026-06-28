#include <stdio.h>
#include <string.h>
#include "cb_hash.h"
#include "cb_timer.h"

void bench_hash(void) {
    char buffer[1024];
    memset(buffer, 'A', sizeof(buffer));

    CB_Timer timer;

    cb_timer_start(&timer);
    uint64_t result64 = 0;
    for (int i = 0; i < 100000; i++) {
        result64 += cb_hash_fnv1a64(buffer, sizeof(buffer));
    }
    double t64 = cb_timer_elapsed_ms(&timer);
    printf("Hash fnv1a64: %.3f ms (100000 x 1KB)\n", t64);

    cb_timer_start(&timer);
    uint32_t result32 = 0;
    for (int i = 0; i < 100000; i++) {
        result32 += cb_hash_fnv1a32(buffer, sizeof(buffer));
    }
    double t32 = cb_timer_elapsed_ms(&timer);
    printf("Hash fnv1a32: %.3f ms (100000 x 1KB)\n", t32);

    cb_timer_start(&timer);
    uint32_t result_crc = 0;
    for (int i = 0; i < 100000; i++) {
        result_crc += cb_crc32(buffer, sizeof(buffer));
    }
    double tcrc = cb_timer_elapsed_ms(&timer);
    printf("Hash crc32:   %.3f ms (100000 x 1KB)\n", tcrc);

    (void)result64; (void)result32; (void)result_crc;
}