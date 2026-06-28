#include <stdio.h>
#include "cb_string_view.h"

int main(void) {
    printf("=== StringView Example ===\n");

    CB_StringView sv = cb_sv_from_cstr("  Hello, CoreBrick World!  ");
    printf("Original: \"%.*s\"\n", (int)sv.len, sv.data);

    CB_StringView trimmed = cb_sv_trim_ascii(sv);
    printf("Trimmed: \"%.*s\"\n", (int)trimmed.len, trimmed.data);

    CB_StringView hello = cb_sv_from_cstr("Hello");
    printf("Starts with 'Hello': %s\n",
           cb_sv_starts_with(trimmed, hello) ? "yes" : "no");

    // Parse numbers
    int64_t i64;
    if (cb_sv_to_i64(cb_sv_from_cstr("  -42  "), &i64) == CB_OK) {
        printf("Parsed i64: %lld\n", (long long)i64);
    }

    double f64;
    if (cb_sv_to_f64(cb_sv_from_cstr("  3.14159  "), &f64) == CB_OK) {
        printf("Parsed f64: %f\n", f64);
    }

    // Split
    CB_StringView csv = cb_sv_from_cstr("apple,banana,cherry");
    CB_SplitIter iter = cb_sv_split(csv, ',');
    CB_StringView part;
    printf("Split: ");
    while (cb_sv_split_next(&iter, &part)) {
        printf("\"%.*s\" ", (int)part.len, part.data);
    }
    printf("\n");

    return 0;
}