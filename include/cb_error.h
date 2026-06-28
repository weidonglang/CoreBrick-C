#ifndef CB_ERROR_H
#define CB_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CB_OK = 0,
    CB_ERR_INVALID_ARGUMENT,
    CB_ERR_OUT_OF_MEMORY,
    CB_ERR_BUFFER_TOO_SMALL,
    CB_ERR_INDEX_OUT_OF_BOUNDS,
    CB_ERR_PARSE,
    CB_ERR_IO,
    CB_ERR_INTERNAL
} CB_Error;

const char *cb_error_string(CB_Error err);

#ifdef __cplusplus
}
#endif

#endif /* CB_ERROR_H */