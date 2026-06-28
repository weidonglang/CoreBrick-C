#include "cb_string_map.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DEFAULT_CAPACITY 16
#define MAX_LOAD_FACTOR 0.7

typedef enum {
    ENTRY_EMPTY,
    ENTRY_OCCUPIED,
    ENTRY_DELETED
} EntryStatus;

typedef struct {
    char *key;
    char *value;
    EntryStatus status;
} Entry;

struct CB_StringMap {
    Entry *entries;
    size_t capacity;
    size_t size;
    size_t tombstone_count;
};

static uint32_t hash_key(const char *key) {
    uint32_t hash = 2166136261U;
    while (*key) {
        hash ^= (uint32_t)(unsigned char)*key;
        hash *= 16777619U;
        key++;
    }
    return hash;
}

static CB_Error rehash(CB_StringMap *map, size_t new_capacity) {
    Entry *old_entries = map->entries;
    size_t old_capacity = map->capacity;
    Entry *new_entries = (Entry *)calloc(new_capacity, sizeof(Entry));
    if (new_entries == NULL) return CB_ERR_OUT_OF_MEMORY;
    map->entries = new_entries;
    map->capacity = new_capacity;
    map->size = 0;
    map->tombstone_count = 0;
    for (size_t i = 0; i < old_capacity; i++) {
        if (old_entries[i].status == ENTRY_OCCUPIED) {
            CB_Error err = cb_string_map_put(map, old_entries[i].key, old_entries[i].value);
            if (err != CB_OK) {
                map->entries = old_entries;
                map->capacity = old_capacity;
                free(new_entries);
                return err;
            }
            free(old_entries[i].key);
            free(old_entries[i].value);
        }
    }
    free(old_entries);
    return CB_OK;
}

CB_Error cb_string_map_create(size_t initial_capacity, CB_StringMap **out_map) {
    if (out_map == NULL) return CB_ERR_INVALID_ARGUMENT;
    CB_StringMap *map = (CB_StringMap *)malloc(sizeof(CB_StringMap));
    if (map == NULL) return CB_ERR_OUT_OF_MEMORY;
    size_t cap = (initial_capacity == 0) ? DEFAULT_CAPACITY : initial_capacity;
    map->entries = (Entry *)calloc(cap, sizeof(Entry));
    if (map->entries == NULL) {
        free(map);
        return CB_ERR_OUT_OF_MEMORY;
    }
    map->capacity = cap;
    map->size = 0;
    map->tombstone_count = 0;
    *out_map = map;
    return CB_OK;
}

CB_Error cb_string_map_put(CB_StringMap *map, const char *key, const char *value) {
    if (map == NULL || key == NULL) return CB_ERR_INVALID_ARGUMENT;
    if ((double)(map->size + map->tombstone_count) / (double)map->capacity >= MAX_LOAD_FACTOR) {
        CB_Error err = rehash(map, map->capacity * 2);
        if (err != CB_OK) return err;
    }
    uint32_t hash = hash_key(key);
    size_t index = hash % map->capacity;
    size_t first_deleted = map->capacity;
    for (size_t i = 0; i < map->capacity; i++) {
        size_t probe = (index + i) % map->capacity;
        if (map->entries[probe].status == ENTRY_EMPTY) {
            if (first_deleted < map->capacity) {
                probe = first_deleted;
                map->tombstone_count--;
            }
            map->entries[probe].key = _strdup(key);
            if (map->entries[probe].key == NULL) return CB_ERR_OUT_OF_MEMORY;
            map->entries[probe].value = (value != NULL) ? _strdup(value) : _strdup("");
            if (map->entries[probe].value == NULL) {
                free(map->entries[probe].key);
                return CB_ERR_OUT_OF_MEMORY;
            }
            map->entries[probe].status = ENTRY_OCCUPIED;
            map->size++;
            return CB_OK;
        } else if (map->entries[probe].status == ENTRY_DELETED) {
            if (first_deleted == map->capacity) first_deleted = probe;
        } else if (strcmp(map->entries[probe].key, key) == 0) {
            char *nv = (value != NULL) ? _strdup(value) : _strdup("");
            if (nv == NULL) return CB_ERR_OUT_OF_MEMORY;
            free(map->entries[probe].value);
            map->entries[probe].value = nv;
            return CB_OK;
        }
    }
    return CB_ERR_BUFFER_TOO_SMALL;
}

CB_Error cb_string_map_get(const CB_StringMap *map, const char *key, const char **out_value) {
    if (map == NULL || key == NULL || out_value == NULL) return CB_ERR_INVALID_ARGUMENT;
    uint32_t hash = hash_key(key);
    size_t index = hash % map->capacity;
    for (size_t i = 0; i < map->capacity; i++) {
        size_t probe = (index + i) % map->capacity;
        if (map->entries[probe].status == ENTRY_EMPTY) {
            *out_value = NULL;
            return CB_OK;
        }
        if (map->entries[probe].status == ENTRY_OCCUPIED &&
            strcmp(map->entries[probe].key, key) == 0) {
            *out_value = map->entries[probe].value;
            return CB_OK;
        }
    }
    *out_value = NULL;
    return CB_OK;
}

CB_Error cb_string_map_contains(const CB_StringMap *map, const char *key, int *out_contains) {
    if (map == NULL || key == NULL || out_contains == NULL) return CB_ERR_INVALID_ARGUMENT;
    uint32_t hash = hash_key(key);
    size_t index = hash % map->capacity;
    for (size_t i = 0; i < map->capacity; i++) {
        size_t probe = (index + i) % map->capacity;
        if (map->entries[probe].status == ENTRY_EMPTY) {
            *out_contains = 0;
            return CB_OK;
        }
        if (map->entries[probe].status == ENTRY_OCCUPIED &&
            strcmp(map->entries[probe].key, key) == 0) {
            *out_contains = 1;
            return CB_OK;
        }
    }
    *out_contains = 0;
    return CB_OK;
}

CB_Error cb_string_map_remove(CB_StringMap *map, const char *key) {
    if (map == NULL || key == NULL) return CB_ERR_INVALID_ARGUMENT;
    uint32_t hash = hash_key(key);
    size_t index = hash % map->capacity;
    for (size_t i = 0; i < map->capacity; i++) {
        size_t probe = (index + i) % map->capacity;
        if (map->entries[probe].status == ENTRY_EMPTY) return CB_OK;
        if (map->entries[probe].status == ENTRY_OCCUPIED &&
            strcmp(map->entries[probe].key, key) == 0) {
            free(map->entries[probe].key);
            free(map->entries[probe].value);
            map->entries[probe].key = NULL;
            map->entries[probe].value = NULL;
            map->entries[probe].status = ENTRY_DELETED;
            map->size--;
            map->tombstone_count++;
            return CB_OK;
        }
    }
    return CB_OK;
}

size_t cb_string_map_size(const CB_StringMap *map) {
    return (map == NULL) ? 0 : map->size;
}

void cb_string_map_clear(CB_StringMap *map) {
    if (map == NULL) return;
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->entries[i].status == ENTRY_OCCUPIED) {
            free(map->entries[i].key);
            free(map->entries[i].value);
        }
        map->entries[i].status = ENTRY_EMPTY;
        map->entries[i].key = NULL;
        map->entries[i].value = NULL;
    }
    map->size = 0;
    map->tombstone_count = 0;
}

void cb_string_map_destroy(CB_StringMap *map) {
    if (map == NULL) return;
    cb_string_map_clear(map);
    free(map->entries);
    free(map);
}
