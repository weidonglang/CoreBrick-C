#include "cb_file.h"

#ifdef _MSC_VER
#define fseeko _fseeki64
#define ftello _ftelli64
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>

CB_Error cb_file_exists(const char *path, int *out_exists) {
    if (path == NULL || out_exists == NULL) return CB_ERR_INVALID_ARGUMENT;
    FILE *f = fopen(path, "rb");
    if (f != NULL) {
        fclose(f);
        *out_exists = 1;
    } else {
        *out_exists = 0;
    }
    return CB_OK;
}

CB_Error cb_file_size(const char *path, uint64_t *out_size) {
    if (path == NULL || out_size == NULL) return CB_ERR_INVALID_ARGUMENT;
    FILE *f = fopen(path, "rb");
    if (f == NULL) return CB_ERR_IO;
    if (fseeko(f, 0, SEEK_END) != 0) {
        fclose(f);
        return CB_ERR_IO;
    }
    int64_t size = (int64_t)ftello(f);
    fclose(f);
    if (size < 0) return CB_ERR_IO;
    *out_size = (uint64_t)size;
    return CB_OK;
}

CB_Error cb_file_read_all(const char *path, CB_Buffer *out_buffer) {
    if (path == NULL || out_buffer == NULL) return CB_ERR_INVALID_ARGUMENT;
    FILE *f = fopen(path, "rb");
    if (f == NULL) return CB_ERR_IO;
    if (fseeko(f, 0, SEEK_END) != 0) {
        fclose(f);
        return CB_ERR_IO;
    }
    int64_t file_size = (int64_t)ftello(f);
    if (file_size < 0) {
        fclose(f);
        return CB_ERR_IO;
    }
    rewind(f);
    /* Use size_t for allocation; check overflow */
    size_t sz = (size_t)file_size;
    if ((int64_t)sz != file_size) {
        fclose(f);
        return CB_ERR_OUT_OF_MEMORY;
    }
    /* Initialize out_buffer fresh (caller should provide uninitialized buffer) */
    out_buffer->data = NULL;
    out_buffer->size = 0;
    out_buffer->capacity = 0;
    CB_Error err = cb_buffer_init(out_buffer, sz);
    if (err != CB_OK) {
        fclose(f);
        return err;
    }
    if (sz > 0) {
        size_t bytes_read = fread(out_buffer->data, 1, sz, f);
        if (bytes_read != sz && ferror(f)) {
            fclose(f);
            cb_buffer_free(out_buffer);
            return CB_ERR_IO;
        }
        out_buffer->size = bytes_read;
    }
    fclose(f);
    return CB_OK;
}

CB_Error cb_file_write_all(const char *path, const void *data, size_t len) {
    if (path == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (data == NULL && len > 0) return CB_ERR_INVALID_ARGUMENT;
    FILE *f = fopen(path, "wb");
    if (f == NULL) return CB_ERR_IO;
    CB_Error err = CB_OK;
    if (len > 0) {
        size_t written = fwrite(data, 1, len, f);
        if (written != len) {
            fclose(f);
            return CB_ERR_IO;
        }
    }
    if (fclose(f) != 0) {
        err = CB_ERR_IO;
    }
    return err;
}