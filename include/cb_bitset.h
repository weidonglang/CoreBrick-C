#ifndef CB_BITSET_H
#define CB_BITSET_H

#include <stddef.h>
#include <stdint.h>
#include "cb_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t *data;
    size_t bit_count;
    size_t byte_count;
} CB_BitSet;

CB_Error cb_bitset_init(CB_BitSet *bitset, size_t bit_count);
CB_Error cb_bitset_set(CB_BitSet *bitset, size_t index);
CB_Error cb_bitset_clear_bit(CB_BitSet *bitset, size_t index);
CB_Error cb_bitset_get(const CB_BitSet *bitset, size_t index, int *out_value);
CB_Error cb_bitset_fill(CB_BitSet *bitset);
CB_Error cb_bitset_clear_all(CB_BitSet *bitset);
CB_Error cb_bitset_count_ones(const CB_BitSet *bitset, size_t *out_count);
void cb_bitset_free(CB_BitSet *bitset);

#ifdef __cplusplus
}
#endif

#endif /* CB_BITSET_H */