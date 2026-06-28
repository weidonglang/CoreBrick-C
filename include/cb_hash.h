#ifndef CB_HASH_H
#define CB_HASH_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t cb_hash_fnv1a64(const void *data, size_t len);
uint32_t cb_hash_fnv1a32(const void *data, size_t len);
uint32_t cb_crc32(const void *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* CB_HASH_H */