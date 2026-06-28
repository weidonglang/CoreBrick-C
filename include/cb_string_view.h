#ifndef CB_STRING_VIEW_H
#define CB_STRING_VIEW_H

#include <stddef.h>
#include <stdint.h>
#include "cb_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *data;
    size_t len;
} CB_StringView;

CB_StringView cb_sv_from_parts(const char *data, size_t len);
CB_StringView cb_sv_from_cstr(const char *text);

int cb_sv_is_empty(CB_StringView sv);
CB_StringView cb_sv_trim_ascii(CB_StringView sv);
int cb_sv_starts_with(CB_StringView sv, CB_StringView prefix);
int cb_sv_ends_with(CB_StringView sv, CB_StringView suffix);
int cb_sv_equals(CB_StringView a, CB_StringView b);
int cb_sv_find(CB_StringView sv, CB_StringView needle, size_t *out_pos);

CB_Error cb_sv_to_i64(CB_StringView sv, int64_t *out_value);
CB_Error cb_sv_to_f64(CB_StringView sv, double *out_value);

typedef struct {
    CB_StringView input;
    char delimiter;
    size_t offset;
} CB_SplitIter;

CB_SplitIter cb_sv_split(CB_StringView input, char delimiter);
int cb_sv_split_next(CB_SplitIter *iter, CB_StringView *out_part);

#ifdef __cplusplus
}
#endif

#endif /* CB_STRING_VIEW_H */