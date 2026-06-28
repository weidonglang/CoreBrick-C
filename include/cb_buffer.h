#ifndef CB_BUFFER_H
#define CB_BUFFER_H

#include <stddef.h>
#include <stdint.h>
#include "cb_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} CB_Buffer;

CB_Error cb_buffer_init(CB_Buffer *buffer, size_t initial_capacity);
CB_Error cb_buffer_reserve(CB_Buffer *buffer, size_t new_capacity);
CB_Error cb_buffer_append(CB_Buffer *buffer, const void *data, size_t len);
CB_Error cb_buffer_append_u8(CB_Buffer *buffer, uint8_t value);
CB_Error cb_buffer_append_u32_le(CB_Buffer *buffer, uint32_t value);
CB_Error cb_buffer_append_cstr(CB_Buffer *buffer, const char *text);
void cb_buffer_clear(CB_Buffer *buffer);
void cb_buffer_free(CB_Buffer *buffer);

#ifdef __cplusplus
}
#endif

#endif /* CB_BUFFER_H */