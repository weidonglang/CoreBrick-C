# Roadmap

This document outlines the planned direction for CoreBrick-C. Priorities may shift based on community feedback and project needs.

## Vision

CoreBrick-C aims to be a **lightweight, dependency-free C11 utility library** that provides practical building blocks for C applications. The focus remains on:

- **Simplicity**: Easy to understand, easy to embed
- **Reliability**: Tested, documented, overflow-safe
- **Portability**: Windows (MSVC) and Ubuntu (GCC/Clang) CI-tested
- **Minimalism**: No external dependencies, no heavy frameworks

## Current Status (v0.4.x)

- **v0.4.2** — Packaging & audit cleanup patch
- **v0.4.3** (in progress) — Documentation & project site improvement:
  - README hero section, module capability matrix, project positioning
  - Examples guide, CI guide, release package guide
  - CONTRIBUTING.md, SECURITY.md, CHANGELOG.md
  - Issue/PR templates, roadmap, architecture doc
  - validate_all.ps1 validation script

## Planned Releases

### v0.5.x — HashMap & HashTable Improvements

- Generic `HashMap<K, V>` with type-safe wrappers (not just string-to-string)
- Custom hash function support for HashMap
- Performance benchmarks for hash-based containers
- Optional load factor configuration for StringMap/HashMap

### v0.6.x — JSON Parser

- Lightweight JSON parser (no schema validation, no DOM builder initially)
- Streaming parser API for large payloads
- Integration with ByteBuffer for input handling
- Basic query path support (e.g., `$.store.book[0].title`)

### v0.7.x — Thread Safety Foundation

- Thread-safe arena allocator (lock-free bump allocation)
- Read-write lock wrapper for containers
- Thread-safe StringMap and HashMap variants
- Clarified memory model and ordering guarantees

### v0.8.x — Performance & Optimization

- Optional SIMD acceleration for hash functions, bloom filter, and CRC32
- Memory-mapped file I/O (`cb_file_mmap`)
- Benchmark suite improvements with statistical reporting
- Profile-guided optimization hints

### v0.9.x — Language Bindings

- Python C extension for core data structures
- Node.js native addon
- Java JNI bindings
- Rust FFI bindings

### v1.0.0 — Stable API

- API/ABI stability guarantee
- Deprecation policy enforcement
- Comprehensive fuzz testing integration
- Formal verification of safety-critical paths
- Long-term support commitment

## Non-Goals (Out of Scope)

The following are explicitly **not planned** for CoreBrick-C:

- Network I/O (HTTP, sockets, protocol parsers)
- Cryptographic algorithms (we provide non-cryptographic hashes only)
- Database drivers or ORMs
- GUI toolkit integration
- Full-blown standard library replacement
- Real-time or RTOS-specific support
- C++ wrapper (use the C API directly from C++)
- WASM compilation target

## Contributing

See [CONTRIBUTING.md](../CONTRIBUTING.md) for how to propose changes to this roadmap or contribute to ongoing work.