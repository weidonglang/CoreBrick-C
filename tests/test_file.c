#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cb_file.h"

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

#define TEST_FILE "test_cb_file.tmp"

void test_file(void) {
    printf("\n--- File Module ---\n");

    /* Clean up any leftover */
    remove(TEST_FILE);

    /* NULL path failure */
    int exists;
    TEST("exists NULL path", cb_file_exists(NULL, &exists) == CB_ERR_INVALID_ARGUMENT);
    uint64_t sz;
    TEST("size NULL path", cb_file_size(NULL, &sz) == CB_ERR_INVALID_ARGUMENT);
    CB_Buffer buf;
    TEST("read NULL path", cb_file_read_all(NULL, &buf) == CB_ERR_INVALID_ARGUMENT);
    TEST("write NULL path", cb_file_write_all(NULL, "data", 4) == CB_ERR_INVALID_ARGUMENT);

    /* exists false */
    cb_file_exists("nonexistent_file_xyz", &exists);
    TEST("exists false for missing file", exists == 0);

    /* write_all success */
    const char *text = "Hello, File!";
    TEST("write_all success", cb_file_write_all(TEST_FILE, text, strlen(text)) == CB_OK);

    /* exists true */
    cb_file_exists(TEST_FILE, &exists);
    TEST("exists true after write", exists == 1);

    /* file_size normal */
    TEST("file_size normal", cb_file_size(TEST_FILE, &sz) == CB_OK && sz == 12);

    /* read_all success */
    cb_file_read_all(TEST_FILE, &buf);
    TEST("read_all success", buf.size == 12);
    TEST("read content correct", memcmp(buf.data, text, 12) == 0);
    cb_buffer_free(&buf);

    /* read missing file failure */
    TEST("read missing file", cb_file_read_all("nonexistent_file_xyz", &buf) == CB_ERR_IO);

    /* write len=0 creates empty file */
    TEST("write len=0", cb_file_write_all(TEST_FILE, "", 0) == CB_OK);
    cb_file_size(TEST_FILE, &sz);
    TEST("write len=0 creates empty file", sz == 0);

    /* binary data with zero bytes */
    unsigned char bin_data[] = { 0x00, 0x01, 0x02, 0xFF, 0xFE };
    TEST("write binary data", cb_file_write_all(TEST_FILE, bin_data, 5) == CB_OK);
    cb_file_size(TEST_FILE, &sz);
    TEST("binary file size correct", sz == 5);
    cb_file_read_all(TEST_FILE, &buf);
    TEST("binary read size correct", buf.size == 5);
    TEST("binary content correct", memcmp(buf.data, bin_data, 5) == 0);
    cb_buffer_free(&buf);

    /* empty file size */
    cb_file_write_all(TEST_FILE, "", 0);
    cb_file_size(TEST_FILE, &sz);
    TEST("empty file size=0", sz == 0);

    /* file_size on empty file */
    TEST("file_size empty file works", cb_file_size(TEST_FILE, &sz) == CB_OK && sz == 0);

    /* path with spaces */
    const char *space_path = "test path with spaces.tmp";
    cb_file_write_all(space_path, "test", 4);
    int space_exists = 0;
    cb_file_exists(space_path, &space_exists);
    TEST("path with spaces exists", space_exists == 1);
    remove(space_path);

    /* Clean up */
    remove(TEST_FILE);
}