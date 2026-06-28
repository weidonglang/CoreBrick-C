# CoreBrick-C API Reference

## Modules

- [Error Codes](./cb_error.h) - CB_Error enum and error string conversion
- [Arena Allocator](./arena.md) - Linear memory allocator
- [ByteBuffer](./buffer.md) - Dynamic byte buffer
- [StringView](./string_view.md) - Zero-copy string slice
- [Vector](./vector.md) - Generic dynamic array
- [Hash](./hash.md) - Non-cryptographic hash utilities
- [File](./file.md) - File utilities
- [Timer](./timer.md) - High-resolution timer
- [BitSet](./bitset.md) - Fixed-size bit array
- [RingBuffer](./ring.md) - Fixed-capacity FIFO ring buffer

- [Bloom Filter](./bloom.md) - Probabilistic set membership filter
- [StringMap](./string_map.md) - String-to-string hash table
- [Version Header](./corebrick_version.h) - Version macros
- [API Policy](./api_policy.md) - API/ABI stability policy
## Error Codes

| Code | Description |
|---|---|
| `CB_OK` | Success |
| `CB_ERR_INVALID_ARGUMENT` | Invalid argument |
| `CB_ERR_OUT_OF_MEMORY` | Memory allocation failure |
| `CB_ERR_BUFFER_TOO_SMALL` | Buffer capacity insufficient |
| `CB_ERR_INDEX_OUT_OF_BOUNDS` | Index out of bounds |
| `CB_ERR_PARSE` | Parse error |
| `CB_ERR_IO` | I/O error |
| `CB_ERR_INTERNAL` | Internal error |

## Full API

```c
// Error
const char *cb_error_string(CB_Error err);

// Arena
CB_Error   cb_arena_create(size_t capacity, CB_Arena **out_arena);
void      *cb_arena_alloc(CB_Arena *arena, size_t size);
void      *cb_arena_alloc_zero(CB_Arena *arena, size_t size);
CB_Error   cb_arena_reset(CB_Arena *arena);
size_t     cb_arena_used(const CB_Arena *arena);
size_t     cb_arena_capacity(const CB_Arena *arena);
void       cb_arena_destroy(CB_Arena *arena);

// Buffer
CB_Error   cb_buffer_init(CB_Buffer *buffer, size_t initial_capacity);
CB_Error   cb_buffer_reserve(CB_Buffer *buffer, size_t new_capacity);
CB_Error   cb_buffer_append(CB_Buffer *buffer, const void *data, size_t len);
CB_Error   cb_buffer_append_u8(CB_Buffer *buffer, uint8_t value);
CB_Error   cb_buffer_append_u32_le(CB_Buffer *buffer, uint32_t value);
CB_Error   cb_buffer_append_cstr(CB_Buffer *buffer, const char *text);
void       cb_buffer_clear(CB_Buffer *buffer);
void       cb_buffer_free(CB_Buffer *buffer);

// StringView
CB_StringView cb_sv_from_parts(const char *data, size_t len);
CB_StringView cb_sv_from_cstr(const char *text);
int           cb_sv_is_empty(CB_StringView sv);
CB_StringView cb_sv_trim_ascii(CB_StringView sv);
int           cb_sv_starts_with(CB_StringView sv, CB_StringView prefix);
int           cb_sv_ends_with(CB_StringView sv, CB_StringView suffix);
int           cb_sv_equals(CB_StringView a, CB_StringView b);
int           cb_sv_find(CB_StringView sv, CB_StringView needle, size_t *out_pos);
CB_Error      cb_sv_to_i64(CB_StringView sv, int64_t *out_value);
CB_Error      cb_sv_to_f64(CB_StringView sv, double *out_value);
CB_SplitIter  cb_sv_split(CB_StringView input, char delimiter);
int           cb_sv_split_next(CB_SplitIter *iter, CB_StringView *out_part);

// Vector
CB_Error   cb_vector_init(CB_Vector *vec, size_t element_size, size_t initial_capacity);
CB_Error   cb_vector_push(CB_Vector *vec, const void *element);
CB_Error   cb_vector_get(const CB_Vector *vec, size_t index, void *out_element);
void      *cb_vector_get_ptr(CB_Vector *vec, size_t index);
CB_Error   cb_vector_set(CB_Vector *vec, size_t index, const void *element);
CB_Error   cb_vector_pop(CB_Vector *vec, void *out_element);
void       cb_vector_clear(CB_Vector *vec);
void       cb_vector_free(CB_Vector *vec);
```
// Hash
uint64_t   cb_hash_fnv1a64(const void *data, size_t len);
uint32_t   cb_hash_fnv1a32(const void *data, size_t len);
uint32_t   cb_crc32(const void *data, size_t len);

// File
CB_Error   cb_file_exists(const char *path, int *out_exists);
CB_Error   cb_file_size(const char *path, uint64_t *out_size);
CB_Error   cb_file_read_all(const char *path, CB_Buffer *out_buffer);
CB_Error   cb_file_write_all(const char *path, const void *data, size_t len);

// BitSet
CB_Error   cb_bitset_init(CB_BitSet *bitset, size_t bit_count);
CB_Error   cb_bitset_set(CB_BitSet *bitset, size_t index);
CB_Error   cb_bitset_clear_bit(CB_BitSet *bitset, size_t index);
CB_Error   cb_bitset_get(const CB_BitSet *bitset, size_t index, int *out_value);
CB_Error   cb_bitset_fill(CB_BitSet *bitset);
CB_Error   cb_bitset_clear_all(CB_BitSet *bitset);
CB_Error   cb_bitset_count_ones(const CB_BitSet *bitset, size_t *out_count);
void       cb_bitset_free(CB_BitSet *bitset);

// RingBuffer
CB_Error   cb_ring_init(CB_Ring *ring, size_t element_size, size_t capacity);
CB_Error   cb_ring_push(CB_Ring *ring, const void *element);
CB_Error   cb_ring_pop(CB_Ring *ring, void *out_element);
CB_Error   cb_ring_peek(const CB_Ring *ring, void *out_element);
int        cb_ring_is_empty(const CB_Ring *ring);
int        cb_ring_is_full(const CB_Ring *ring);
void       cb_ring_clear(CB_Ring *ring);
void       cb_ring_free(CB_Ring *ring);

// Bloom Filter
CB_Error   cb_bloom_init(CB_Bloom *bloom, size_t bit_count, size_t hash_count);
CB_Error   cb_bloom_add(CB_Bloom *bloom, const void *data, size_t len);
CB_Error   cb_bloom_might_contain(const CB_Bloom *bloom, const void *data, size_t len, int *out_maybe);
CB_Error   cb_bloom_clear(CB_Bloom *bloom);
void       cb_bloom_free(CB_Bloom *bloom);

// StringMap
CB_Error   cb_string_map_create(size_t initial_capacity, CB_StringMap **out_map);
CB_Error   cb_string_map_put(CB_StringMap *map, const char *key, const char *value);
CB_Error   cb_string_map_get(const CB_StringMap *map, const char *key, const char **out_value);
CB_Error   cb_string_map_contains(const CB_StringMap *map, const char *key, int *out_contains);
CB_Error   cb_string_map_remove(CB_StringMap *map, const char *key);
size_t     cb_string_map_size(const CB_StringMap *map);
void       cb_string_map_clear(CB_StringMap *map);
void       cb_string_map_destroy(CB_StringMap *map);
```