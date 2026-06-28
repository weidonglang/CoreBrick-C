# RingBuffer

## Overview

A fixed-capacity FIFO ring buffer (circular buffer). Stores elements of a fixed size without automatic growth. When full, push returns `CB_ERR_BUFFER_TOO_SMALL`.

## API Reference

### `cb_ring_init`
```c
CB_Error cb_ring_init(CB_Ring *ring, size_t element_size, size_t capacity);
```
Initializes a ring buffer. Returns error if element_size or capacity is 0.

### `cb_ring_push`
```c
CB_Error cb_ring_push(CB_Ring *ring, const void *element);
```
Pushes an element. Returns `CB_ERR_BUFFER_TOO_SMALL` if full.

### `cb_ring_pop`
```c
CB_Error cb_ring_pop(CB_Ring *ring, void *out_element);
```
Removes and returns the oldest element.

### `cb_ring_peek`
```c
CB_Error cb_ring_peek(const CB_Ring *ring, void *out_element);
```
Returns the oldest element without removing it.

### `cb_ring_is_empty` / `cb_ring_is_full`
```c
int cb_ring_is_empty(const CB_Ring *ring);
int cb_ring_is_full(const CB_Ring *ring);
```
Check if the ring is empty or full.

### `cb_ring_clear`
```c
void cb_ring_clear(CB_Ring *ring);
```
Resets the ring without freeing memory.

### `cb_ring_free`
```c
void cb_ring_free(CB_Ring *ring);
```
Frees all memory and resets fields.