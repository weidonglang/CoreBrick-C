#ifndef CB_STRING_MAP_H
#define CB_STRING_MAP_H

#include <stddef.h>
#include "cb_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CB_StringMap CB_StringMap;

CB_Error cb_string_map_create(size_t initial_capacity, CB_StringMap **out_map);
CB_Error cb_string_map_put(CB_StringMap *map, const char *key, const char *value);
CB_Error cb_string_map_get(const CB_StringMap *map, const char *key, const char **out_value);
CB_Error cb_string_map_contains(const CB_StringMap *map, const char *key, int *out_contains);
CB_Error cb_string_map_remove(CB_StringMap *map, const char *key);
size_t cb_string_map_size(const CB_StringMap *map);
void cb_string_map_clear(CB_StringMap *map);
void cb_string_map_destroy(CB_StringMap *map);

#ifdef __cplusplus
}
#endif

#endif /* CB_STRING_MAP_H */