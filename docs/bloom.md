# Bloom Filter

## Overview

A space-efficient probabilistic data structure for testing set membership. 

## Important Notice

```
Bloom Filter has false positives.
Bloom Filter has no false negatives for inserted items unless misused.
Do not use Bloom Filter as an exact set.
```

## API Reference

### `cb_bloom_init`
```c
CB_Error cb_bloom_init(CB_Bloom *bloom, size_t bit_count, size_t hash_count);
```
Initializes a bloom filter. bit_count and hash_count must be > 0.

### `cb_bloom_add`
```c
CB_Error cb_bloom_add(CB_Bloom *bloom, const void *data, size_t len);
```
Adds an item to the filter.

### `cb_bloom_might_contain`
```c
CB_Error cb_bloom_might_contain(const CB_Bloom *bloom, const void *data, size_t len, int *out_maybe);
```
Returns 1 if the item might be in the set, 0 if definitely not.

### `cb_bloom_clear`
```c
CB_Error cb_bloom_clear(CB_Bloom *bloom);
```
Clears the filter, resetting all bits and inserted count.

### `cb_bloom_free`
```c
void cb_bloom_free(CB_Bloom *bloom);
```
Frees the internal bitset and resets fields.