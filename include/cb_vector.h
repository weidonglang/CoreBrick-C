#ifndef CB_VECTOR_H
#define CB_VECTOR_H

#include <stddef.h>
#include "cb_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *data;
    size_t element_size;
    size_t size;
    size_t capacity;
} CB_Vector;

CB_Error cb_vector_init(CB_Vector *vec, size_t element_size, size_t initial_capacity);
CB_Error cb_vector_push(CB_Vector *vec, const void *element);
CB_Error cb_vector_get(const CB_Vector *vec, size_t index, void *out_element);
void *cb_vector_get_ptr(CB_Vector *vec, size_t index);
CB_Error cb_vector_set(CB_Vector *vec, size_t index, const void *element);
CB_Error cb_vector_pop(CB_Vector *vec, void *out_element);
void cb_vector_clear(CB_Vector *vec);
void cb_vector_free(CB_Vector *vec);

#ifdef __cplusplus
}
#endif

#endif /* CB_VECTOR_H */