#include <stdio.h>
#include <string.h>
#include "cb_file.h"

int main(void) {
    printf("=== File Example ===\n");

    const char *test_file = "test_example.tmp";

    /* Write */
    const char *content = "Hello, CoreBrick File!";
    CB_Error err = cb_file_write_all(test_file, content, strlen(content));
    printf("Write: %s\n", err == CB_OK ? "OK" : "FAIL");

    /* Exists */
    int exists = 0;
    cb_file_exists(test_file, &exists);
    printf("Exists: %s\n", exists ? "yes" : "no");

    /* Size */
    uint64_t size = 0;
    cb_file_size(test_file, &size);
    printf("Size: %llu bytes\n", (unsigned long long)size);

    /* Read */
    CB_Buffer buf;
    cb_file_read_all(test_file, &buf);
    buf.data[buf.size] = '\0';
    printf("Read: %s\n", (const char *)buf.data);
    cb_buffer_free(&buf);

    remove(test_file);
    return 0;
}