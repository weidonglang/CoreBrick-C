#include "cb_arena.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Default alignment: max_align_t on most platforms is 8 or 16 */
#define ARENA_DEFAULT_ALIGNMENT (sizeof(void *) > 4 ? 16 : 8)

static size_t align_up_size(size_t value, size_t alignment) {
    if (alignment == 0) return value;
    size_t mask = alignment - 1;
    if (value > SIZE_MAX - mask) return SIZE_MAX; /* overflow */
    return (value + mask) & ~mask;
}

struct CB_Arena {
    unsigned char *buffer;
    size_t capacity;
    size_t used;
};

CB_Error cb_arena_create(size_t capacity, CB_Arena **out_arena) {
    if (capacity == 0) {
        return CB_ERR_INVALID_ARGUMENT;
    }
    if (out_arena == NULL) {
        return CB_ERR_INVALID_ARGUMENT;
    }

    CB_Arena *arena = (CB_Arena *)malloc(sizeof(CB_Arena));
    if (arena == NULL) {
        return CB_ERR_OUT_OF_MEMORY;
    }

    arena->buffer = (unsigned char *)malloc(capacity);
    if (arena->buffer == NULL) {
        free(arena);
        return CB_ERR_OUT_OF_MEMORY;
    }

    arena->capacity = capacity;
    arena->used = 0;
    *out_arena = arena;

    return CB_OK;
}

void *cb_arena_alloc(CB_Arena *arena, size_t size) {
    if (arena == NULL) {
        return NULL;
    }
    if (size == 0) {
        return NULL;
    }

    /* Align current offset up to default alignment */
    size_t aligned_used = align_up_size(arena->used, ARENA_DEFAULT_ALIGNMENT);

    /* Check overflow: size > capacity - aligned_used */
    if (size > arena->capacity - aligned_used) {
        return NULL;
    }

    void *ptr = arena->buffer + aligned_used;
    arena->used = aligned_used + size;
    return ptr;
}

void *cb_arena_alloc_zero(CB_Arena *arena, size_t size) {
    void *ptr = cb_arena_alloc(arena, size);
    if (ptr != NULL) {
        memset(ptr, 0, size);
    }
    return ptr;
}

CB_Error cb_arena_reset(CB_Arena *arena) {
    if (arena == NULL) {
        return CB_ERR_INVALID_ARGUMENT;
    }
    arena->used = 0;
    return CB_OK;
}

size_t cb_arena_used(const CB_Arena *arena) {
    if (arena == NULL) {
        return 0;
    }
    return arena->used;
}

size_t cb_arena_capacity(const CB_Arena *arena) {
    if (arena == NULL) {
        return 0;
    }
    return arena->capacity;
}

void cb_arena_destroy(CB_Arena *arena) {
    if (arena != NULL) {
        free(arena->buffer);
        free(arena);
    }
}