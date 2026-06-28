#include "cb_buffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

CB_Error cb_buffer_init(CB_Buffer *buffer, size_t initial_capacity) {
    if (buffer == NULL) return CB_ERR_INVALID_ARGUMENT;
    buffer->data = NULL;
    buffer->size = 0;
    buffer->capacity = 0;
    if (initial_capacity > 0) {
        buffer->data = (uint8_t *)malloc(initial_capacity);
        if (buffer->data == NULL) return CB_ERR_OUT_OF_MEMORY;
        buffer->capacity = initial_capacity;
    }
    return CB_OK;
}

CB_Error cb_buffer_reserve(CB_Buffer *buffer, size_t new_capacity) {
    if (buffer == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (new_capacity <= buffer->capacity) return CB_OK;
    uint8_t *new_data = (uint8_t *)realloc(buffer->data, new_capacity);
    if (new_data == NULL) return CB_ERR_OUT_OF_MEMORY;
    buffer->data = new_data;
    buffer->capacity = new_capacity;
    return CB_OK;
}

CB_Error cb_buffer_append(CB_Buffer *buffer, const void *data, size_t len) {
    if (buffer == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (len == 0) return CB_OK;
    if (data == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (len > SIZE_MAX - buffer->size) return CB_ERR_OUT_OF_MEMORY;
    if (buffer->size + len > buffer->capacity) {
        size_t new_capacity = buffer->capacity == 0 ? len : buffer->capacity * 2;
        if (new_capacity < buffer->capacity) return CB_ERR_OUT_OF_MEMORY;
        while (new_capacity < buffer->size + len) {
            size_t prev = new_capacity;
            new_capacity *= 2;
            if (new_capacity < prev) return CB_ERR_OUT_OF_MEMORY;
        }
        CB_Error err = cb_buffer_reserve(buffer, new_capacity);
        if (err != CB_OK) return err;
    }
    memcpy(buffer->data + buffer->size, data, len);
    buffer->size += len;
    return CB_OK;
}

CB_Error cb_buffer_append_u8(CB_Buffer *buffer, uint8_t value) {
    return cb_buffer_append(buffer, &value, 1);
}

CB_Error cb_buffer_append_u32_le(CB_Buffer *buffer, uint32_t value) {
    uint8_t bytes[4];
    bytes[0] = (uint8_t)(value & 0xFF);
    bytes[1] = (uint8_t)((value >> 8) & 0xFF);
    bytes[2] = (uint8_t)((value >> 16) & 0xFF);
    bytes[3] = (uint8_t)((value >> 24) & 0xFF);
    return cb_buffer_append(buffer, bytes, 4);
}

CB_Error cb_buffer_append_cstr(CB_Buffer *buffer, const char *text) {
    if (text == NULL) return CB_ERR_INVALID_ARGUMENT;
    return cb_buffer_append(buffer, text, strlen(text));
}

void cb_buffer_clear(CB_Buffer *buffer) {
    if (buffer != NULL) buffer->size = 0;
}

void cb_buffer_free(CB_Buffer *buffer) {
    if (buffer != NULL) {
        free(buffer->data);
        buffer->data = NULL;
        buffer->size = 0;
        buffer->capacity = 0;
    }
}