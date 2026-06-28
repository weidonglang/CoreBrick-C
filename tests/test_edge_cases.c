#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "cb_arena.h"
#include "cb_buffer.h"
#include "cb_string_view.h"

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

void test_edge_cases(void) {
    printf("\n--- Edge Cases ---\n");

    CB_Arena *arena = NULL;

    /* Arena: size 0 returns NULL */
    TEST("arena alloc size 0 returns NULL", cb_arena_alloc(arena, 0) == NULL);
    TEST("arena alloc_zero size 0 returns NULL", cb_arena_alloc_zero(arena, 0) == NULL);

    cb_arena_create(100, &arena);

    /* Arena: maximum capacity allocation with alignment */
    void *p = cb_arena_alloc(arena, 80);
    TEST("arena alloc 80 fits", p != NULL);

    /* After using most space, next alloc should fail */
    TEST("arena over capacity returns NULL", cb_arena_alloc(arena, 50) == NULL);
    cb_arena_destroy(arena);

    /* Alignment tests */
    cb_arena_create(128, &arena);
    void *a1 = cb_arena_alloc(arena, 1);
    TEST("alignment: first alloc non-null", a1 != NULL);
    void *a2 = cb_arena_alloc(arena, sizeof(double));
    TEST("alignment: double alloc non-null", a2 != NULL);
    TEST("alignment: double pointer aligned", ((uintptr_t)a2 % sizeof(double)) == 0);
    void *a3 = cb_arena_alloc(arena, sizeof(long double));
    TEST("alignment: struct alloc non-null", a3 != NULL);
    cb_arena_destroy(arena);

    /* Buffer: append len=0 keeps size */
    CB_Buffer buf;
    cb_buffer_init(&buf, 16);
    cb_buffer_append(&buf, "hello", 5);
    size_t before = buf.size;
    cb_buffer_append(&buf, "world", 0);
    TEST("buffer append len=0 does not change size", buf.size == before);
    TEST("buffer append NULL data returns error",
         cb_buffer_append(&buf, NULL, 5) == CB_ERR_INVALID_ARGUMENT);

    /* Buffer: large append */
    cb_buffer_clear(&buf);
    size_t large = 1024 * 1024;
    char *big = (char *)malloc(large);
    memset(big, 'A', large);
    TEST("buffer large append succeeds", cb_buffer_append(&buf, big, large) == CB_OK);
    TEST("buffer large append size correct", buf.size == large);
    free(big);
    cb_buffer_free(&buf);

    /* StringView edge cases */
    TEST("sv_find with NULL sv", cb_sv_find(cb_sv_from_parts(NULL, 0),
          cb_sv_from_cstr("x"), NULL) == 0);
    TEST("sv_starts_with NULL prefix",
         cb_sv_starts_with(cb_sv_from_cstr("hello"), cb_sv_from_parts(NULL, 0)) == 0);
    TEST("sv_ends_with NULL suffix",
         cb_sv_ends_with(cb_sv_from_cstr("hello"), cb_sv_from_parts(NULL, 0)) == 0);

    /* Split empty string */
    CB_StringView empty = cb_sv_from_parts("", 0);
    CB_SplitIter sit = cb_sv_split(empty, ',');
    CB_StringView part;
    TEST("split empty string yields one empty part", cb_sv_split_next(&sit, &part) == 1);
    TEST("split empty string part is empty", part.len == 0);

    /* to_i64 / to_f64 edge cases */
    int64_t i64val;
    double f64val;
    TEST("to_i64 large number", cb_sv_to_i64(cb_sv_from_cstr("9223372036854775807"), &i64val) == CB_OK);
    TEST("to_i64 overflow fails", cb_sv_to_i64(cb_sv_from_cstr("9223372036854775808"), &i64val) == CB_ERR_PARSE);
    TEST("to_f64 negative", cb_sv_to_f64(cb_sv_from_cstr("-3.14"), &f64val) == CB_OK);

    /* Arena: reset + re-alloc loop */
    cb_arena_create(256, &arena);
    for (int i = 0; i < 10; i++) {
        void *pp = cb_arena_alloc(arena, 16);
        TEST("arena used>0 after alloc", pp != NULL);
        cb_arena_reset(arena);
        TEST("arena used=0 after reset", cb_arena_used(arena) == 0);
    }
    cb_arena_destroy(arena);

    /* Buffer: clear resets size */
    CB_Buffer b2;
    cb_buffer_init(&b2, 16);
    for (int i = 0; i < 5; i++) {
        cb_buffer_append(&b2, "x", 1);
        cb_buffer_clear(&b2);
        TEST("buffer clear resets size", b2.size == 0);
    }
    cb_buffer_free(&b2);

    /* StringView: trim empty */
    CB_StringView trimmed = cb_sv_trim_ascii(cb_sv_from_parts("", 0));
    TEST("trim empty string", trimmed.len == 0);
    TEST("trim NULL sv", cb_sv_trim_ascii(cb_sv_from_parts(NULL, 0)).len == 0);
}