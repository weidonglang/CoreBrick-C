# Release Notes

## v0.1.0

Initial release of CoreBrick-C.

### Modules

- **Arena**: Linear memory allocator with bump allocation, reset, and zero-initialized allocation.
- **ByteBuffer**: Dynamic byte buffer with automatic growth, append operations for bytes, integers, and strings.
- **StringView**: Zero-copy string slice with trimming, searching, comparison, parsing (i64/f64), and splitting.

### Known Limitations

- Windows-only support in this release
- No HashMap, Bloom Filter, RingBuffer, FileUtils, or Timer modules yet
- No language bindings (Python, Java, Rust, Node)
- No SIMD optimizations
- No thread-safe variants