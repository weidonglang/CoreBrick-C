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

    /* from_cstr normal */
    CB_StringView sv = cb_sv_from_cstr("Hello, World!");
    TEST("from_cstr normal", sv.data != NULL && sv.len == 13);

    /* from_cstr NULL returns empty */
    CB_StringView empty = cb_sv_from_cstr(NULL);
    TEST("from_cstr NULL returns empty", empty.data == NULL && empty.len == 0);

    /* from_parts */
    const char *text = "Test123";
    CB_StringView sv2 = cb_sv_from_parts(text, 4);
    TEST("from_parts", sv2.data == text && sv2.len == 4);

    /* is_empty */
    TEST("non-empty sv is not empty", !cb_sv_is_empty(sv));
    CB_StringView empty_sv = {NULL, 0};
    TEST("NULL data is empty", cb_sv_is_empty(empty_sv));
    CB_StringView zero_len = {text, 0};
    TEST("zero len is empty", cb_sv_is_empty(zero_len));

    /* trim spaces */
    CB_StringView spaced = cb_sv_from_cstr("  hello  ");
    CB_StringView trimmed = cb_sv_trim_ascii(spaced);
    TEST("trim spaces", trimmed.len == 5 && memcmp(trimmed.data, "hello", 5) == 0);

    /* trim tab / CRLF */
    CB_StringView tabbed = cb_sv_from_cstr("\t\n\rhello\t\n\r");
    CB_StringView trimmed2 = cb_sv_trim_ascii(tabbed);
    TEST("trim tab/CRLF", trimmed2.len == 5 && memcmp(trimmed2.data, "hello", 5) == 0);

    /* trim all spaces */
    CB_StringView all_spaces = cb_sv_from_cstr("   ");
    CB_StringView trimmed3 = cb_sv_trim_ascii(all_spaces);
    TEST("trim all spaces", trimmed3.len == 0);

    /* starts_with true / false */
    CB_StringView prefix_hello = cb_sv_from_cstr("Hello");
    TEST("starts_with true", cb_sv_starts_with(sv, prefix_hello));
    CB_StringView prefix_goodbye = cb_sv_from_cstr("Goodbye");
    TEST("starts_with false", !cb_sv_starts_with(sv, prefix_goodbye));
    TEST("starts_with longer prefix false",
         !cb_sv_starts_with(sv, cb_sv_from_cstr("Hello, World!!!")));

    /* ends_with true / false */
    CB_StringView suffix_world = cb_sv_from_cstr("World!");
    TEST("ends_with true", cb_sv_ends_with(sv, suffix_world));
    CB_StringView suffix_bad = cb_sv_from_cstr("Bad");
    TEST("ends_with false", !cb_sv_ends_with(sv, suffix_bad));

    /* equals true / false */
    CB_StringView a = cb_sv_from_cstr("abc");
    CB_StringView b = cb_sv_from_cstr("abc");
    CB_StringView c = cb_sv_from_cstr("xyz");
    TEST("equals true", cb_sv_equals(a, b));
    TEST("equals false", !cb_sv_equals(a, c));
    TEST("equals different len false", !cb_sv_equals(a, cb_sv_from_cstr("abcd")));
    TEST("equals both NULL",
         cb_sv_equals(cb_sv_from_cstr(NULL), cb_sv_from_cstr(NULL)));

    /* find found */
    size_t pos;
    CB_StringView needle = cb_sv_from_cstr("World");
    TEST("find found", cb_sv_find(sv, needle, &pos));
    TEST("find position correct", pos == 7);

    /* find not found */
    CB_StringView not_found = cb_sv_from_cstr("xyz");
    TEST("find not found", !cb_sv_find(sv, not_found, NULL));

    /* find empty needle returns 0 */
    CB_StringView empty_needle = cb_sv_from_cstr("");
    size_t empty_pos = 999;
    TEST("find empty needle", cb_sv_find(sv, empty_needle, &empty_pos));
    TEST("find empty needle pos=0", empty_pos == 0);
