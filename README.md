# CoreBrick-C

A lightweight C11 core utility library with arena allocator, byte buffer, and string view modules.

## Features

### v0.2.0 Modules
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