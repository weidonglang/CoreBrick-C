# BitSet

## Overview

A fixed-size bit array for compact boolean state tracking. Each bit can be set, cleared, and tested independently.

## API Reference

### `cb_bitset_init`
```c
CB_Error cb_bitset_init(CB_BitSet *bitset, size_t bit_count);
```
Initializes a bitset with `bit_count` bits (all zero). Returns `CB_ERR_INVALID_ARGUMENT` if bit_count is 0.

### `cb_bitset_set`
```c
CB_Error cb_bitset_set(CB_BitSet *bitset, size_t index);
```
Sets the bit at `index` to 1.

### `cb_bitset_clear_bit`
```c
CB_Error cb_bitset_clear_bit(CB_BitSet *bitset, size_t index);
```
Sets the bit at `index` to 0.

### `cb_bitset_get`
```c
CB_Error cb_bitset_get(const CB_BitSet *bitset, size_t index, int *out_value);
```
Returns the value of the bit at `index` (0 or 1).

### `cb_bitset_fill`
```c
CB_Error cb_bitset_fill(CB_BitSet *bitset);
```
Sets all bits to 1. Bits beyond the bit_count in the last byte are not set.

### `cb_bitset_clear_all`
```c
CB_Error cb_bitset_clear_all(CB_BitSet *bitset);
```
Sets all bits to 0.

### `cb_bitset_count_ones`
```c
CB_Error cb_bitset_count_ones(const CB_BitSet *bitset, size_t *out_count);
```
Counts the number of bits set to 1.

### `cb_bitset_free`
```c
void cb_bitset_free(CB_BitSet *bitset);
```
Frees all memory and resets fields.