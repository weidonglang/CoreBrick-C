# Hash Utilities

## Overview

Non-cryptographic hash functions and checksums.

## Security Notice

```
cb_hash is non-cryptographic.
Do not use it for password hashing, signatures, or security-sensitive integrity verification.
```

## API Reference

### `cb_hash_fnv1a64`
```c
uint64_t cb_hash_fnv1a64(const void *data, size_t len);
```
FNV-1a 64-bit hash. Returns the offset basis for empty/NULL input.

### `cb_hash_fnv1a32`
```c
uint32_t cb_hash_fnv1a32(const void *data, size_t len);
```
FNV-1a 32-bit hash. Returns the offset basis for empty/NULL input.

### `cb_crc32`
```c
uint32_t cb_crc32(const void *data, size_t len);
```
CRC-32 checksum (IEEE polynomial). Returns 0 for empty/NULL input.