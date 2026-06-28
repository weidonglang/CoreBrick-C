# StringView

## Overview

A zero-copy string slice that does not own memory. Provides operations for trimming, searching, comparing, parsing, and splitting without allocation.

## API Reference

### `cb_sv_from_parts` / `cb_sv_from_cstr`
```c
CB_StringView cb_sv_from_parts(const char *data, size_t len);
CB_StringView cb_sv_from_cstr(const char *text);
```
Create a StringView from parts or a null-terminated string. NULL-safe.

### `cb_sv_is_empty`
```c
int cb_sv_is_empty(CB_StringView sv);
```
Returns 1 if data is NULL or length is 0.

### `cb_sv_trim_ascii`
```c
CB_StringView cb_sv_trim_ascii(CB_StringView sv);
```
Removes leading and trailing ASCII whitespace.

### `cb_sv_starts_with` / `cb_sv_ends_with`
```c
int cb_sv_starts_with(CB_StringView sv, CB_StringView prefix);
int cb_sv_ends_with(CB_StringView sv, CB_StringView suffix);
```
Check prefix/suffix.

### `cb_sv_equals`
```c
int cb_sv_equals(CB_StringView a, CB_StringView b);
```
Content equality check.

### `cb_sv_find`
```c
int cb_sv_find(CB_StringView sv, CB_StringView needle, size_t *out_pos);
```
Find first occurrence of needle. Returns 1 if found.

### `cb_sv_to_i64` / `cb_sv_to_f64`
```c
CB_Error cb_sv_to_i64(CB_StringView sv, int64_t *out_value);
CB_Error cb_sv_to_f64(CB_StringView sv, double *out_value);
```
Parse string to integer or double.

### `cb_sv_split` / `cb_sv_split_next`
```c
CB_SplitIter cb_sv_split(CB_StringView input, char delimiter);
int cb_sv_split_next(CB_SplitIter *iter, CB_StringView *out_part);
```
Split string by delimiter. Iterator-based API.