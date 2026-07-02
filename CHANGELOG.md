# Changelog

All notable changes to CoreBrick-C are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Issue templates: bug report, feature request, task (.github/ISSUE_TEMPLATE/)
- PR template (.github/PULL_REQUEST_TEMPLATE.md)
- SECURITY.md with vulnerability reporting policy
- CONTRIBUTING.md with coding standards and workflow
- Module capability matrix (docs/module_matrix.md)
- Examples guide (docs/examples_guide.md)
- CI guide (docs/ci_guide.md)
- Release package guide (docs/release_package.md)

### Changed

- README hero section with project badges and tagline
- README project positioning for clarity

## [0.4.2] - 2026-06-30

### Packaging & Audit Cleanup Patch

- Aligned VERSION, CMake project version, and `corebrick_version.h`
- Updated README project positioning
- Improved release source package contents
- Fixed install_test path checks for CMake package files
- Cleaned audit traceability records

## [0.4.1] - 2026-06-29

### Integration Completion Patch

- Fixed exported CMake target name to `CoreBrick::corebrick`
- Updated install test to use `CoreBrick::corebrick`
- Finalized v0.4.1 audit records

## [0.4.0] - 2026-06-28

### CMake Package Support & Installation

- Added `CMakePackageConfigHelpers`-based install with `CoreBrickConfig.cmake` and version file
- Created `cmake/CoreBrickConfig.cmake.in` template with `find_dependency` support
- Added `CoreBrick::corebrick_static` IMPORTED target for external `find_package` consumers
- Added `install()` rules for library, headers, CMake config, and targets export
- Added `GNUInstallDirs` integration for platform-appropriate install paths
- Added `include/corebrick_version.h` with version macros
- Added `docs/api_policy.md` documenting public API contract, deprecation policy, and versioning
- Added `docs/install.md` with build, install, and external project usage instructions
- Updated `docs/api_reference.md` with version API section
- Updated `README.md` with installation section
- Added `install_test.c` using `find_package(CoreBrick CONFIG REQUIRED)`
- Added CI install test step
- Created `tests/cmake_install_test/` as standalone CMake project
- Added `minimal_example.c` demonstrating minimal usage

## [0.3.3] - 2026-06-25

### Reliability & CI Fix

- Restored `check_version.ps1` with proper VERSION file validation
- Restored `smoke_test.ps1` with updated test name and full validation
- Restored `test_edge_cases.c` with edge case tests for all modules
- Fixed CI Windows `ctest` configuration by adding `-C Debug` flag

## [0.3.2] - 2026-06-24

### Reliability Fix

- Fixed Windows QPC multiplication overflow in `cb_timer_now_ns()`
- Changed to divide-first-then-multiply pattern

## [0.3.1] - 2026-06-20

### Reliability Fixes

- Replaced `_strdup` with portable `cb_strdup_local` (strlen + malloc + memcpy)
- Rewrote `rehash` to use internal `insert_into` instead of public `cb_string_map_put`
- Fixed rehash failure path: frees partially copied entries, restores old table state
- Fixed `used + size` overflow check in Arena
- Added default `max_align_t` alignment for Arena allocations
- Added integer overflow checks for ByteBuffer, Vector, RingBuffer
- Replaced `long` with `int64_t` for file size in FileUtils
- `cb_file_write_all` now checks `fclose` return value
- `cb_file_read_all` initializes `out_buffer` fresh
- Empty input now handled correctly in Bloom Filter
- `cb_sv_find` with empty needle returns 0 (not found)
- `cb_sv_split_next` handles NULL input safely
- Replaced lazy CRC32 table initialization with static const table
- Fixed examples `target_link_libraries` alignment in CMake

## [0.3.0] - 2026-06-15

### New Modules

- **BitSet**: Fixed-size bit array with set, clear, fill, count_ones operations
- **RingBuffer**: Fixed-capacity FIFO ring buffer for fixed-size elements
- **Bloom Filter**: Probabilistic set membership filter using double hashing
- **StringMap**: String-to-string hash table with open addressing and linear probing

### Process Improvement

- Strict PR workflow: each Task = 1 Issue + 1 Branch + 1 Commit + 1 PR
- AUDIT_INDEX.md tracks all Issue/PR/Commit relationships

## [0.2.0] - 2026-06-01

### New Modules

- **Vector**: Generic dynamic array with push, pop, get, set, get_ptr
- **Hash**: Non-cryptographic hash utilities (FNV-1a, CRC-32)
- **FileUtils**: File existence, size, read/write utilities
- **Timer**: Cross-platform high-resolution timer
- **Benchmark**: Performance measurement suite

## [0.1.0] - 2026-05-15

### Initial Release

- **Arena**: Linear memory allocator
- **ByteBuffer**: Dynamic byte buffer
- **StringView**: Zero-copy string slice

[Unreleased]: https://github.com/weidonglang/CoreBrick-C/compare/v0.4.2...HEAD
[0.4.2]: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.4.2
[0.4.1]: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.4.1
[0.4.0]: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.4.0
[0.3.3]: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.3.3
[0.3.2]: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.3.2
[0.3.1]: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.3.1
[0.3.0]: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.3.0
[0.2.0]: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.2.0
[0.1.0]: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.1.0