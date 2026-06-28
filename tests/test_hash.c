#include <stdio.h>
#include <string.h>
#include "cb_hash.h"

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

void test_hash(void) {
    printf("\n--- Hash Module ---\n");

    /* FNV-1a 64 empty input */
    uint64_t h64_empty = cb_hash_fnv1a64("", 0);
    TEST("fnv1a64 empty returns offset basis", h64_empty == 14695981039346656037ULL);

    /* FNV-1a 32 empty input */
    uint32_t h32_empty = cb_hash_fnv1a32("", 0);
    TEST("fnv1a32 empty returns offset basis", h32_empty == 2166136261U);

    /* FNV-1a same input same output */
    uint64_t h64_a = cb_hash_fnv1a64("hello", 5);
    uint64_t h64_b = cb_hash_fnv1a64("hello", 5);
    TEST("fnv1a64 same input same output", h64_a == h64_b);

    uint32_t h32_a = cb_hash_fnv1a32("hello", 5);
    uint32_t h32_b = cb_hash_fnv1a32("hello", 5);
    TEST("fnv1a32 same input same output", h32_a == h32_b);

    /* FNV-1a different input different output */
    uint64_t h64_c = cb_hash_fnv1a64("world", 5);
    TEST("fnv1a64 different input different output", h64_a != h64_c);

    /* FNV-1a consistency checks */
    /* Same input always gives same output (already tested above) */
    /* Different input gives different output */
    uint32_t h32_abc = cb_hash_fnv1a32("abc", 3);
    uint32_t h32_abd = cb_hash_fnv1a32("abd", 3);
    TEST("fnv1a32 'abc' != 'abd'", h32_abc != h32_abd);

    uint64_t h64_abc = cb_hash_fnv1a64("abc", 3);
    uint64_t h64_abd = cb_hash_fnv1a64("abd", 3);
    TEST("fnv1a64 'abc' != 'abd'", h64_abc != h64_abd);

    /* Verify non-zero */
    TEST("fnv1a32 'abc' non-zero", h32_abc != 0);
    TEST("fnv1a64 'abc' non-zero", h64_abc != 0);
    TEST("fnv1a32 'abc' != offset basis", h32_abc != 2166136261U);
    TEST("fnv1a64 'abc' != offset basis", h64_abc != 14695981039346656037ULL);

    /* CRC32 "123456789" == 0xCBF43926 */
    uint32_t crc = cb_crc32("123456789", 9);
    TEST("crc32 '123456789' == 0xCBF43926", crc == 0xCBF43926U);

    /* CRC32 empty input */
    uint32_t crc_empty = cb_crc32("", 0);
    TEST("crc32 empty returns 0", crc_empty == 0);

    /* CRC32 same input same output */
    uint32_t crc_a = cb_crc32("hello", 5);
    uint32_t crc_b = cb_crc32("hello", 5);
    TEST("crc32 same input same output", crc_a == crc_b);

    /* CRC32 different input */
    uint32_t crc_c = cb_crc32("world", 5);
    TEST("crc32 different input different output", crc_a != crc_c);

    /* NULL len=0 safe */
    uint64_t h64_null = cb_hash_fnv1a64(NULL, 0);
    TEST("fnv1a64 NULL len=0 returns offset basis", h64_null == 14695981039346656037ULL);

    uint32_t h32_null = cb_hash_fnv1a32(NULL, 0);
    TEST("fnv1a32 NULL len=0 returns offset basis", h32_null == 2166136261U);

    uint32_t crc_null = cb_crc32(NULL, 0);
    TEST("crc32 NULL len=0 returns 0", crc_null == 0);

    /* NULL len>0 safe */
    uint64_t h64_null_len = cb_hash_fnv1a64(NULL, 10);
    TEST("fnv1a64 NULL len>0 returns offset basis (safe)", h64_null_len == 14695981039346656037ULL);

    uint32_t h32_null_len = cb_hash_fnv1a32(NULL, 10);
    TEST("fnv1a32 NULL len>0 returns offset basis (safe)", h32_null_len == 2166136261U);

    uint32_t crc_null_len = cb_crc32(NULL, 10);
    TEST("crc32 NULL len>0 returns 0 (safe)", crc_null_len == 0);

    /* large buffer */
    char large[1024];
    memset(large, 'A', 1024);
    uint64_t h64_large = cb_hash_fnv1a64(large, 1024);
    TEST("fnv1a64 large buffer works", h64_large != 14695981039346656037ULL);
    uint32_t crc_large = cb_crc32(large, 1024);
    TEST("crc32 large buffer works", crc_large != 0);
}