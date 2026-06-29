/*
 * test_install.c — Verify that installed headers and library are usable
 *
 * This test is compiled outside the main build tree using find_package
 * to verify the installed CMake config works correctly.
 *
 * Compile manually after `cmake --install`:
 *   gcc -o test_install test_install.c -I<install_prefix>/include -L<install_prefix>/lib -lcorebrick
 *
 * Or via CMake:
 *   find_package(CoreBrick CONFIG REQUIRED)
 *   target_link_libraries(myapp PRIVATE CoreBrick::corebrick)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "corebrick.h"

/* Assert that never gets disabled by NDEBUG */
#define CHECK(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "FAIL at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        return 1; \
    } \
} while(0)

static int test_version(void) {
    printf("Version: %s\n", COREBRICK_VERSION_STRING);
    CHECK(strlen(COREBRICK_VERSION_STRING) > 0);
    CHECK(COREBRICK_VERSION_MAJOR == 0);
    CHECK(COREBRICK_VERSION_MINOR >= 4);
    printf("  Version macros: %d.%d.%d\n",
           COREBRICK_VERSION_MAJOR,
           COREBRICK_VERSION_MINOR,
           COREBRICK_VERSION_PATCH);
    return 0;
}

static int test_arena(void) {
    CB_Arena *arena = NULL;
    CHECK(cb_arena_create(256, &arena) == CB_OK);
    CHECK(arena != NULL);

    int *p = (int *)cb_arena_alloc(arena, sizeof(int));
    CHECK(p != NULL);
    *p = 42;

    CHECK(cb_arena_used(arena) >= sizeof(int));
    CHECK(cb_arena_capacity(arena) == 256);

    cb_arena_destroy(arena);
    printf("Arena: OK\n");
    return 0;
}

static int test_buffer(void) {
    CB_Buffer buf;
    CHECK(cb_buffer_init(&buf, 8) == CB_OK);
    CHECK(buf.size == 0);
    CHECK(buf.capacity == 8);

    CHECK(cb_buffer_append(&buf, "Hello", 5) == CB_OK);
    CHECK(buf.size == 5);
    CHECK(memcmp(buf.data, "Hello", 5) == 0);

    CHECK(cb_buffer_append_cstr(&buf, " World") == CB_OK);
    CHECK(buf.size == 11);

    cb_buffer_clear(&buf);
    CHECK(buf.size == 0);
    cb_buffer_free(&buf);
    printf("ByteBuffer: OK\n");
    return 0;
}

static int test_string_view(void) {
    CB_StringView sv = cb_sv_from_cstr("  test  ");
    sv = cb_sv_trim_ascii(sv);
    CHECK(cb_sv_equals(sv, cb_sv_from_cstr("test")));
    CHECK(cb_sv_starts_with(sv, cb_sv_from_cstr("te")));
    CHECK(!cb_sv_find(sv, cb_sv_from_cstr(""), NULL)); /* empty needle -> 0 */
    printf("StringView: OK\n");
    return 0;
}

static int test_vector(void) {
    CB_Vector vec;
    CHECK(cb_vector_init(&vec, sizeof(int), 4) == CB_OK);

    int v = 100;
    CHECK(cb_vector_push(&vec, &v) == CB_OK);
    int *ptr = (int *)cb_vector_get_ptr(&vec, 0);
    CHECK(ptr != NULL);
    CHECK(*ptr == 100);

    cb_vector_free(&vec);
    printf("Vector: OK\n");
    return 0;
}

static int test_hash(void) {
    uint64_t fnv = cb_hash_fnv1a64("hello", 5);
    CHECK(fnv == 11831194018420276491ULL);

    uint32_t crc = cb_crc32("123456789", 9);
    CHECK(crc == 0xCBF43926);

    printf("Hash (FNV + CRC32): OK\n");
    return 0;
}

