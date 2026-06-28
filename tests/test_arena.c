#include <stdio.h>
#include <string.h>
#include "cb_arena.h"

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

void test_arena(void) {
    printf("\n--- Arena Module ---\n");

    CB_Arena *arena = NULL;

    /* create success */
    TEST("cb_arena_create success",
         cb_arena_create(1024, &arena) == CB_OK && arena != NULL);

    /* capacity / used correct */
    TEST("capacity is 1024", cb_arena_capacity(arena) == 1024);
    TEST("used is 0", cb_arena_used(arena) == 0);

    /* alloc success */
    int *arr = (int *)cb_arena_alloc(arena, 10 * sizeof(int));
    TEST("alloc returns non-null", arr != NULL);

    /* used increases after alloc */
    TEST("used increases after alloc", cb_arena_used(arena) >= 10 * sizeof(int));

    /* alloc_zero returns zeroed memory */
    void *zero_block = cb_arena_alloc_zero(arena, 100);
    TEST("alloc_zero returns non-null", zero_block != NULL);
    {
        unsigned char *bytes = (unsigned char *)zero_block;
        int all_zero = 1;
        for (size_t i = 0; i < 100; i++) {
            if (bytes[i] != 0) { all_zero = 0; break; }
        }
        TEST("alloc_zero memory is zeroed", all_zero);
    }

    /* over capacity returns NULL */
    size_t remaining = cb_arena_capacity(arena) - cb_arena_used(arena);
    void *large = cb_arena_alloc(arena, remaining + 1);
    TEST("over capacity returns NULL", large == NULL);

    /* reset sets used to 0 */
    TEST("reset success", cb_arena_reset(arena) == CB_OK);
    TEST("used is 0 after reset", cb_arena_used(arena) == 0);

    /* reset allows re-allocation */
    void *after_reset = cb_arena_alloc(arena, 500);
    TEST("re-allocation after reset succeeds", after_reset != NULL);

    /* destroy NULL does not crash */
    cb_arena_destroy(NULL);
    TEST("destroy NULL does not crash", 1);

    /* destroy */
    cb_arena_destroy(arena);

    /* capacity 0 failure */
    CB_Arena *bad_arena = NULL;
    TEST("capacity 0 returns error",
         cb_arena_create(0, &bad_arena) == CB_ERR_INVALID_ARGUMENT);

    /* out_arena NULL failure */
    TEST("out_arena NULL returns error",
         cb_arena_create(1024, NULL) == CB_ERR_INVALID_ARGUMENT);

    /* NULL arena functions return safe values */
    TEST("arena_used(NULL) returns 0", cb_arena_used(NULL) == 0);
    TEST("arena_capacity(NULL) returns 0", cb_arena_capacity(NULL) == 0);
    TEST("arena_alloc(NULL, 10) returns NULL", cb_arena_alloc(NULL, 10) == NULL);
    TEST("arena_alloc_zero(NULL, 10) returns NULL", cb_arena_alloc_zero(NULL, 10) == NULL);
    TEST("arena_reset(NULL) returns error",
         cb_arena_reset(NULL) == CB_ERR_INVALID_ARGUMENT);
}