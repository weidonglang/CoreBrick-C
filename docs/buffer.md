# ByteBuffer

## Overview

A dynamic byte buffer that grows automatically. Supports appending raw bytes, single bytes, little-endian 32-bit integers, and C strings.

## API Reference

### `cb_buffer_init`
```c
CB_Error cb_buffer_init(CB_Buffer *buffer, size_t initial_capacity);
```
Initializes a buffer. If `initial_capacity` is 0, no memory is allocated yet.

### `cb_buffer_reserve`
```c
CB_Error cb_buffer_reserve(CB_Buffer *buffer, size_t new_capacity);
```
Ensures capacity is at least `new_capacity`. No-op if already sufficient.

### `cb_buffer_append`
```c
CB_Error cb_buffer_append(CB_Buffer *buffer, const void *data, size_t len);
```
Appends `len` bytes. Automatically grows if needed.

### `cb_buffer_append_u8` / `cb_buffer_append_u32_le`
```c
CB_Error cb_buffer_append_u8(CB_Buffer *buffer, uint8_t value);
CB_Error cb_buffer_append_u32_le(CB_Buffer *buffer, uint32_t value);
```
Appends single byte or little-endian 32-bit integer.

### `cb_buffer_append_cstr`
```c
CB_Error cb_buffer_append_cstr(CB_Buffer *buffer, const char *text);
```
Appends a null-terminated string (without the null terminator).

### `cb_buffer_clear`
```c
void cb_buffer_clear(CB_Buffer *buffer);
```
Sets size to 0, preserving capacity.

### `cb_buffer_free`
```c
void cb_buffer_free(CB_Buffer *buffer);
```
Frees memory and resets all fields.