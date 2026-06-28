#include "cb_ring.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

CB_Error cb_ring_init(CB_Ring *ring, size_t element_size, size_t capacity) {
    if (ring == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (element_size == 0 || capacity == 0) return CB_ERR_INVALID_ARGUMENT;
    if (element_size > SIZE_MAX / capacity) return CB_ERR_OUT_OF_MEMORY;
    ring->data = malloc(element_size * capacity);
    if (ring->data == NULL) return CB_ERR_OUT_OF_MEMORY;
    ring->element_size = element_size;
    ring->capacity = capacity;
    ring->size = 0;
    ring->head = 0;
    ring->tail = 0;
    return CB_OK;
}

CB_Error cb_ring_push(CB_Ring *ring, const void *element) {
    if (ring == NULL || element == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (ring->data == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (ring->size >= ring->capacity) return CB_ERR_BUFFER_TOO_SMALL;
    memcpy((unsigned char *)ring->data + (ring->tail * ring->element_size),
           element, ring->element_size);
    ring->tail = (ring->tail + 1) % ring->capacity;
    ring->size++;
    return CB_OK;
}

CB_Error cb_ring_pop(CB_Ring *ring, void *out_element) {
    if (ring == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (ring->data == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (ring->size == 0) return CB_ERR_INDEX_OUT_OF_BOUNDS;
    if (out_element != NULL) {
        memcpy(out_element,
               (const unsigned char *)ring->data + (ring->head * ring->element_size),
               ring->element_size);
    }
    ring->head = (ring->head + 1) % ring->capacity;
    ring->size--;
    return CB_OK;
}

CB_Error cb_ring_peek(const CB_Ring *ring, void *out_element) {
    if (ring == NULL || out_element == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (ring->data == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (ring->size == 0) return CB_ERR_INDEX_OUT_OF_BOUNDS;
    memcpy(out_element,
           (const unsigned char *)ring->data + (ring->head * ring->element_size),
           ring->element_size);
    return CB_OK;
}

int cb_ring_is_empty(const CB_Ring *ring) {
    if (ring == NULL || ring->data == NULL) return 1;
    return ring->size == 0;
}

int cb_ring_is_full(const CB_Ring *ring) {
    if (ring == NULL || ring->data == NULL) return 0;
    return ring->size >= ring->capacity;
}

void cb_ring_clear(CB_Ring *ring) {
    if (ring != NULL) {
        ring->size = 0;
        ring->head = 0;
        ring->tail = 0;
    }
}

void cb_ring_free(CB_Ring *ring) {
    if (ring != NULL) {
        free(ring->data);
        ring->data = NULL;
        ring->element_size = 0;
        ring->capacity = 0;
        ring->size = 0;
        ring->head = 0;
        ring->tail = 0;
    }
}