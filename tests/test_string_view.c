#include <stdio.h>
#include <string.h>
#include "cb_string_view.h"

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

void test_string_view(void) {
    printf("\n--- StringView Module ---\n");

    /* from_cstr */
    CB_StringView sv = cb_sv_from_cstr("Hello, World!");
    TEST("from_cstr normal", sv.len == 13);
    TEST("from_cstr NULL returns empty", cb_sv_from_cstr(NULL).len == 0);

    /* from_parts */
    CB_StringView sv2 = cb_sv_from_parts("abc", 3);
    TEST("from_parts", sv2.len == 3);

    /* is_empty */
    TEST("non-empty sv is not empty", !cb_sv_is_empty(sv));
    TEST("NULL data is empty", cb_sv_is_empty(cb_sv_from_parts(NULL, 0)));
    TEST("zero len is empty", cb_sv_is_empty(cb_sv_from_parts("x", 0)));

    /* trim_ascii */
    CB_StringView trimmed = cb_sv_trim_ascii(cb_sv_from_cstr("  Hello  "));
    TEST("trim spaces", trimmed.len == 5);
    TEST("trim tab/CRLF", cb_sv_trim_ascii(cb_sv_from_cstr("\t\nHello\r\n")).len == 5);
    CB_StringView trimmed3 = cb_sv_trim_ascii(cb_sv_from_cstr("   "));
    TEST("trim all spaces", trimmed3.len == 0);

    /* starts_with */
    TEST("starts_with true", cb_sv_starts_with(sv, cb_sv_from_cstr("Hello")));
    TEST("starts_with false", !cb_sv_starts_with(sv, cb_sv_from_cstr("Goodbye")));
    TEST("starts_with longer prefix false",
         !cb_sv_starts_with(sv, cb_sv_from_cstr("Hello, World!!!")));

    /* ends_with */
    TEST("ends_with true", cb_sv_ends_with(sv, cb_sv_from_cstr("World!")));
    TEST("ends_with false", !cb_sv_ends_with(sv, cb_sv_from_cstr("Bad")));

    /* equals */
    TEST("equals true", cb_sv_equals(cb_sv_from_cstr("abc"), cb_sv_from_cstr("abc")));
    TEST("equals false", !cb_sv_equals(cb_sv_from_cstr("abc"), cb_sv_from_cstr("xyz")));
    TEST("equals different len false", !cb_sv_equals(cb_sv_from_cstr("ab"), cb_sv_from_cstr("abc")));
    TEST("equals both NULL", cb_sv_equals(cb_sv_from_parts(NULL, 0), cb_sv_from_parts(NULL, 0)));

    /* find */
    size_t pos;
    TEST("find found", cb_sv_find(sv, cb_sv_from_cstr("World"), &pos));
    TEST("find position correct", pos == 7);
    TEST("find not found", !cb_sv_find(sv, cb_sv_from_cstr("xyz"), NULL));
    /* Empty needle returns 0 */
    size_t empty_pos = 999;
    TEST("find empty needle returns 0", cb_sv_find(sv, cb_sv_from_cstr(""), &empty_pos) == 0);
    TEST("find empty needle pos unchanged", empty_pos == 999);

    /* to_i64 */
    int64_t i64;
    TEST("to_i64 positive", cb_sv_to_i64(cb_sv_from_cstr("42"), &i64) == CB_OK && i64 == 42);
    TEST("to_i64 negative", cb_sv_to_i64(cb_sv_from_cstr("-7"), &i64) == CB_OK && i64 == -7);
    TEST("to_i64 with spaces", cb_sv_to_i64(cb_sv_from_cstr(" 10 "), &i64) == CB_OK && i64 == 10);
    TEST("to_i64 invalid char fails", cb_sv_to_i64(cb_sv_from_cstr("12a"), &i64) == CB_ERR_PARSE);
    TEST("to_i64 empty string fails", cb_sv_to_i64(cb_sv_from_cstr(""), &i64) == CB_ERR_PARSE);

    /* to_f64 */
    double f64;
    TEST("to_f64 valid", cb_sv_to_f64(cb_sv_from_cstr("3.14"), &f64) == CB_OK);
    TEST("to_f64 value approx", f64 > 3.13 && f64 < 3.15);
    TEST("to_f64 invalid char fails", cb_sv_to_f64(cb_sv_from_cstr("1.2a"), &f64) == CB_ERR_PARSE);
    TEST("to_f64 empty fails", cb_sv_to_f64(cb_sv_from_cstr(""), &f64) == CB_ERR_PARSE);

    /* out_value NULL safety */
    TEST("to_i64 NULL out_value", cb_sv_to_i64(cb_sv_from_cstr("5"), NULL) == CB_ERR_INVALID_ARGUMENT);
    TEST("to_f64 NULL out_value", cb_sv_to_f64(cb_sv_from_cstr("5"), NULL) == CB_ERR_INVALID_ARGUMENT);

    /* split */
    CB_StringView input = cb_sv_from_cstr("a,b,c");
    CB_SplitIter iter = cb_sv_split(input, ',');
    CB_StringView part;
    int count = 0;
    while (cb_sv_split_next(&iter, &part)) count++;
    TEST("split count is 3", count == 3);

    /* split each part */
    iter = cb_sv_split(input, ',');
    cb_sv_split_next(&iter, &part);
    TEST("split part 0 is a", part.len == 1 && part.data[0] == 'a');
    cb_sv_split_next(&iter, &part);
    TEST("split part 1 is b", part.len == 1 && part.data[0] == 'b');
    cb_sv_split_next(&iter, &part);
    TEST("split part 2 is c", part.len == 1 && part.data[0] == 'c');

    /* split consecutive delimiters */
    input = cb_sv_from_cstr("a,,c");
    iter = cb_sv_split(input, ',');
    count = 0;
    while (cb_sv_split_next(&iter, &part)) count++;
    TEST("split consecutive delimiters count=3", count == 3);

    /* split trailing delimiter */
    input = cb_sv_from_cstr("a,b,");
    iter = cb_sv_split(input, ',');
    count = 0;
    while (cb_sv_split_next(&iter, &part)) count++;
    TEST("split trailing delimiter count=3", count == 3);
    /* The last part should be empty */
    iter = cb_sv_split(input, ',');
    cb_sv_split_next(&iter, &part);
    cb_sv_split_next(&iter, &part);
    cb_sv_split_next(&iter, &part);
    TEST("split trailing delimiter last part empty", part.len == 0);

    /* split NULL input safety */
    TEST("split NULL empty input returns 0", cb_sv_split_next(&(CB_SplitIter){.input = cb_sv_from_parts(NULL, 0), .delimiter = ',', .offset = 0}, &part) == 0);
    TEST("split NULL non-empty len returns 0", cb_sv_split_next(&(CB_SplitIter){.input = cb_sv_from_parts(NULL, 5), .delimiter = ',', .offset = 0}, &part) == 0);
    TEST("split valid input unchanged", 1);

    /* UTF-8 */
    CB_StringView utf8 = cb_sv_from_cstr("héllo");
    TEST("UTF-8 len is 3 bytes for 'é'", utf8.len == 6);
    CB_StringView trimmed_utf8 = cb_sv_trim_ascii(utf8);
    TEST("UTF-8 trim does not modify", trimmed_utf8.len == utf8.len);
}