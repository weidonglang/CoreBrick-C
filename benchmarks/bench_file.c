#include <stdio.h>
#include <string.h>
#include "cb_file.h"
#include "cb_buffer.h"
#include "cb_timer.h"

void bench_file(void) {
    const char *tmp = "_bench_tmp.bin";
    char data[4096];
    memset(data, 0xAB, sizeof(data));

    /* Write benchmark */
    CB_Timer timer;
    cb_timer_start(&timer);
    for (int i = 0; i < 100; i++) {
        cb_file_write_all(tmp, data, sizeof(data));
    }
    double tw = cb_timer_elapsed_ms(&timer);
    printf("File write:   %.3f ms (100 x 4KB)\n", tw);

    /* Read benchmark */
    cb_timer_start(&timer);
    CB_Buffer buf;
    for (int i = 0; i < 100; i++) {
        cb_file_read_all(tmp, &buf);
        cb_buffer_free(&buf);
    }
    double tr = cb_timer_elapsed_ms(&timer);
    printf("File read:    %.3f ms (100 x 4KB)\n", tr);

    remove(tmp);
}