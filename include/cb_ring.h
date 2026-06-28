#ifndef CB_RING_H
#define CB_RING_H

#include <stddef.h>
#include "cb_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *data;
    size_t element_size;
    size_t capacity;
    size_t size;
    size_t head;
    size_t tail;
} CB_Ring;

CB_Error cb_ring_init(CB_Ring *ring, size_t element_size, size_t capacity);
CB_Error cb_ring_push(CB_Ring *ring, const void *element);
CB_Error cb_ring_pop(CB_Ring *ring, void *out_element);
CB_Error cb_ring_peek(const CB_Ring *ring, void *out_element);
int cb_ring_is_empty(const CB_Ring *ring);
int cb_ring_is_full(const CB_Ring *ring);
void cb_ring_clear(CB_Ring *ring);
void cb_ring_free(CB_Ring *ring);

#ifdef __cplusplus
}
#endif

#endif /* CB_RING_H */