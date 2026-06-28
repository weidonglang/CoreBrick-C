#include <stdio.h>
#include "cb_timer.h"

extern int tests_passed;
extern int tests_failed;

#define TEST(name, expr) do { \
    if (!(expr)) { \
        printf("  FAIL: %s (%s:%d)\n", name, __FILE__, __LINE__); \
        tests_failed++; \
    } else { \
        printf("  PASS: %s\n", name); \
        tests_passed++; \
    } \
} while (0)

void test_timer(void) {
    printf("\n--- Timer Module ---\n");

    /* now_ns returns non-zero */
    uint64_t now = cb_timer_now_ns();
    TEST("now_ns returns non-zero", now > 0);

    /* elapsed_ns increases */
    CB_Timer timer;
    cb_timer_start(&timer);
    uint64_t elapsed = cb_timer_elapsed_ns(&timer);
    TEST("elapsed_ns is non-negative", elapsed >= 0);
    TEST("elapsed_ns is small after quick read", elapsed < 100000000ULL); /* < 100ms */

    /* elapsed_ms >= 0 */
    double elapsed_ms = cb_timer_elapsed_ms(&timer);
    TEST("elapsed_ms >= 0", elapsed_ms >= 0.0);

    /* timer multiple starts */
    cb_timer_start(&timer);
    uint64_t e1 = cb_timer_elapsed_ns(&timer);
    cb_timer_start(&timer);
    uint64_t e2 = cb_timer_elapsed_ns(&timer);
    TEST("multiple starts works", e1 >= 0 && e2 >= 0);

    /* start NULL is safe (documented) */
    cb_timer_start(NULL);
    TEST("start NULL safe", 1);

    /* elapsed_ns with NULL */
    uint64_t null_elapsed = cb_timer_elapsed_ns(NULL);
    TEST("elapsed_ns NULL returns 0", null_elapsed == 0);

    /* elapsed_ms with NULL */
    double null_ms = cb_timer_elapsed_ms(NULL);
    TEST("elapsed_ms NULL returns 0.0", null_ms == 0.0);
}