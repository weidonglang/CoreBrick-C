#include <stdio.h>
#include <string.h>
#include "cb_error.h"

int tests_passed = 0;
int tests_failed = 0;

#define TEST(name, expr) do { \
    if (!(expr)) { \
        printf("  FAIL: %s (%s:%d)\n", name, __FILE__, __LINE__); \
        tests_failed++; \
    } else { \
        printf("  PASS: %s\n", name); \
        tests_passed++; \
    } \
} while (0)

void test_arena(void);
void test_buffer(void);
void test_string_view(void);
void test_vector(void);
void test_hash(void);
void test_file(void);
void test_edge_cases(void);
void test_timer(void);

int main(void) {
    printf("=== CoreBrick-C Test Suite ===\n\n");
    printf("--- Error Module ---\n");
    TEST("cb_error_string(CB_OK) returns non-empty",
         cb_error_string(CB_OK) != NULL && strlen(cb_error_string(CB_OK)) > 0);
    TEST("cb_error_string(CB_OK) returns OK",
         strcmp(cb_error_string(CB_OK), "OK") == 0);
TEST("cb_error_string(CB_ERR_INVALID_ARGUMENT) returns non-empty",
         cb_error_string(CB_ERR_INVALID_ARGUMENT) != NULL && strlen(cb_error_string(CB_ERR_INVALID_ARGUMENT)) > 0);
    TEST("cb_error_string(CB_ERR_OUT_OF_MEMORY) returns non-empty",
         cb_error_string(CB_ERR_OUT_OF_MEMORY) != NULL && strlen(cb_error_string(CB_ERR_OUT_OF_MEMORY)) > 0);
    TEST("cb_error_string(CB_ERR_BUFFER_TOO_SMALL) returns non-empty",
         cb_error_string(CB_ERR_BUFFER_TOO_SMALL) != NULL && strlen(cb_error_string(CB_ERR_BUFFER_TOO_SMALL)) > 0);
    TEST("cb_error_string(CB_ERR_INDEX_OUT_OF_BOUNDS) returns non-empty",
         cb_error_string(CB_ERR_INDEX_OUT_OF_BOUNDS) != NULL && strlen(cb_error_string(CB_ERR_INDEX_OUT_OF_BOUNDS)) > 0);
    TEST("cb_error_string(CB_ERR_PARSE) returns non-empty",
         cb_error_string(CB_ERR_PARSE) != NULL && strlen(cb_error_string(CB_ERR_PARSE)) > 0);
    TEST("cb_error_string(CB_ERR_IO) returns non-empty",
         cb_error_string(CB_ERR_IO) != NULL && strlen(cb_error_string(CB_ERR_IO)) > 0);
    TEST("cb_error_string(CB_ERR_INTERNAL) returns non-empty",
         cb_error_string(CB_ERR_INTERNAL) != NULL && strlen(cb_error_string(CB_ERR_INTERNAL)) > 0);
    TEST("unknown error code returns fallback string",
         cb_error_string((CB_Error)999) != NULL && strlen(cb_error_string((CB_Error)999)) > 0);
    TEST("unknown error code returns unknown error",
         strcmp(cb_error_string((CB_Error)999), "unknown error") == 0);

    test_arena();
    test_buffer();
    test_string_view();
    test_vector();
    test_hash();
test_file();
    test_edge_cases();
test_timer();

    printf("\n--- Summary ---\n");
    printf("Total: %d, Passed: %d, Failed: %d\n\n",
           tests_passed + tests_failed, tests_passed, tests_failed);
    return tests_failed > 0 ? 1 : 0;
}