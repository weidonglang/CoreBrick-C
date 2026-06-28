# CoreBrick-C Architecture

## Overview

CoreBrick-C is a lightweight C11 core utility library organized into independent modules. Each module has a clear public API, implementation, and test suite.

## Modules

### Arena (`cb_arena.h` / `cb_arena.c`)
Linear memory allocator. Allocations are bump-allocated from a pre-allocated buffer. Freeing individual allocations is not supported; instead, the entire arena is reset at once.

### ByteBuffer (`cb_buffer.h` / `cb_buffer.c`)
Dynamic byte buffer that grows automatically. Supports appending raw bytes, integers (little-endian), and C strings.

### StringView (`cb_string_view.h` / `cb_string_view.c`)
Zero-copy string slice. Does not own memory. Provides trimming, searching, parsing, and splitting utilities.

## Dependencies

- No external dependencies beyond C standard library.
- All modules depend on `cb_error.h` for error types.
- `corebrick.h` is the umbrella header.

## Build System

- CMake 3.15+
- C11 standard
- Three targets: `corebrick_static` (library), `corebrick_tests` (test runner), `cbcli` (CLI tool)