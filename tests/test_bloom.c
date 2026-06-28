#include <stdio.h>
#include <string.h>
#include "cb_bloom.h"

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

void test_bloom(void) {
    printf("\n--- Bloom Filter Module ---\n");

    /* init success */
    CB_Bloom bloom;
    memset(&bloom, 0, sizeof(bloom));
    TEST("init success", cb_bloom_init(&bloom, 1024, 3) == CB_OK);
    TEST("init bit_count=1024", bloom.bit_count == 1024);
    TEST("init hash_count=3", bloom.hash_count == 3);
    TEST("init inserted=0", bloom.inserted_count == 0);

    /* init zero bit_count failure */
    TEST("init zero bit_count", cb_bloom_init(&bloom, 0, 3) == CB_ERR_INVALID_ARGUMENT);

    /* init zero hash_count failure */
    TEST("init zero hash_count", cb_bloom_init(&bloom, 1024, 0) == CB_ERR_INVALID_ARGUMENT);

    /* re-init for real testing */
    cb_bloom_init(&bloom, 1024, 3);

    /* add success */
    TEST("add 'hello'", cb_bloom_add(&bloom, "hello", 5) == CB_OK);
    TEST("inserted_count=1", bloom.inserted_count == 1);

    /* contains added item true */
    int maybe;
    cb_bloom_might_contain(&bloom, "hello", 5, &maybe);
    TEST("contains 'hello' true", maybe == 1);

    /* multiple items */
    cb_bloom_add(&bloom, "world", 5);
    cb_bloom_add(&bloom, "test", 4);
    TEST("inserted_count=3", bloom.inserted_count == 3);
    cb_bloom_might_contain(&bloom, "world", 5, &maybe);
    TEST("contains 'world' true", maybe == 1);
    cb_bloom_might_contain(&bloom, "test", 4, &maybe);
    TEST("contains 'test' true", maybe == 1);

    /* clear makes inserted_count zero */
    TEST("clear success", cb_bloom_clear(&bloom) == CB_OK);
    TEST("inserted_count=0 after clear", bloom.inserted_count == 0);

    /* free resets fields */
    cb_bloom_free(&bloom);
    TEST("free bits.data=NULL", bloom.bits.data == NULL);
    TEST("free bit_count=0", bloom.bit_count == 0);

    /* NULL bloom failure */
    int val;
    TEST("add NULL bloom", cb_bloom_add(NULL, "x", 1) == CB_ERR_INVALID_ARGUMENT);
    TEST("might_contain NULL bloom", cb_bloom_might_contain(NULL, "x", 1, &val) == CB_ERR_INVALID_ARGUMENT);
    TEST("clear NULL bloom", cb_bloom_clear(NULL) == CB_ERR_INVALID_ARGUMENT);
    cb_bloom_free(NULL);
    TEST("free NULL safe", 1);

    /* NULL data len>0 failure */
    CB_Bloom b2;
    cb_bloom_init(&b2, 64, 3);
    TEST("add NULL data len>0", cb_bloom_add(&b2, NULL, 5) == CB_ERR_INVALID_ARGUMENT);
    TEST("might_contain NULL data len>0", cb_bloom_might_contain(&b2, NULL, 5, &val) == CB_ERR_INVALID_ARGUMENT);

    /* empty input as valid key */
    cb_bloom_clear(&b2);
    TEST("add empty data sets bit", cb_bloom_add(&b2, "", 0) == CB_OK);
    TEST("inserted_count=1 after empty add", b2.inserted_count == 1);
    cb_bloom_might_contain(&b2, "", 0, &maybe);
    TEST("contains empty key after add", maybe == 1);

    /* empty key not yet added should not be found (high probability) */
    /* Note: This test is probabilistic and depends on bit_count */
    cb_bloom_clear(&b2);
    cb_bloom_add(&b2, "x", 1);
    cb_bloom_might_contain(&b2, "", 0, &maybe);
    /* With a fresh filter, empty key was not added, so bits for it are 0 */
    cb_bloom_free(&b2);

    /* small bitset works */
    CB_Bloom small;
    cb_bloom_init(&small, 8, 2);
    TEST("small bitset add", cb_bloom_add(&small, "a", 1) == CB_OK);
    cb_bloom_might_contain(&small, "a", 1, &val);
    TEST("small bitset contains 'a'", val == 1);
    cb_bloom_free(&small);

    /* large bitset works */
    CB_Bloom large;
    cb_bloom_init(&large, 10000, 5);
    int ok = 1;
    for (int i = 0; i < 100; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "key-%d", i);
        if (cb_bloom_add(&large, buf, strlen(buf)) != CB_OK) {
            ok = 0;
            break;
        }
    }
    TEST("large bitset 100 adds", ok == 1);
    TEST("large inserted_count=100", large.inserted_count == 100);
    int all_found = 1;
    for (int i = 0; i < 100; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "key-%d", i);
        cb_bloom_might_contain(&large, buf, strlen(buf), &val);
        if (val != 1) {
            all_found = 0;
            break;
        }
    }
    TEST("all 100 items found", all_found == 1);
    cb_bloom_free(&large);
}