static int test_file(void) {
    const char *tmp = "install_test_tmp.txt";
    CHECK(cb_file_write_all(tmp, "data", 4) == CB_OK);

    int exists = 0;
    CHECK(cb_file_exists(tmp, &exists) == CB_OK);
    CHECK(exists);

    uint64_t sz = 0;
    CHECK(cb_file_size(tmp, &sz) == CB_OK);
    CHECK(sz == 4);

    remove(tmp);
    printf("FileUtils: OK\n");
    return 0;
}

static int test_bitset(void) {
    CB_BitSet bs;
    CHECK(cb_bitset_init(&bs, 32) == CB_OK);

    CHECK(cb_bitset_set(&bs, 5) == CB_OK);
    int val = 0;
    CHECK(cb_bitset_get(&bs, 5, &val) == CB_OK);
    CHECK(val);
    CHECK(cb_bitset_get(&bs, 6, &val) == CB_OK);
    CHECK(!val);

    cb_bitset_free(&bs);
    printf("BitSet: OK\n");
    return 0;
}

static int test_ring(void) {
    CB_Ring ring;
    CHECK(cb_ring_init(&ring, sizeof(int), 4) == CB_OK);

    int v = 7;
    CHECK(cb_ring_push(&ring, &v) == CB_OK);
    int out = 0;
    CHECK(cb_ring_peek(&ring, &out) == CB_OK);
    CHECK(out == 7);
    CHECK(!cb_ring_is_empty(&ring));
    CHECK(!cb_ring_is_full(&ring));

    cb_ring_free(&ring);
    printf("RingBuffer: OK\n");
    return 0;
}

static int test_bloom(void) {
    CB_Bloom bloom;
    CHECK(cb_bloom_init(&bloom, 64, 3) == CB_OK);

    CHECK(cb_bloom_add(&bloom, "", 0) == CB_OK); /* empty key */
    CHECK(cb_bloom_add(&bloom, "key", 3) == CB_OK);

    int maybe = 0;
    CHECK(cb_bloom_might_contain(&bloom, "", 0, &maybe) == CB_OK);
    CHECK(maybe);
    CHECK(cb_bloom_might_contain(&bloom, "key", 3, &maybe) == CB_OK);
    CHECK(maybe);

    cb_bloom_free(&bloom);
    printf("Bloom Filter: OK\n");
    return 0;
}

static int test_string_map(void) {
    CB_StringMap *map = NULL;
    CHECK(cb_string_map_create(8, &map) == CB_OK);
    CHECK(cb_string_map_put(map, "a", "1") == CB_OK);

    const char *v = NULL;
    CHECK(cb_string_map_get(map, "a", &v) == CB_OK);
    CHECK(strcmp(v, "1") == 0);
    CHECK(cb_string_map_size(map) == 1);

    cb_string_map_destroy(map);
    printf("StringMap: OK\n");
    return 0;
}

int main(void) {
    printf("=== CoreBrick-C Install Test ===\n\n");

    int total = 0, passed = 0;

    /* Test each module, continue on failure to report as many as possible */
    struct { const char *name; int (*fn)(void); } tests[] = {
        { "Version",      test_version },
        { "Arena",        test_arena },
        { "ByteBuffer",   test_buffer },
        { "StringView",   test_string_view },
        { "Vector",       test_vector },
        { "Hash",         test_hash },
        { "FileUtils",    test_file },
        { "BitSet",       test_bitset },
        { "RingBuffer",   test_ring },
        { "Bloom Filter", test_bloom },
        { "StringMap",    test_string_map },
    };

    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        total++;
        printf("  [TEST] %s ... ", tests[i].name);
        fflush(stdout);
        if (tests[i].fn() == 0) {
            printf("  [PASS]\n");
            passed++;
        } else {
            printf("  [FAIL]\n");
        }
    }

    printf("\n--- Results: %d/%d passed ---\n", passed, total);
    return (passed == total) ? 0 : 1;
}