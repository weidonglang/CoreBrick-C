# File Utilities

## Overview

Small file utility functions for reading, writing, and querying files.

## API Reference

### `cb_file_exists`
```c
CB_Error cb_file_exists(const char *path, int *out_exists);
```
Checks if a file exists. Sets `*out_exists` to 1 if found, 0 otherwise.

### `cb_file_size`
```c
CB_Error cb_file_size(const char *path, uint64_t *out_size);
```
Returns the file size in bytes. Supports 0-byte files.

### `cb_file_read_all`
```c
CB_Error cb_file_read_all(const char *path, CB_Buffer *out_buffer);
```
Reads entire file into a buffer. Use binary mode.

### `cb_file_write_all`
```c
CB_Error cb_file_write_all(const char *path, const void *data, size_t len);
```
Writes data to file (binary mode). Creates or overwrites. `len=0` creates an empty file.