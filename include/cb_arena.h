#ifndef CB_ARENA_H
#define CB_ARENA_H

#include <stddef.h>
#include "cb_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CB_Arena CB_Arena;

CB_Error cb_arena_create(size_t capacity, CB_Arena **out_arena);
void *cb_arena_alloc(CB_Arena *arena, size_t size);
void *cb_arena_alloc_zero(CB_Arena *arena, size_t size);
CB_Error cb_arena_reset(CB_Arena *arena);
size_t cb_arena_used(const CB_Arena *arena);
size_t cb_arena_capacity(const CB_Arena *arena);
void cb_arena_destroy(CB_Arena *arena);

#ifdef __cplusplus
}
#endif

#endif /* CB_ARENA_H */