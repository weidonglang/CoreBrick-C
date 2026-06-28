# Vector

## Overview

A generic dynamic array that stores elements of a fixed size. Automatically grows when full.

## API Reference

### `cb_vector_init`
```c
CB_Error cb_vector_init(CB_Vector *vec, size_t element_size, size_t initial_capacity);
```
Initializes a vector. Returns `CB_ERR_INVALID_ARGUMENT` if vec is NULL or element_size is 0.

### `cb_vector_push`
```c
CB_Error cb_vector_push(CB_Vector *vec, const void *element);
```
Appends an element. Automatically grows if needed.

### `cb_vector_get`
```c
CB_Error cb_vector_get(const CB_Vector *vec, size_t index, void *out_element);
```
Copies element at `index` to `out_element`. Returns `CB_ERR_INDEX_OUT_OF_BOUNDS` if out of range.

### `cb_vector_get_ptr`
```c
void *cb_vector_get_ptr(CB_Vector *vec, size_t index);
```
Returns a pointer to the element at `index`, or NULL if out of range.

### `cb_vector_set`
```c
CB_Error cb_vector_set(CB_Vector *vec, size_t index, const void *element);
```
Overwrites the element at `index`.

### `cb_vector_pop`
```c
CB_Error cb_vector_pop(CB_Vector *vec, void *out_element);
```
Removes and returns the last element. `out_element` can be NULL to discard.

### `cb_vector_clear`
```c
void cb_vector_clear(CB_Vector *vec);
```
Sets size to 0, preserving capacity.

### `cb_vector_free`
```c
void cb_vector_free(CB_Vector *vec);
```
Frees all memory and resets fields.