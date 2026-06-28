#include "cb_string_view.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

CB_StringView cb_sv_from_parts(const char *data, size_t len) {
    CB_StringView sv;
    sv.data = data;
    sv.len = len;
    return sv;
}

CB_StringView cb_sv_from_cstr(const char *text) {
    CB_StringView sv;
    if (text == NULL) {
        sv.data = NULL;
        sv.len = 0;
    } else {
        sv.data = text;
        sv.len = strlen(text);
    }
    return sv;
}

int cb_sv_is_empty(CB_StringView sv) {
    return sv.len == 0 || sv.data == NULL;
}

CB_StringView cb_sv_trim_ascii(CB_StringView sv) {
    if (sv.data == NULL || sv.len == 0) {
        return sv;
    }

    const char *start = sv.data;
    const char *end = sv.data + sv.len - 1;

    while (start <= end && isspace((unsigned char)*start)) {
        start++;
    }

    while (end >= start && isspace((unsigned char)*end)) {
        end--;
    }

    CB_StringView result;
    result.data = start;
    result.len = (size_t)(end - start + 1);
    return result;
}

int cb_sv_starts_with(CB_StringView sv, CB_StringView prefix) {
    if (prefix.len > sv.len) {
        return 0;
    }
    if (sv.data == NULL || prefix.data == NULL) {
        return 0;
    }
    return memcmp(sv.data, prefix.data, prefix.len) == 0;
}

int cb_sv_ends_with(CB_StringView sv, CB_StringView suffix) {
    if (suffix.len > sv.len) {
        return 0;
    }
    if (sv.data == NULL || suffix.data == NULL) {
        return 0;
    }
    return memcmp(sv.data + sv.len - suffix.len, suffix.data, suffix.len) == 0;
}

int cb_sv_equals(CB_StringView a, CB_StringView b) {
    if (a.len != b.len) {
        return 0;
    }
    if (a.data == NULL && b.data == NULL) {
        return 1;
    }
    if (a.data == NULL || b.data == NULL) {
        return 0;
    }
    return memcmp(a.data, b.data, a.len) == 0;
}

int cb_sv_find(CB_StringView sv, CB_StringView needle, size_t *out_pos) {
    if (needle.len == 0) {
        if (out_pos != NULL) {
            *out_pos = 0;
        }
        return 1;
    }
    if (needle.len > sv.len) {
        return 0;
    }
    if (sv.data == NULL || needle.data == NULL) {
        return 0;
    }

    for (size_t i = 0; i <= sv.len - needle.len; i++) {
        if (memcmp(sv.data + i, needle.data, needle.len) == 0) {
            if (out_pos != NULL) {
                *out_pos = i;
            }
            return 1;
        }
    }
    return 0;
}

CB_Error cb_sv_to_i64(CB_StringView sv, int64_t *out_value) {
    if (out_value == NULL) {
        return CB_ERR_INVALID_ARGUMENT;
    }
    if (sv.data == NULL || sv.len == 0) {
        return CB_ERR_PARSE;
    }

    // Trim whitespace
    CB_StringView trimmed = cb_sv_trim_ascii(sv);
    if (trimmed.len == 0) {
        return CB_ERR_PARSE;
    }

    // Create a null-terminated copy for strtol
    char *endptr = NULL;
    char *copy = (char *)malloc(trimmed.len + 1);
    if (copy == NULL) {
        return CB_ERR_OUT_OF_MEMORY;
    }
    memcpy(copy, trimmed.data, trimmed.len);
    copy[trimmed.len] = '\0';

    errno = 0;
    int64_t result = (int64_t)strtoll(copy, &endptr, 10);

    if (endptr != copy + trimmed.len || errno == ERANGE) {
        free(copy);
        return CB_ERR_PARSE;
    }

    free(copy);
    *out_value = result;
    return CB_OK;
}

CB_Error cb_sv_to_f64(CB_StringView sv, double *out_value) {
    if (out_value == NULL) {
        return CB_ERR_INVALID_ARGUMENT;
    }
    if (sv.data == NULL || sv.len == 0) {
        return CB_ERR_PARSE;
    }

    // Trim whitespace
    CB_StringView trimmed = cb_sv_trim_ascii(sv);
    if (trimmed.len == 0) {
        return CB_ERR_PARSE;
    }

    // Create a null-terminated copy for strtod
    char *endptr = NULL;
    char *copy = (char *)malloc(trimmed.len + 1);
    if (copy == NULL) {
        return CB_ERR_OUT_OF_MEMORY;
    }
    memcpy(copy, trimmed.data, trimmed.len);
    copy[trimmed.len] = '\0';

    errno = 0;
    double result = strtod(copy, &endptr);

    if (endptr != copy + trimmed.len || errno == ERANGE) {
        free(copy);
        return CB_ERR_PARSE;
    }

    free(copy);
    *out_value = result;
    return CB_OK;
}

CB_SplitIter cb_sv_split(CB_StringView input, char delimiter) {
    CB_SplitIter iter;
    iter.input = input;
    iter.delimiter = delimiter;
    iter.offset = 0;
    return iter;
}

int cb_sv_split_next(CB_SplitIter *iter, CB_StringView *out_part) {
    if (iter == NULL || out_part == NULL) {
        return 0;
    }
    if (iter->offset > iter->input.len) {
        return 0;
    }

    size_t start = iter->offset;
    size_t pos = start;

    while (pos < iter->input.len && iter->input.data[pos] != iter->delimiter) {
        pos++;
    }

    out_part->data = iter->input.data + start;
    out_part->len = pos - start;

    iter->offset = pos + 1;
    return 1;
}