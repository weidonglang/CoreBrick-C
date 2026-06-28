# CoreBrick-C API / ABI Policy

## Versioning

CoreBrick-C follows a semantic versioning style:

```
MAJOR.MINOR.PATCH
```

- **MAJOR**: Incompatible API changes
- **MINOR**: Backward-compatible feature additions
- **PATCH**: Backward-compatible bug fixes

## Current Phase

CoreBrick-C is currently in **v0.x pre-release phase**.

During this phase:

- API may change between minor versions as the library matures.
- PATCH releases contain only bug fixes and reliability improvements.
- MINOR releases may add new modules, functions, or features.

## Public API Boundary

- **Public API**: All headers in `include/*.h`
- **Internal implementation**: All source files in `src/*.c`

Only symbols declared in `include/*.h` are considered part of the public API. Internal functions and types in `src/*.c` are implementation details and must not be used directly.

## ABI Stability

- **No ABI guarantee**: CoreBrick-C v0.x does not guarantee ABI stability.
- Static library usage is the primary distribution method.
- Recompilation of client code is expected after library updates.

## Thread Safety

- CoreBrick-C v0.x does **not** provide thread safety guarantees.
- Concurrent access to the same object from multiple threads without external synchronization is unsafe.
- This policy may change in a future major version.

## Supported Usage

- Static linkage via `corebrick_static.lib` / `libcorebrick_static.a`
- CMake `find_package(CoreBrick CONFIG REQUIRED)` with `CoreBrick::corebrick` target
- Manual include + link (not recommended but supported)

## Unsupported Usage

- Dynamic linking (shared libraries) — not tested, no export annotations
- Direct inclusion of `src/*.c` files
- Calling internal functions or accessing internal data structures
- Thread-unsafe concurrent access patterns

## Compatibility

- Target language standard: C11
- Supported compilers: MSVC, GCC, Clang
- Supported platforms: Windows, Linux, macOS

## Future Plans

- v1.0.0 will introduce API stability guarantees.
- ABI stability is not planned before v2.0.0.

---

*Last updated: v0.4.0*