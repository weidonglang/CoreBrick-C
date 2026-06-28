#include "cb_error.h"

const char *cb_error_string(CB_Error err) {
    switch (err) {
        case CB_OK:                    return "OK";
        case CB_ERR_INVALID_ARGUMENT:  return "invalid argument";
        case CB_ERR_OUT_OF_MEMORY:     return "out of memory";
        case CB_ERR_BUFFER_TOO_SMALL:  return "buffer too small";
        case CB_ERR_INDEX_OUT_OF_BOUNDS: return "index out of bounds";
        case CB_ERR_PARSE:             return "parse error";
        case CB_ERR_IO:                return "I/O error";
        case CB_ERR_INTERNAL:          return "internal error";
        default:                       return "unknown error";
    }
}