#include "cb_bloom.h"
#include "cb_hash.h"
#include <string.h>

#define DOUBLE_HASH_CONSTANT 0xABCDABCDU

CB_Error cb_bloom_init(CB_Bloom *bloom, size_t bit_count, size_t hash_count) {
    if (bloom == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (bit_count == 0 || hash_count == 0) return CB_ERR_INVALID_ARGUMENT;
    CB_Error err = cb_bitset_init(&bloom->bits, bit_count);
    if (err != CB_OK) return err;
    bloom->bit_count = bit_count;
    bloom->hash_count = hash_count;
    bloom->inserted_count = 0;
    return CB_OK;
}

CB_Error cb_bloom_add(CB_Bloom *bloom, const void *data, size_t len) {
    if (bloom == NULL || bloom->bits.data == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (data == NULL && len > 0) return CB_ERR_INVALID_ARGUMENT;
    uint64_t h1 = cb_hash_fnv1a64(data, len);
    uint32_t h2 = cb_crc32(data, len);
    if (h2 == 0) h2 = DOUBLE_HASH_CONSTANT;
    for (size_t i = 0; i < bloom->hash_count; i++) {
        size_t index = (size_t)((h1 + (uint64_t)i * h2) % bloom->bit_count);
        cb_bitset_set(&bloom->bits, index);
    }
    bloom->inserted_count++;
    return CB_OK;
}

CB_Error cb_bloom_might_contain(const CB_Bloom *bloom, const void *data, size_t len, int *out_maybe) {
    if (bloom == NULL || bloom->bits.data == NULL || out_maybe == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (data == NULL && len > 0) return CB_ERR_INVALID_ARGUMENT;
    uint64_t h1 = cb_hash_fnv1a64(data, len);
    uint32_t h2 = cb_crc32(data, len);
    if (h2 == 0) h2 = DOUBLE_HASH_CONSTANT;
    for (size_t i = 0; i < bloom->hash_count; i++) {
        size_t index = (size_t)((h1 + (uint64_t)i * h2) % bloom->bit_count);
        int bit_val;
        cb_bitset_get(&bloom->bits, index, &bit_val);
        if (bit_val == 0) {
            *out_maybe = 0;
            return CB_OK;
        }
    }
    *out_maybe = 1;
    return CB_OK;
}

CB_Error cb_bloom_clear(CB_Bloom *bloom) {
    if (bloom == NULL || bloom->bits.data == NULL) return CB_ERR_INVALID_ARGUMENT;
    CB_Error err = cb_bitset_clear_all(&bloom->bits);
    if (err != CB_OK) return err;
    bloom->inserted_count = 0;
    return CB_OK;
}

void cb_bloom_free(CB_Bloom *bloom) {
    if (bloom != NULL) {
        cb_bitset_free(&bloom->bits);
        bloom->bit_count = 0;
        bloom->hash_count = 0;
        bloom->inserted_count = 0;
    }
}