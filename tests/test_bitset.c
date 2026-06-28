#include <stdio.h>
#include <string.h>
#include "cb_bitset.h"

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

void test_bitset(void) {
    printf("\n--- BitSet Module ---\n");

    /* init success */
    CB_BitSet bs;
    memset(&bs, 0, sizeof(bs));
    TEST("init success", cb_bitset_init(&bs, 100) == CB_OK);
    TEST("init bit_count=100", bs.bit_count == 100);
    TEST("init byte_count=13", bs.byte_count == 13);
    TEST("init data non-null", bs.data != NULL);

    /* init zero bit_count failure */
    CB_BitSet bs0;
    TEST("init zero bit_count failure", cb_bitset_init(&bs0, 0) == CB_ERR_INVALID_ARGUMENT);

    /* init NULL failure */
    TEST("init NULL failure", cb_bitset_init(NULL, 100) == CB_ERR_INVALID_ARGUMENT);

    /* all bits initially zero */
    int val;
    cb_bitset_get(&bs, 0, &val);
    TEST("bit 0 initially 0", val == 0);
    cb_bitset_get(&bs, 50, &val);
    TEST("bit 50 initially 0", val == 0);
    cb_bitset_get(&bs, 99, &val);
    TEST("bit 99 initially 0", val == 0);

    /* set first bit */
    TEST("set bit 0", cb_bitset_set(&bs, 0) == CB_OK);
    cb_bitset_get(&bs, 0, &val);
    TEST("bit 0 is 1 after set", val == 1);

    /* set middle bit */
    TEST("set bit 50", cb_bitset_set(&bs, 50) == CB_OK);
    cb_bitset_get(&bs, 50, &val);
    TEST("bit 50 is 1 after set", val == 1);

    /* set last bit */
    TEST("set bit 99", cb_bitset_set(&bs, 99) == CB_OK);
    cb_bitset_get(&bs, 99, &val);
    TEST("bit 99 is 1 after set", val == 1);

    /* get out of bounds failure */
    TEST("get out of bounds", cb_bitset_get(&bs, 100, &val) == CB_ERR_INDEX_OUT_OF_BOUNDS);

    /* clear_bit success */
    TEST("clear bit 0", cb_bitset_clear_bit(&bs, 0) == CB_OK);
    cb_bitset_get(&bs, 0, &val);
    TEST("bit 0 is 0 after clear", val == 0);

    /* clear_bit out of bounds */
    TEST("clear_bit out of bounds", cb_bitset_clear_bit(&bs, 100) == CB_ERR_INDEX_OUT_OF_BOUNDS);

    /* clear_all */
    cb_bitset_set(&bs, 10);
    TEST("clear_all success", cb_bitset_clear_all(&bs) == CB_OK);
    cb_bitset_get(&bs, 10, &val);
    TEST("bit 10 is 0 after clear_all", val == 0);

    /* fill */
    TEST("fill success", cb_bitset_fill(&bs) == CB_OK);
    cb_bitset_get(&bs, 0, &val);
    TEST("bit 0 is 1 after fill", val == 1);
    cb_bitset_get(&bs, 99, &val);
    TEST("bit 99 is 1 after fill", val == 1);

    /* count_ones empty after init */
    CB_BitSet bs2;
    cb_bitset_init(&bs2, 64);
    cb_bitset_clear_all(&bs2);  /* ensure cleared */
    size_t count;
    cb_bitset_count_ones(&bs2, &count);
    TEST("count_ones after clear_all is 0", count == 0);

    /* count_ones after several set */
    cb_bitset_set(&bs2, 0);
    cb_bitset_set(&bs2, 10);
    cb_bitset_set(&bs2, 63);
    cb_bitset_count_ones(&bs2, &count);
    TEST("count_ones after 3 sets is 3", count == 3);

    /* count_ones after fill */
    cb_bitset_fill(&bs2);
    cb_bitset_count_ones(&bs2, &count);
    TEST("count_ones after fill of 64 bits is 64", count == 64);

    /* fill with non-aligned bit count */
    CB_BitSet bs3;
    cb_bitset_init(&bs3, 10);
    cb_bitset_fill(&bs3);
    cb_bitset_count_ones(&bs3, &count);
    TEST("fill 10 bits count=10", count == 10);

    /* free resets fields */
    cb_bitset_free(&bs3);
    TEST("free data=NULL", bs3.data == NULL);
    TEST("free bit_count=0", bs3.bit_count == 0);
    TEST("free byte_count=0", bs3.byte_count == 0);

    /* double free safe */
    cb_bitset_free(&bs3);
    TEST("double free safe", 1);

    /* large bitset */
    CB_BitSet large;
    cb_bitset_init(&large, 100000);
    TEST("large bitset init", large.data != NULL);
    for (size_t i = 0; i < 100000; i += 1000) {
        cb_bitset_set(&large, i);
    }
    cb_bitset_count_ones(&large, &count);
    TEST("large bitset count=100", count == 100);
    cb_bitset_free(&large);

    /* NULL safety */
    TEST("set NULL", cb_bitset_set(NULL, 0) == CB_ERR_INVALID_ARGUMENT);
    TEST("get NULL", cb_bitset_get(NULL, 0, &val) == CB_ERR_INVALID_ARGUMENT);
    TEST("clear_bit NULL", cb_bitset_clear_bit(NULL, 0) == CB_ERR_INVALID_ARGUMENT);
    TEST("fill NULL", cb_bitset_fill(NULL) == CB_ERR_INVALID_ARGUMENT);
    TEST("clear_all NULL", cb_bitset_clear_all(NULL) == CB_ERR_INVALID_ARGUMENT);
    TEST("count_ones NULL", cb_bitset_count_ones(NULL, &count) == CB_ERR_INVALID_ARGUMENT);
    cb_bitset_free(NULL);
    TEST("free NULL safe", 1);

    cb_bitset_free(&bs);
    cb_bitset_free(&bs2);
}