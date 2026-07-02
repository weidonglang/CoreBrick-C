# Security Policy

## Supported Versions

CoreBrick-C follows [semantic versioning](https://semver.org/). Only the latest minor release of the current major version receives security updates.

| Version | Supported          |
|---------|--------------------|
| 0.4.x   | :white_check_mark: |
| < 0.4   | :x:                |

## Reporting a Vulnerability

We take security seriously. If you discover a security vulnerability in CoreBrick-C, please report it privately.

**Do not report security vulnerabilities via public GitHub issues.**

Instead, please send a detailed report to the maintainers via one of the following methods:

1. **GitHub Security Advisory**: Navigate to the repository's [Security Advisories](https://github.com/weidonglang/CoreBrick-C/security/advisories) page and create a new advisory.
2. **Email**: Contact the maintainers directly (see `CONTRIBUTING.md` for contact information).

### What to Include

Please include the following information in your report:

- Type of vulnerability (e.g., buffer overflow, integer overflow, denial of service)
- Affected module(s) and function(s)
- Steps to reproduce the issue
- Potential impact
- Suggested fix (if available)
- Test case or proof of concept (if available)

### Response Timeline

- **Acknowledgment**: Within 3 business days
- **Initial assessment**: Within 5 business days
- **Fix development**: Timeline depends on severity and complexity
- **Release**: Critical fixes are prioritized for the next patch release

## Security Considerations

### Integer Overflow Protection

CoreBrick-C provides overflow-safe arithmetic in allocation paths:

- `cb_arena_alloc` checks `used + size` overflow before allocation
- `cb_buffer_append` and `cb_buffer_reserve` check `size + len` and `capacity * 2` overflow
- `cb_vector_init` and `cb_vector_push` check `element_size * capacity` overflow
- `cb_ring_init` checks `element_size * capacity` overflow

### Memory Safety

- All allocations are bounds-checked
- Arena allocator returns properly aligned addresses
- StringMap uses internal `cb_strdup_local` instead of platform-specific `_strdup`
- ByteBuffer operations validate capacity before growth

### Thread Safety

CoreBrick-C is currently designed for **single-threaded** use. The following are **not** thread-safe:

- Concurrent reads/writes to any data structure
- Lazy initialization of global state (e.g., CRC32 tables)
- Arena allocation from multiple threads

If you need thread-safe access, you must provide external synchronization (e.g., mutex).

### File I/O Safety

- `cb_file_read_all` uses `int64_t` for file sizes and validates against `SIZE_MAX`
- `cb_file_write_all` checks `fclose` return value
- Invalid paths and missing files are handled gracefully

## Security-Relevant API Behavior

| Function | Security Consideration |
|---|---|
| `cb_arena_alloc` | Returns NULL on overflow or capacity exceeded |
| `cb_buffer_append` | Returns `CB_ERR_OUT_OF_MEMORY` on overflow |
| `cb_vector_push` | Returns `CB_ERR_OUT_OF_MEMORY` on overflow |
| `cb_ring_init` | Returns `CB_ERR_OUT_OF_MEMORY` on overflow |
| `cb_string_map_put` | Handles NULL keys/values as empty strings |
| `cb_bloom_add` | Accepts empty keys (len=0) as valid input |
| `cb_sv_find` | Returns 0 for empty needle |
| `cb_sv_split_next` | Returns 0 for NULL input |
| `cb_file_read_all` | Validates file size against `SIZE_MAX` |
| `cb_file_write_all` | Checks `fclose` return value |

## Dependencies

CoreBrick-C has **zero external dependencies**. It uses only the C standard library, which minimizes supply chain risk.

## Build Security

- CMake build system with no external downloads
- No preprocessor macros that disable security checks
- All warnings enabled by default (`-Wall -Wextra` on GCC/Clang, `/W4` on MSVC)
- Static analysis and sanitizer builds available via CMake options

## Version History

See [CHANGELOG.md](CHANGELOG.md) for security-related fixes in each release.