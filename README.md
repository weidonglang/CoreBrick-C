# CoreBrick-C

A lightweight C11 core utility library with arena allocator, byte buffer, and string view modules.

## Features

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