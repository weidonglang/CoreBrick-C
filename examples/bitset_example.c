#include <stdio.h>
#include "cb_bitset.h"

int main(void) {
    printf("=== BitSet Example ===\n");

    CB_BitSet bs;
    cb_bitset_init(&bs, 64);

    cb_bitset_set(&bs, 0);
    cb_bitset_set(&bs, 10);
    cb_bitset_set(&bs, 63);

    int val;
    cb_bitset_get(&bs, 0, &val);  printf("Bit 0: %d\n", val);
    cb_bitset_get(&bs, 10, &val); printf("Bit 10: %d\n", val);
    cb_bitset_get(&bs, 63, &val); printf("Bit 63: %d\n", val);

    size_t count;
    cb_bitset_count_ones(&bs, &count);
    printf("Ones: %zu\n", count);

    cb_bitset_free(&bs);
    return 0;
}