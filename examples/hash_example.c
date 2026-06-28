#include <stdio.h>
#include "cb_hash.h"

int main(void) {
    printf("=== Hash Example ===\n");

    const char *text = "Hello, CoreBrick!";
    size_t len = 16;

    uint64_t h64 = cb_hash_fnv1a64(text, len);
    uint32_t h32 = cb_hash_fnv1a32(text, len);
    uint32_t crc = cb_crc32(text, len);

    printf("FNV-1a 64: %016llx\n", (unsigned long long)h64);
    printf("FNV-1a 32: %08x\n", h32);
    printf("CRC-32:    %08x\n", crc);

    return 0;
}