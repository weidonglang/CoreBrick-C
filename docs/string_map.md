# StringMap

## Overview

A hash table mapping string keys to string values. Uses open addressing with linear probing and tombstone-based deletion.

## API Reference

### `cb_string_map_create`
```c
CB_Error cb_string_map_create(size_t initial_capacity, CB_StringMap **out_map);
```
Creates a string map. Uses default capacity 16 if `initial_capacity` is 0.

### `cb_string_map_put`
```c
CB_Error cb_string_map_put(CB_StringMap *map, const char *key, const char *value);
```
Inserts or overwrites a key-value pair. Keys and values are copied internally. NULL values are stored as empty strings.

### `cb_string_map_get`
```c
CB_Error cb_string_map_get(const CB_StringMap *map, const char *key, const char **out_value);
```
Returns the value for a key. Sets `*out_value` to NULL if the key is not found (still returns CB_OK).

### `cb_string_map_contains`
```c
CB_Error cb_string_map_contains(const CB_StringMap *map, const char *key, int *out_contains);
```
Checks if a key exists.

### `cb_string_map_remove`
```c
CB_Error cb_string_map_remove(CB_StringMap *map, const char *key);
```
Removes a key-value pair. Safe to call with a non-existent key.

### `cb_string_map_size`
```c
size_t cb_string_map_size(const CB_StringMap *map);
```
Returns the number of entries.

### `cb_string_map_clear`
```c
void cb_string_map_clear(CB_StringMap *map);
```
Removes all entries without deallocating the map.

### `cb_string_map_destroy`
```c
void cb_string_map_destroy(CB_StringMap *map);
```
Frees all memory. Safe to call with NULL.

## Implementation Details

- Open addressing with linear probing
- Tombstone markers for deletion
- Automatic rehashing when load factor exceeds 0.7
- FNV-1a 32-bit hash function
- Keys and values are internally duplicated