#ifndef CB_BLOOM_H
#define CB_BLOOM_H

#include <stddef.h>
#include <stdint.h>
#include "cb_error.h"
#include "cb_bitset.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    CB_BitSet bits;
    size_t bit_count;
    size_t hash_count;
    size_t inserted_count;
} CB_Bloom;

CB_Error cb_bloom_init(CB_Bloom *bloom, size_t bit_count, size_t hash_count);
CB_Error cb_bloom_add(CB_Bloom *bloom, const void *data, size_t len);
CB_Error cb_bloom_might_contain(const CB_Bloom *bloom, const void *data, size_t len, int *out_maybe);
CB_Error cb_bloom_clear(CB_Bloom *bloom);
void cb_bloom_free(CB_Bloom *bloom);

#ifdef __cplusplus
}
#endif

#endif /* CB_BLOOM_H */