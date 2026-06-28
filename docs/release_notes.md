# Release Notes

## v0.2.0

### New Modules

- **Vector**: Generic dynamic array with push, pop, get, set, get_ptr operations. Supports any fixed-size element type.
- **Hash**: Non-cryptographic hash utilities including FNV-1a (32/64-bit) and CRC-32.
- **FileUtils**: File utilities for checking existence, getting size, reading/writing files.
- **Timer**: Cross-platform high-resolution timer (QueryPerformanceCounter on Windows, clock_gettime on POSIX).
- **Benchmark**: Benchmark suite with measurements for Vector, Hash, File, and Timer operations.

### v0.1.0 Modules (carried forward)

- **Arena**: Linear memory allocator
- **ByteBuffer**: Dynamic byte buffer
- **StringView**: Zero-copy string slice

### Known Limitations

- No HashMap, Bloom Filter, RingBuffer, or JSON Parser yet
- No language bindings
- No SIMD optimizations
- No thread-safe variants

## v0.1.0

Initial release of CoreBrick-C.