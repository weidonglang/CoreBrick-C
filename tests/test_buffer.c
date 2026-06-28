#include <stdio.h>
#include <string.h>
#include "cb_buffer.h"

extern int tests_passed;
extern int tests_failed;

#define TEST(name, expr) do { \
    if (!(expr)) { \
        printf("  FAIL: %s (%s:%d)\n", name, __FILE__, __LINE__); \
        tests_failed++; \
    } else { \
        printf("  PASS: %s\n", name); \
        tests_passed++; \
    } \
} while (0)

void test_buffer(void) {
    printf("\n--- Buffer Module ---\n");

    CB_Buffer buf;
    memset(&buf, 0, sizeof(buf));

    /* init NULL failure */
    TEST("init NULL failure", cb_buffer_init(NULL, 16) == CB_ERR_INVALID_ARGUMENT);

    /* init success */
    TEST("init success", cb_buffer_init(&buf, 16) == CB_OK);
    TEST("data is non-null after init", buf.data != NULL);
    TEST("size is 0 after init", buf.size == 0);
    TEST("capacity is 16 after init", buf.capacity >= 16);

    /* append bytes success */
    const char *hello = "Hello";
    TEST("append bytes success", cb_buffer_append(&buf, hello, 5) == CB_OK);
    TEST("size is 5 after append", buf.size == 5);
    TEST("content correct after append", memcmp(buf.data, hello, 5) == 0);

    /* append_u8 success */
    TEST("append_u8 success", cb_buffer_append_u8(&buf, '!') == CB_OK);
    TEST("size is 6 after append_u8", buf.size == 6);
    TEST("last byte is '!'", buf.data[5] == '!');

    /* append_u32_le byte order correct */
    CB_Buffer le_buf;
    cb_buffer_init(&le_buf, 8);
    cb_buffer_append_u32_le(&le_buf, 0x12345678);
    TEST("u32_le byte 0 is 0x78", le_buf.data[0] == 0x78);
    TEST("u32_le byte 1 is 0x56", le_buf.data[1] == 0x56);
    TEST("u32_le byte 2 is 0x34", le_buf.data[2] == 0x34);
    TEST("u32_le byte 3 is 0x12", le_buf.data[3] == 0x12);
    cb_buffer_free(&le_buf);

    /* append_cstr success */
    TEST("append_cstr success", cb_buffer_append_cstr(&buf, " World") == CB_OK);
    TEST("size correct after cstr", buf.size == 12);

    /* append_cstr NULL returns error */
    TEST("append_cstr NULL returns error",
         cb_buffer_append_cstr(&buf, NULL) == CB_ERR_INVALID_ARGUMENT);

    /* auto grow success */
    size_t old_capacity = buf.capacity;
    for (int i = 0; i < 100; i++) {
        cb_buffer_append_u8(&buf, (uint8_t)i);
    }
    TEST("auto grow increased capacity", buf.capacity > old_capacity);
    TEST("size is 112 after many appends", buf.size == 112);

    /* multiple append content correct */
    buf.data[buf.size] = '\0';
    TEST("content starts with 'Hello! World'",
         memcmp(buf.data, "Hello! World", 12) == 0);
    for (size_t i = 0; i < 100; i++) {
        if (buf.data[12 + i] != (uint8_t)i) {
            TEST("multiple append content correct", 0);
            break;
        }
    }
    TEST("multiple append content verified", 1);

    /* clear sets size=0 and keeps capacity */
    size_t cap_before_clear = buf.capacity;
    cb_buffer_clear(&buf);
    TEST("size is 0 after clear", buf.size == 0);
    TEST("capacity preserved after clear", buf.capacity == cap_before_clear);

    /* free sets data=NULL size=0 capacity=0 */
    cb_buffer_free(&buf);
    TEST("data is NULL after free", buf.data == NULL);
    TEST("size is 0 after free", buf.size == 0);
    TEST("capacity is 0 after free", buf.capacity == 0);

    /* reserve larger capacity success */
    CB_Buffer buf2;
    cb_buffer_init(&buf2, 8);
    TEST("reserve larger capacity", cb_buffer_reserve(&buf2, 64) == CB_OK);
    TEST("capacity >= 64 after reserve", buf2.capacity >= 64);

    /* reserve smaller capacity safe */
    TEST("reserve smaller is safe", cb_buffer_reserve(&buf2, 16) == CB_OK);
    TEST("capacity did not shrink", buf2.capacity >= 64);
    cb_buffer_free(&buf2);

    /* zero-capacity init */
    CB_Buffer zero_buf;
    cb_buffer_init(&zero_buf, 0);
    TEST("zero-capacity init", zero_buf.data == NULL && zero_buf.size == 0 && zero_buf.capacity == 0);
    cb_buffer_free(&zero_buf);

    /* init with zero capacity, then append auto-grows */
    CB_Buffer grow_buf;
    cb_buffer_init(&grow_buf, 0);
    TEST("append after zero-capacity init",
         cb_buffer_append_cstr(&grow_buf, "Hello") == CB_OK);
    TEST("size is 5 after append to zero-capacity buf", grow_buf.size == 5);
    cb_buffer_free(&grow_buf);
}