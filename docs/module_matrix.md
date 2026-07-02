# CoreBrick-C Module Capability Matrix

## Overview

This document provides a capability matrix for all CoreBrick-C modules, summarizing their functionality, dependencies, test coverage, and example availability.

## Capability Matrix

| Module | Data Structure | Memory Model | Thread Safety | Error Handling | Test Coverage | Example |
|---|---|---|---|---|---|---|
| Arena | Linear allocator | Owned, bump-pointer | Not thread-safe (single block) | Returns NULL on OOM | Full | arena_example |
| ByteBuffer | Dynamic byte array | Owned, contiguous | Not thread-safe | CB_ERR_OUT_OF_MEMORY | Full | buffer_example |
| StringView | String slice (non-owning) | Borrowed | Reentrant (no internal state) | Returns 0 / NULL | Full | string_view_example |
| Vector | Generic dynamic array | Owned, element-contiguous | Not thread-safe | CB_ERR_OUT_OF_MEMORY | Full | vector_example |
| Hash | Hash functions (FNV-1a, CRC32) | Stateless | Thread-safe (static tables) | N/A | Full | hash_example |
| FileUtils | File I/O utilities | Stateless | Reentrant | CB_ERR_IO / CB_ERR_NOT_FOUND | Full | file_example |
| Timer | High-resolution timer (Windows/Linux) | Owned | Not thread-safe | N/A | Full | timer_example |
| BitSet | Fixed-size bit array | Owned, inline storage | Not thread-safe | Returns 0 on bounds | Full | bitset_example |
| RingBuffer | Fixed-capacity FIFO | Owned, contiguous | Not thread-safe | Returns 0 on full/empty | Full | ring_example |
| Bloom Filter | Probabilistic set membership | Owned, bit array | Not thread-safe | CB_ERR_INVALID_ARG | Full | bloom_example |
| StringMap | String-to-string hash table | Owned (keys/values copied) | Not thread-safe | CB_ERR_OUT_OF_MEMORY | Full | string_map_example |
| Benchmark | Performance measurement suite | Owned (statistics) | Not thread-safe | N/A | N/A (standalone) | N/A |
| CLI | Command-line tool | N/A | N/A | N/A | N/A | N/A |

## Feature Comparison

| Feature | Arena | ByteBuffer | Vector | RingBuffer | StringMap | Bloom Filter | BitSet |
|---|---|---|---|---|---|---|---|
| Dynamic growth | No (fixed block) | Yes (doubling) | Yes (doubling) | No (fixed) | Yes (rehash) | No (fixed bits) | No (fixed size) |
| Element erase | No | Overwrite | By index | Dequeue | Lazy (tombstone) | No | Set/clear bit |
| Element lookup | By offset | By index | By index | FIFO peek | By key | might_contain | Test bit |
| Iteration | N/A | Manual offset | Index loop | Dequeue all | Iterate entries | N/A | Index loop |
| Memory overhead | Minimal | Capacity overhead | Capacity overhead | Fixed array | Hash table overhead | Bit array | Bit array |
| Alignment guarantee | max_align_t | 1-byte | element_size | element_size | Pointer-aligned | Bit-level | Bit-level |

## Dependency Graph

```
corebrick_tests
  ├── Arena
  ├── ByteBuffer
  ├── StringView
  ├── Vector
  │     └── ByteBuffer (internal memcpy)
  ├── Hash
  │     └── (no dependencies)
  ├── FileUtils
  │     └── ByteBuffer
  ├── Timer
  │     └── (platform API)
  ├── BitSet
  │     └── (no dependencies)
  ├── RingBuffer
  │     └── (no dependencies)
  ├── Bloom Filter
  │     └── Hash
  ├── StringMap
  │     └── Hash
  └── Edge Cases
        └── (all modules)
```

## API Surface Sizes

| Module | Public Functions | Public Types | Header Size (LOC) |
|---|---|---|---|
| Arena | 4 | 1 | ~50 |
| ByteBuffer | 9 | 1 | ~80 |
| StringView | 12 | 1 | ~100 |
| Vector | 7 | 1 | ~70 |
| Hash | 4 | 0 | ~60 |
| FileUtils | 6 | 0 | ~50 |
| Timer | 5 | 1 | ~60 |
| BitSet | 5 | 1 | ~50 |
| RingBuffer | 6 | 1 | ~60 |
| Bloom Filter | 4 | 1 | ~60 |
| StringMap | 8 | 1 | ~80 |
| CLI | Varies | 0 | ~200 |

## Platform Support

| Module | Windows (MSVC) | Ubuntu (GCC) | macOS (Clang) |
|---|---|---|---|
| Arena | ✓ | ✓ | ✓ |
| ByteBuffer | ✓ | ✓ | ✓ |
| StringView | ✓ | ✓ | ✓ |
| Vector | ✓ | ✓ | ✓ |
| Hash | ✓ | ✓ | ✓ |
| FileUtils | ✓ | ✓ | ✓ |
| Timer | ✓ (QueryPerformanceCounter) | ✓ (clock_gettime) | ✓ (clock_gettime) |
| BitSet | ✓ | ✓ | ✓ |
| RingBuffer | ✓ | ✓ | ✓ |
| Bloom Filter | ✓ | ✓ | ✓ |
| StringMap | ✓ | ✓ | ✓ |
| CLI | ✓ | ✓ | ✓ |

## Version History

| Version | Modules Added | Key Changes |
|---|---|---|
| v0.1.0 | Arena, ByteBuffer, StringView | Initial release |
| v0.2.0 | Vector, Hash, FileUtils, Timer | Core expansion |
| v0.3.0 | BitSet, RingBuffer, Bloom Filter, StringMap | Data structures |
| v0.3.1 | — | 15 reliability fixes |
| v0.3.2 | — | Edge case tests |
| v0.3.3 | — | CI and script fixes |
| v0.4.0 | CLI, CMake install | Package infrastructure |
| v0.4.1 | — | CMake target fix |
| v0.4.2 | — | Audit cleanup |

## Error Code Usage

| Error Code | Modules Using It | Meaning |
|---|---|---|
| CB_OK | All | Success |
| CB_ERR_OUT_OF_MEMORY | Arena, ByteBuffer, Vector, StringMap, RingBuffer | Allocation failure |
| CB_ERR_INVALID_ARG | Bloom Filter, StringMap | Invalid parameter |
| CB_ERR_IO | FileUtils | I/O operation failed |
| CB_ERR_NOT_FOUND | FileUtils | File does not exist |