#include "cb_bitset.h"
#include <stdlib.h>
#include <string.h>

#define BITS_PER_BYTE 8

static size_t bytes_needed(size_t bit_count) {
    return (bit_count + BITS_PER_BYTE - 1) / BITS_PER_BYTE;
}

CB_Error cb_bitset_init(CB_BitSet *bitset, size_t bit_count) {
    if (bitset == NULL) {
        return CB_ERR_INVALID_ARGUMENT;
    }
    if (bit_count == 0) {
        return CB_ERR_INVALID_ARGUMENT;
    }

    bitset->data = NULL;
    bitset->bit_count = 0;
    bitset->byte_count = 0;

    size_t byte_count = bytes_needed(bit_count);
    bitset->data = (uint8_t *)calloc(byte_count, 1);
    if (bitset->data == NULL) {
        return CB_ERR_OUT_OF_MEMORY;
    }

    bitset->bit_count = bit_count;
    bitset->byte_count = byte_count;
    return CB_OK;
}

CB_Error cb_bitset_set(CB_BitSet *bitset, size_t index) {
    if (bitset == NULL || bitset->data == NULL) {
        return CB_ERR_INVALID_ARGUMENT;
    }
    if (index >= bitset->bit_count) {
        return CB_ERR_INDEX_OUT_OF_BOUNDS;
    }

    bitset->data[index / BITS_PER_BYTE] |= (uint8_t)(1U << (index % BITS_PER_BYTE));
    return CB_OK;
}

CB_Error cb_bitset_clear_bit(CB_BitSet *bitset, size_t index) {
    if (bitset == NULL || bitset->data == NULL) {
        return CB_ERR_INVALID_ARGUMENT;
    }
    if (index >= bitset->bit_count) {
        return CB_ERR_INDEX_OUT_OF_BOUNDS;
    }

    bitset->data[index / BITS_PER_BYTE] &= (uint8_t)~(1U << (index % BITS_PER_BYTE));
    return CB_OK;
}

CB_Error cb_bitset_get(const CB_BitSet *bitset, size_t index, int *out_value) {
    if (bitset == NULL || bitset->data == NULL || out_value == NULL) {
        return CB_ERR_INVALID_ARGUMENT;
    }
    if (index >= bitset->bit_count) {
        return CB_ERR_INDEX_OUT_OF_BOUNDS;
    }

    *out_value = (bitset->data[index / BITS_PER_BYTE] >> (index % BITS_PER_BYTE)) & 1;
    return CB_OK;
}

CB_Error cb_bitset_fill(CB_BitSet *bitset) {
    if (bitset == NULL || bitset->data == NULL) {
        return CB_ERR_INVALID_ARGUMENT;
    }

    memset(bitset->data, 0xFF, bitset->byte_count);

    /* Clear any bits beyond bit_count in the last byte */
    size_t remainder = bitset->bit_count % BITS_PER_BYTE;
    if (remainder != 0) {
        uint8_t mask = (uint8_t)((1U << remainder) - 1);
        bitset->data[bitset->byte_count - 1] &= mask;
    }

    return CB_OK;
}

CB_Error cb_bitset_clear_all(CB_BitSet *bitset) {
    if (bitset == NULL || bitset->data == NULL) {
        return CB_ERR_INVALID_ARGUMENT;
    }

    memset(bitset->data, 0, bitset->byte_count);
    return CB_OK;
}

CB_Error cb_bitset_count_ones(const CB_BitSet *bitset, size_t *out_count) {
    if (bitset == NULL || bitset->data == NULL || out_count == NULL) {
        return CB_ERR_INVALID_ARGUMENT;
    }

    size_t count = 0;
    for (size_t i = 0; i < bitset->byte_count; i++) {
        uint8_t byte = bitset->data[i];
        while (byte) {
            count += (byte & 1);
            byte >>= 1;
        }
    }

    *out_count = count;
    return CB_OK;
}

void cb_bitset_free(CB_BitSet *bitset) {
    if (bitset != NULL) {
        free(bitset->data);
        bitset->data = NULL;
        bitset->bit_count = 0;
        bitset->byte_count = 0;
    }
}