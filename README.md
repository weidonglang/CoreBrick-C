# CoreBrick-C

A lightweight C11 core utility library with memory, buffer, string, container, hash, file, timer, bitset, ring buffer, bloom filter, string map, benchmark, and CMake package integration support.

## Why CoreBrick-C?

CoreBrick-C is designed as a small, readable, dependency-light C utility library for learning, embedding, and small native tooling projects. It provides practical building blocks commonly needed in C applications — from arena allocation and dynamic buffers to hash maps, bloom filters, and cross-platform timers — without requiring heavyweight dependencies or complex build systems.

## Current Status

- **Current clean release**: [v0.4.2](https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.4.2)
- **Language**: C11 (ISO/IEC 9899:2011)
- **Build**: CMake 3.16+
- **Package usage**: `find_package(CoreBrick CONFIG REQUIRED)`
- **CI**: Windows (MSVC) + Ubuntu (GCC)
- **License**: [MIT](./LICENSE)
- **Repository**: [github.com/weidonglang/CoreBrick-C](https://github.com/weidonglang/CoreBrick-C)

[![CI Windows](https://github.com/weidonglang/CoreBrick-C/actions/workflows/ci.yml/badge.svg)](https://github.com/weidonglang/CoreBrick-C/actions/workflows/ci.yml)
[![Release](https://img.shields.io/github/v/release/weidonglang/CoreBrick-C?label=release)](https://github.com/weidonglang/CoreBrick-C/releases)
[![License](https://img.shields.io/github/license/weidonglang/CoreBrick-C)](https://github.com/weidonglang/CoreBrick-C/blob/main/LICENSE)

## Features

### v0.4.2 Packaging & Audit Cleanup Patch

- Aligns VERSION, CMake project version, and corebrick_version.h
- Improves release source package contents
- Hardens install_test scripts for Windows and Ubuntu
- Clarifies v0.4.1 as an integration audit patch
- Keeps `CoreBrick::corebrick` as the public CMake target

### v0.4.0 CMake Install & API Policy

- **Version Header** — `corebrick_version.h` for compile-time version checking
- **API Policy** — documented [API/ABI stability policy](./docs/api_policy.md)
- **CMake Install** — `cmake --install` support with `find_package` config
- **Install Guide** — [docs/install.md](./docs/install.md)
- See [Release Notes](./docs/release_notes.md) for details

### v0.3.3 Reliability Patch
- Restored version scripts and edge case tests
- Fixed CI Windows ctest configuration
- See [Release Notes](./docs/release_notes.md) for details

### v0.3.1 Reliability Patch
- 15 reliability fixes across all modules
  - StringMap: portable strdup, rehash safety
  - Arena: alignment, overflow checks
  - ByteBuffer / Vector / RingBuffer: integer overflow checks
  - FileUtils: 64-bit file size, fclose check
  - Bloom: empty key semantics
  - StringView: empty needle, NULL split
  - CRC32: static const table, thread-safe
  - CMake: examples cleanup
- See [Release Notes](./docs/release_notes.md) for details

### v0.3.0 Modules
- **BitSet** - Fixed-size bit array for state flags
- **RingBuffer** - Fixed-capacity FIFO ring buffer
- **Bloom Filter** - Probabilistic set membership filter
- **StringMap** - String-to-string hash table with open addressing

- **Vector** - Generic dynamic array
- **Hash** - Non-cryptographic hash utilities (FNV-1a, CRC-32)
- **FileUtils** - File read/write/size/exists utilities
- **Timer** - Cross-platform high-resolution timer
- **Benchmark** - Performance measurement suite

### v0.1.0 Modules
- **Arena** - Linear memory allocator for fast, scoped allocations
- **ByteBuffer** - Dynamic byte buffer with automatic growth
- **StringView** - Zero-copy string slice with parsing utilities

## When to Use CoreBrick-C

- You need a small, self-contained C utility library for a personal or embedded project.
- You want readable, well-documented C code that you can learn from and modify.
- You need practical building blocks (arena, buffer, string view, hash, file, timer, bloom filter, etc.) without adding heavy dependencies.
- You want a project with proper CMake packaging and CI that you can use as a reference for your own C projects.

## When Not to Use CoreBrick-C

- You need a production-grade replacement for GLib, uthash, xxHash, zlib, or SQLite.
- You need cryptographic-grade hash functions (CoreBrick-C uses FNV-1a and CRC32, which are **not** secure).
- You need multithreaded data structures or thread-safe containers (current version is single-thread oriented).
- You need a large ecosystem with bindings to multiple languages.

## Quick Start

### Build & Test

```powershell
# Build
cmake -S . -B build
cmake --build build

# Run tests
ctest --test-dir build --output-on-failure
```

### Install

```powershell
# Install to a local prefix
cmake --install build --prefix build/install

# Or install system-wide (requires admin)
# cmake --install build
```

### Use in External CMake Projects

Add to your `CMakeLists.txt`:

```cmake
find_package(CoreBrick CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE CoreBrick::corebrick)
```

Then configure with the install prefix:

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="/path/to/CoreBrick-C/build/install"
```

See [docs/install.md](./docs/install.md) for details.

### Run CLI & Examples

```powershell
build\Debug\cbcli.exe
build\Debug\arena_example.exe
build\Debug\buffer_example.exe
build\Debug\bitset_example.exe
build\Debug\ring_example.exe
build\Debug\bloom_example.exe
build\Debug\string_map_example.exe
build\Debug\string_view_example.exe
build\Debug\vector_example.exe
build\Debug\hash_example.exe
build\Debug\file_example.exe
build\Debug\timer_example.exe
```

### Run Benchmarks

```powershell
build\Debug\corebrick_bench.exe
```

## Project Structure

```
CoreBrick-C/
  include/    - Public headers
  src/        - Implementation
  tests/      - Unit tests
  docs/       - Documentation
  examples/   - Usage examples
  scripts/    - Build and validation scripts
  cli/        - Command-line tool
```

## License

MIT