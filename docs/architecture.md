# Architecture

This document describes the high-level architecture, module layering, and design conventions of CoreBrick-C.

## Overview

CoreBrick-C is a C11 utility library organized as a set of independent modules. Each module provides a single data structure or utility, with a consistent error-handling pattern, naming convention, and memory ownership model.

```
┌─────────────────────────────────────────────────────┐
│                    Public API                        │
│  corebrick.h (umbrella header)                      │
│  include/cb_*.h (per-module headers)                │
├─────────────────────────────────────────────────────┤
│                                                      │
│  ┌──────────┐  ┌──────────┐  ┌──────────────────┐  │
│  │  Arena   │  │ ByteBuf  │  │   StringView     │  │
│  │ (memory) │  │ (buffer) │  │ (string slice)   │  │
│  └────┬─────┘  └────┬─────┘  └────────┬─────────┘  │
│       │              │                  │            │
│  ┌────▼──────────────▼──────────────────▼─────────┐ │
│  │              Container Layer                    │ │
│  │  Vector │ StringMap │ RingBuffer │ BitSet      │ │
│  │  Bloom Filter (probabilistic)                  │ │
│  └────────────────────┬───────────────────────────┘ │
│                       │                              │
│  ┌────────────────────▼───────────────────────────┐ │
│  │              Utility Layer                      │ │
│  │  Hash (FNV-1a, CRC32) │ FileUtils │ Timer     │ │
│  │  Benchmark                                      │ │
│  └─────────────────────────────────────────────────┘ │
│                                                      │
│  ┌─────────────────────────────────────────────────┐ │
│  │              Error Handling Layer                │ │
│  │  CB_Error enum │ cb_error_string()              │ │
│  └─────────────────────────────────────────────────┘ │
├─────────────────────────────────────────────────────┤
│                    Build System                      │
│  CMake 3.16+ │ corebrick_static target              │ │
│  find_package(CoreBrick CONFIG REQUIRED) support    │ │
└─────────────────────────────────────────────────────┘
```

## Module Layering

### Layer 0: Foundation (v0.1.0)

These modules have no internal dependencies on other CoreBrick modules and form the base of the library.

| Module | Header | Purpose |
|---|---|---|
| **Arena** | `cb_arena.h` | Linear memory allocator (bump allocator) |
| **ByteBuffer** | `cb_buffer.h` | Dynamic byte buffer with append/reserve/truncate |
| **StringView** | `cb_string_view.h` | Zero-copy string slice with find/split/trim |

### Layer 1: Containers (v0.2.0, v0.3.0)

These modules depend on Layer 0.

| Module | Header | Depends On | Purpose |
|---|---|---|---|
| **Vector** | `cb_vector.h` | Arena | Generic dynamic array for fixed-size elements |
| **StringMap** | `cb_string_map.h` | Arena, Hash | String-to-string hash table with open addressing |
| **RingBuffer** | `cb_ring.h` | — | Fixed-capacity FIFO ring buffer |
| **BitSet** | `cb_bitset.h` | — | Fixed-size bit array for state flags |
| **Bloom Filter** | `cb_bloom.h` | BitSet, Hash | Probabilistic set membership filter |

### Layer 2: Utilities (v0.2.0)

These modules are standalone or depend only on Layer 0.

| Module | Header | Depends On | Purpose |
|---|---|---|---|
| **Hash** | `cb_hash.h` | — | Non-cryptographic hashes (FNV-1a, CRC-32) |
| **FileUtils** | `cb_file.h` | ByteBuffer | File read/write/size/exists utilities |
| **Timer** | `cb_timer.h` | — | Cross-platform high-resolution timer |
| **Benchmark** | `cb_bench.h` | Timer | Performance measurement suite |

### Error Handling

| Module | Header | Purpose |
|---|---|---|
| **Error** | `cb_error.h` | Unified error codes and error string conversion |

