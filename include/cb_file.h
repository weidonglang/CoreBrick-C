#ifndef CB_FILE_H
#define CB_FILE_H

#include <stddef.h>
#include <stdint.h>
#include "cb_error.h"
#include "cb_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

CB_Error cb_file_exists(const char *path, int *out_exists);
CB_Error cb_file_size(const char *path, uint64_t *out_size);
CB_Error cb_file_read_all(const char *path, CB_Buffer *out_buffer);
CB_Error cb_file_write_all(const char *path, const void *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* CB_FILE_H */