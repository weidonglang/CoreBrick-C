#include "cb_vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

CB_Error cb_vector_init(CB_Vector *vec, size_t element_size, size_t initial_capacity) {
    if (vec == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (element_size == 0) return CB_ERR_INVALID_ARGUMENT;
    vec->data = NULL;
    vec->element_size = element_size;
    vec->size = 0;
    vec->capacity = 0;
    if (initial_capacity > 0) {
        if (element_size > SIZE_MAX / initial_capacity) return CB_ERR_OUT_OF_MEMORY;
        vec->data = malloc(element_size * initial_capacity);
        if (vec->data == NULL) return CB_ERR_OUT_OF_MEMORY;
        vec->capacity = initial_capacity;
    }
    return CB_OK;
}

CB_Error cb_vector_push(CB_Vector *vec, const void *element) {
    if (vec == NULL || element == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (vec->size >= vec->capacity) {
        size_t new_capacity = vec->capacity == 0 ? 4 : vec->capacity * 2;
        if (new_capacity < vec->capacity) return CB_ERR_OUT_OF_MEMORY;
        if (vec->element_size > SIZE_MAX / new_capacity) return CB_ERR_OUT_OF_MEMORY;
        size_t new_capacity_bytes = new_capacity * vec->element_size;
        void *new_data = realloc(vec->data, new_capacity_bytes);
        if (new_data == NULL) return CB_ERR_OUT_OF_MEMORY;
        vec->data = new_data;
        vec->capacity = new_capacity;
    }
    memcpy((unsigned char *)vec->data + (vec->size * vec->element_size),
           element, vec->element_size);
    vec->size++;
    return CB_OK;
}

CB_Error cb_vector_get(const CB_Vector *vec, size_t index, void *out_element) {
    if (vec == NULL || out_element == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (index >= vec->size) return CB_ERR_INDEX_OUT_OF_BOUNDS;
    memcpy(out_element,
           (const unsigned char *)vec->data + (index * vec->element_size),
           vec->element_size);
    return CB_OK;
}

void *cb_vector_get_ptr(CB_Vector *vec, size_t index) {
    if (vec == NULL) return NULL;
    if (index >= vec->size) return NULL;
    return (unsigned char *)vec->data + (index * vec->element_size);
}

CB_Error cb_vector_set(CB_Vector *vec, size_t index, const void *element) {
    if (vec == NULL || element == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (index >= vec->size) return CB_ERR_INDEX_OUT_OF_BOUNDS;
    memcpy((unsigned char *)vec->data + (index * vec->element_size),
           element, vec->element_size);
    return CB_OK;
}

CB_Error cb_vector_pop(CB_Vector *vec, void *out_element) {
    if (vec == NULL) return CB_ERR_INVALID_ARGUMENT;
    if (vec->size == 0) return CB_ERR_INDEX_OUT_OF_BOUNDS;
    vec->size--;
    if (out_element != NULL) {
        memcpy(out_element,
               (const unsigned char *)vec->data + (vec->size * vec->element_size),
               vec->element_size);
    }
    return CB_OK;
}

void cb_vector_clear(CB_Vector *vec) {
    if (vec != NULL) vec->size = 0;
}

void cb_vector_free(CB_Vector *vec) {
    if (vec != NULL) {
        free(vec->data);
        vec->data = NULL;
        vec->size = 0;
        vec->capacity = 0;
        vec->element_size = 0;
    }
}