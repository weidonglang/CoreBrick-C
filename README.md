# CoreBrick-C

A lightweight C11 core utility library with arena allocator, byte buffer, and string view modules.

## Features

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