#include "cb_hash.h"
#include <string.h>

/* FNV-1a 64-bit */
#define FNV1A64_OFFSET_BASIS 14695981039346656037ULL
#define FNV1A64_PRIME        1099511628211ULL

uint64_t cb_hash_fnv1a64(const void *data, size_t len) {
    if (data == NULL || len == 0) {
        return FNV1A64_OFFSET_BASIS;
    }

    uint64_t hash = FNV1A64_OFFSET_BASIS;
    const unsigned char *bytes = (const unsigned char *)data;

    for (size_t i = 0; i < len; i++) {
        hash ^= (uint64_t)bytes[i];
        hash *= FNV1A64_PRIME;
    }

    return hash;
}

/* FNV-1a 32-bit */
#define FNV1A32_OFFSET_BASIS 2166136261U
#define FNV1A32_PRIME        16777619U

uint32_t cb_hash_fnv1a32(const void *data, size_t len) {
    if (data == NULL || len == 0) {
        return FNV1A32_OFFSET_BASIS;
    }

    uint32_t hash = FNV1A32_OFFSET_BASIS;
    const unsigned char *bytes = (const unsigned char *)data;

    for (size_t i = 0; i < len; i++) {
        hash ^= (uint32_t)bytes[i];
        hash *= FNV1A32_PRIME;
    }

    return hash;
}

/* CRC-32 (IEEE polynomial) */
#define CRC32_POLY 0xEDB88320U

static uint32_t crc32_table[256];
static int crc32_table_initialized = 0;

static void crc32_init_table(void) {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ CRC32_POLY;
            } else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
    crc32_table_initialized = 1;
}

uint32_t cb_crc32(const void *data, size_t len) {
    if (data == NULL || len == 0) {
        return 0;
    }

    if (!crc32_table_initialized) {
        crc32_init_table();
    }

    uint32_t crc = 0xFFFFFFFFU;
    const unsigned char *bytes = (const unsigned char *)data;

    for (size_t i = 0; i < len; i++) {
        crc = crc32_table[(crc ^ bytes[i]) & 0xFF] ^ (crc >> 8);
    }

    return crc ^ 0xFFFFFFFFU;
}