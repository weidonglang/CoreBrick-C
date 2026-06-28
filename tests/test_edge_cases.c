#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "corebrick.h"

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

    /* Arena: alloc size 0 returns NULL */
    CB_Arena *arena = NULL;
    cb_arena_create(1024, &arena);
    TEST("arena alloc size 0 returns NULL", cb_arena_alloc(arena, 0) == NULL);
    TEST("arena alloc_zero size 0 returns NULL", cb_arena_alloc_zero(arena, 0) == NULL);
    cb_arena_destroy(arena);

    /* Arena: maximum capacity allocation */
    cb_arena_create(100, &arena);
    void *p = cb_arena_alloc(arena, 100);
    TEST("arena alloc exactly capacity", p != NULL);
    TEST("arena over capacity returns NULL", cb_arena_alloc(arena, 1) == NULL);
    cb_arena_destroy(arena);

    /* Buffer: append len=0 keeps size */
    CB_Buffer buf;
    cb_buffer_init(&buf, 16);
    cb_buffer_append(&buf, "hello", 5);
    size_t before = buf.size;
    cb_buffer_append(&buf, "world", 0);
    TEST("buffer append len=0 does not change size", buf.size == before);
    cb_buffer_free(&buf);

    /* Buffer: NULL data append */
    cb_buffer_init(&buf, 16);
    TEST("buffer append NULL data returns error",
         cb_buffer_append(&buf, NULL, 5) == CB_ERR_INVALID_ARGUMENT);
    cb_buffer_free(&buf);

    /* Buffer: large append */
    cb_buffer_init(&buf, 4);
    size_t large_size = 1024 * 10;
    char *big_data = (char *)malloc(large_size);
    memset(big_data, 'A', large_size);
    TEST("buffer large append succeeds",
         cb_buffer_append(&buf, big_data, large_size) == CB_OK);
    TEST("buffer large append size correct", buf.size == large_size);
    free(big_data);
    cb_buffer_free(&buf);

    /* StringView: find with NULL sv */
    size_t fpos;
    TEST("sv_find with NULL sv",
         !cb_sv_find(cb_sv_from_cstr(NULL), cb_sv_from_cstr("a"), NULL));

    /* StringView: starts_with with NULL */
    TEST("sv_starts_with NULL prefix",
         !cb_sv_starts_with(cb_sv_from_cstr("abc"), cb_sv_from_cstr(NULL)));

    /* StringView: ends_with with NULL */
    TEST("sv_ends_with NULL suffix",
         !cb_sv_ends_with(cb_sv_from_cstr("abc"), cb_sv_from_cstr(NULL)));

    /* StringView: split with empty input */
    CB_SplitIter iter = cb_sv_split(cb_sv_from_cstr(""), ',');
    CB_StringView part;
    int count = 0;
    while (cb_sv_split_next(&iter, &part)) {
        count++;
    }
    TEST("split empty string yields one empty part", count == 1);
    TEST("split empty string part is empty",
         part.len == 0 && part.data != NULL);

    /* StringView: to_i64 large number */
    int64_t big;
    TEST("to_i64 large number",
         cb_sv_to_i64(cb_sv_from_cstr("9223372036854775807"), &big) == CB_OK &&
         big == 9223372036854775807LL);

    /* StringView: to_i64 overflow */
    TEST("to_i64 overflow fails",
         cb_sv_to_i64(cb_sv_from_cstr("99999999999999999999"), &big) == CB_ERR_PARSE);

    /* StringView: to_f64 negative */
    double d;
    TEST("to_f64 negative",
         cb_sv_to_f64(cb_sv_from_cstr("-2.5"), &d) == CB_OK && d == -2.5);

    /* Multiple arena resets */
    cb_arena_create(256, &arena);
    for (int i = 0; i < 10; i++) {
        cb_arena_alloc(arena, 20);
        cb_arena_reset(arena);
        TEST("arena used=0 after reset", cb_arena_used(arena) == 0);
    }
    cb_arena_destroy(arena);

    /* Buffer: multiple clear/append cycles */
    cb_buffer_init(&buf, 8);
    for (int i = 0; i < 5; i++) {
        cb_buffer_append_cstr(&buf, "test");
        cb_buffer_clear(&buf);
        TEST("buffer clear resets size", buf.size == 0);
    }
    cb_buffer_free(&buf);

    /* StringView: trim on empty */
    CB_StringView trimmed_empty = cb_sv_trim_ascii(cb_sv_from_cstr(""));
    TEST("trim empty string", trimmed_empty.len == 0);

    /* StringView: trim on NULL */
    trimmed_empty = cb_sv_trim_ascii(cb_sv_from_cstr(NULL));
    TEST("trim NULL sv", trimmed_empty.data == NULL && trimmed_empty.len == 0);
}