All modules return `CB_Error` (or a type that includes it) for error propagation. See [Error Handling](#error-handling) below.

## Design Conventions

### Naming

- **Public functions**: `cb_<module>_<operation>()` — e.g., `cb_arena_alloc()`, `cb_sv_find()`
- **Internal functions**: `static` in `.c` files, prefixed for clarity where needed
- **Types**: `CB_<Module>` — e.g., `CB_Arena`, `CB_ByteBuffer`, `CB_StringView`
- **Error codes**: `CB_ERR_<REASON>` — e.g., `CB_ERR_OUT_OF_MEMORY`, `CB_ERR_INVALID_ARGUMENT`
- **Macros**: `CB_<MODULE>_<CONSTANT>` — e.g., `CB_BLOOM_DEFAULT_SIZE`

### Memory Ownership

- **Single-thread oriented**: No internal locking; the caller is responsible for synchronization
- **Arena-based allocation**: Vector and StringMap use Arena for internal allocations where possible
- **Caller-owned output buffers**: Functions like `cb_file_read_all` write to `CB_ByteBuffer*` provided by the caller
- **No hidden allocations**: Every allocation is explicit and can fail with `CB_ERR_OUT_OF_MEMORY`
- **No global state**: No hidden global variables or lazy initialization (except for the CRC32 static table, which is `const`)

### Error Handling

Every function that can fail returns `CB_Error`. The convention is:

```c
CB_Error err = cb_some_operation(&result, arg1, arg2);
if (err != CB_OK) {
    // handle error
    return err;
}
// use result
```

Functions that cannot fail (e.g., `cb_sv_length()`, `cb_sv_data()`) return values directly.

### Memory Alignment

- Arena allocations are aligned to `max_align_t` (typically 8 or 16 bytes)
- Vector and RingBuffer operate on fixed-size elements and do not add padding
- BitSet operates on byte-aligned storage

## File Structure

```
CoreBrick-C/
├── include/            # Public headers (one per module + umbrella)
│   ├── corebrick.h
│   ├── corebrick_version.h
│   ├── cb_error.h
│   ├── cb_arena.h
│   ├── cb_buffer.h
│   ├── cb_string_view.h
│   ├── cb_vector.h
│   ├── cb_hash.h
│   ├── cb_file.h
│   ├── cb_timer.h
│   ├── cb_bench.h
│   ├── cb_bitset.h
│   ├── cb_ring.h
│   ├── cb_bloom.h
│   └── cb_string_map.h
├── src/                # Implementation files
│   ├── cb_arena.c
│   ├── cb_buffer.c
│   ├── cb_string_view.c
│   ├── cb_vector.c
│   ├── cb_hash.c
│   ├── cb_file.c
│   ├── cb_timer.c
│   ├── cb_bench.c
│   ├── cb_bitset.c
│   ├── cb_ring.c
│   ├── cb_bloom.c
│   └── cb_string_map.c
├── tests/              # Test files (one per module)
│   ├── test_arena.c
│   ├── test_buffer.c
│   ├── test_string_view.c
│   ├── test_vector.c
│   ├── test_hash.c
│   ├── test_file.c
│   ├── test_timer.c
│   ├── test_bench.c
│   ├── test_bitset.c
│   ├── test_ring.c
│   ├── test_bloom.c
│   ├── test_string_map.c
│   └── test_edge_cases.c
├── docs/               # Documentation
├── examples/           # Example programs
├── scripts/            # Build, test, packaging scripts
├── cmake/              # CMake package config templates
└── .github/            # CI workflows, issue/PR templates
```

## CI Testing Matrix

| Platform | Compiler | Config | Tests |
|---|---|---|---|
| Windows (latest) | MSVC | Release | All unit tests + edge cases |
| Ubuntu (latest) | GCC | Release | All unit tests + edge cases |

## Design Constraints

- **C11 standard**: No C99 or C17 extensions required
- **No dynamic dependencies**: Standard library only (stdlib.h, string.h, stdint.h, etc.)
- **No POSIX-specific APIs**: FileUtils uses `fopen`/`fclose`/`fread`/`fwrite` (C11 standard)
- **No thread-local storage**: All state is in caller-provided structs
- **No variable-length arrays (VLAs)**: All dynamic allocations use `malloc`/`calloc`
- **No platform-specific alignment assumptions**: Alignment is explicit via `max_align_t` and `_Alignas`