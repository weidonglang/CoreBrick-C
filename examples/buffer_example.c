#include <stdio.h>
#include <string.h>
#include "cb_buffer.h"

int main(void) {
    printf("=== Buffer Example ===\n");

    CB_Buffer buf;
    CB_Error err = cb_buffer_init(&buf, 32);
    if (err != CB_OK) {
        printf("Failed to init buffer: %s\n", cb_error_string(err));
        return 1;
    }

    cb_buffer_append_cstr(&buf, "Hello, ");
    cb_buffer_append_u8(&buf, 'C');
    cb_buffer_append_cstr(&buf, " World!");
    cb_buffer_append_u32_le(&buf, 0x12345678);

    // Null-terminate for printing
    buf.data[buf.size] = '\0';
    printf("Buffer: %s\n", (const char *)buf.data);
    printf("Size: %zu, Capacity: %zu\n", buf.size, buf.capacity);

    // Verify LE bytes
    printf("LE bytes: %02x %02x %02x %02x\n",
           buf.data[14], buf.data[15], buf.data[16], buf.data[17]);

    cb_buffer_clear(&buf);
    printf("After clear - size: %zu, capacity: %zu\n", buf.size, buf.capacity);

    cb_buffer_free(&buf);
    printf("After free - data: %p, size: %zu, capacity: %zu\n",
           (void *)buf.data, buf.size, buf.capacity);

    return 0;
}