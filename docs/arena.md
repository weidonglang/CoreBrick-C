# Arena Allocator

## Overview

The Arena allocator provides fast, scoped memory allocations. Memory is allocated from a pre-allocated buffer using a bump pointer. The entire arena can be reset at once, making it ideal for temporary allocations within a scope.

## API Reference

### `cb_arena_create`
```c
CB_Error cb_arena_create(size_t capacity, CB_Arena **out_arena);
```
Creates an arena with the given capacity. Returns `CB_ERR_INVALID_ARGUMENT` if `capacity == 0` or `out_arena == NULL`.

### `cb_arena_alloc`
```c
void *cb_arena_alloc(CB_Arena *arena, size_t size);
```
Allocates `size` bytes from the arena. Returns NULL if out of memory or arena is NULL.

### `cb_arena_alloc_zero`
```c
void *cb_arena_alloc_zero(CB_Arena *arena, size_t size);
```
Allocates and zero-initializes memory.

### `cb_arena_reset`
```c
CB_Error cb_arena_reset(CB_Arena *arena);
```
Resets the arena, making all memory available for reuse.

### `cb_arena_used` / `cb_arena_capacity`
```c
size_t cb_arena_used(const CB_Arena *arena);
size_t cb_arena_capacity(const CB_Arena *arena);
```
Returns current usage and total capacity.

### `cb_arena_destroy`
```c
void cb_arena_destroy(CB_Arena *arena);
```
Frees all memory. Safe to call with NULL.