/* to_i64 positive */
    int64_t i64_val;
    TEST("to_i64 positive",
         cb_sv_to_i64(cb_sv_from_cstr("42"), &i64_val) == CB_OK && i64_val == 42);

    /* to_i64 negative */
    TEST("to_i64 negative",
         cb_sv_to_i64(cb_sv_from_cstr("-123"), &i64_val) == CB_OK && i64_val == -123);

    /* to_i64 with surrounding spaces */
    TEST("to_i64 with spaces",
         cb_sv_to_i64(cb_sv_from_cstr("  456  "), &i64_val) == CB_OK && i64_val == 456);

    /* to_i64 invalid char fails */
    TEST("to_i64 invalid char fails",
         cb_sv_to_i64(cb_sv_from_cstr("12a34"), &i64_val) == CB_ERR_PARSE);

    /* to_i64 empty string fails */
    TEST("to_i64 empty string fails",
         cb_sv_to_i64(cb_sv_from_cstr(""), &i64_val) == CB_ERR_PARSE);

    /* to_f64 valid decimal */
    double f64_val;
    TEST("to_f64 valid",
         cb_sv_to_f64(cb_sv_from_cstr("3.14"), &f64_val) == CB_OK);
    TEST("to_f64 value approx", f64_val > 3.13 && f64_val < 3.15);

    /* to_f64 invalid char fails */
    TEST("to_f64 invalid char fails",
         cb_sv_to_f64(cb_sv_from_cstr("12.a"), &f64_val) == CB_ERR_PARSE);

    /* to_f64 empty fails */
    TEST("to_f64 empty fails",
         cb_sv_to_f64(cb_sv_from_cstr(""), &f64_val) == CB_ERR_PARSE);

    /* to_i64 NULL out_value */
    TEST("to_i64 NULL out_value",
         cb_sv_to_i64(cb_sv_from_cstr("42"), NULL) == CB_ERR_INVALID_ARGUMENT);

    /* to_f64 NULL out_value */
    TEST("to_f64 NULL out_value",
         cb_sv_to_f64(cb_sv_from_cstr("3.14"), NULL) == CB_ERR_INVALID_ARGUMENT);

    /* split normal string */
    CB_StringView csv = cb_sv_from_cstr("a,b,c");
    CB_SplitIter iter = cb_sv_split(csv, ',');
    CB_StringView part;
    int count = 0;
    while (cb_sv_split_next(&iter, &part)) {
        count++;
    }
    TEST("split count is 3", count == 3);

    /* split verify parts */
    iter = cb_sv_split(csv, ',');
    cb_sv_split_next(&iter, &part);
    TEST("split part 0 is a", part.len == 1 && part.data[0] == 'a');
    cb_sv_split_next(&iter, &part);
    TEST("split part 1 is b", part.len == 1 && part.data[0] == 'b');
    cb_sv_split_next(&iter, &part);
    TEST("split part 2 is c", part.len == 1 && part.data[0] == 'c');

    /* split consecutive delimiters creates empty field */
    CB_StringView csv2 = cb_sv_from_cstr("a,,c");
    iter = cb_sv_split(csv2, ',');
    count = 0;
    while (cb_sv_split_next(&iter, &part)) {
        count++;
    }
    TEST("split consecutive delimiters count=3", count == 3);

    /* split trailing delimiter creates empty field */
    CB_StringView csv3 = cb_sv_from_cstr("a,b,");
    iter = cb_sv_split(csv3, ',');
    count = 0;
    while (cb_sv_split_next(&iter, &part)) {
        count++;
    }
    TEST("split trailing delimiter count=3", count == 3);
    /* Verify last part is empty */
    iter = cb_sv_split(csv3, ',');
    cb_sv_split_next(&iter, &part);
    cb_sv_split_next(&iter, &part);
    cb_sv_split_next(&iter, &part);
    TEST("split trailing delimiter last part empty",
         part.len == 0 && part.data != NULL);

    /* UTF-8 bytes are not modified */
    /* Use raw UTF-8 bytes: U+0041 U+0042 U+0043 = "ABC" */
    const char utf8_bytes[] = { 'A', 'B', 'C', 0 };
    CB_StringView utf8 = cb_sv_from_cstr(utf8_bytes);
    TEST("UTF-8 len is 3 bytes", utf8.len == 3);
    TEST("UTF-8 trim does not modify", cb_sv_equals(cb_sv_trim_ascii(utf8), utf8));
}