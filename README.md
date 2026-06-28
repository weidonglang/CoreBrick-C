# CoreBrick-C

A lightweight C11 core utility library with arena allocator, byte buffer, and string view modules.

## Features

### v0.2.0 Modules
### v0.3.2 Reliability Patch
- Timer Windows QPC overflow fixed (divide-first-then-multiply pattern)
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

## Quick Start

```powershell
# Build
cmake -S . -B build
cmake --build build

# Run tests
ctest --test-dir build --output-on-failure

# Run CLI
build\Debug\cbcli.exe

# Run examples
build\Debug\arena_example.exe
build\Debug\buffer_example.exe
build\\Debug\\bitset_example.exe
build\\Debug\\ring_example.exe
build\\Debug\\bloom_example.exe
build\\Debug\\string_map_example.exe


build\Debug\string_view_example.exe
build\Debug\vector_example.exe
build\Debug\hash_example.exe
build\Debug\file_example.exe
build\Debug\timer_example.exe

# Run benchmarks
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