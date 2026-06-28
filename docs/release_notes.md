# Release Notes

## v0.3.1

### Reliability Fixes

This patch release fixes 15 issues across all modules:

#### StringMap
- Replaced `_strdup` with portable `cb_strdup_local` (strlen + malloc + memcpy)
- Rewrote `rehash` to use internal `insert_into` instead of public `cb_string_map_put`
- Fixed rehash failure path: frees partially copied entries, restores old table state

#### Arena
- Fixed `used + size` overflow check: `size > capacity - used`
- Added default `max_align_t` alignment for all allocations

#### ByteBuffer / Vector / RingBuffer
- Added integer overflow checks for `size + len`, `capacity * 2`, `element_size * capacity`

#### FileUtils
- Replaced `long` with `int64_t` for file size (supports large files)
- `cb_file_write_all` now checks `fclose` return value
- `cb_file_read_all` initializes `out_buffer` fresh

#### Bloom Filter
- Empty input (`NULL, 0` and `"", 0`) now adds a valid key with inserted bits

#### StringView
- `cb_sv_find` with empty needle now returns 0 (not found)
- `cb_sv_split_next` handles NULL input safely

#### Hash
- Replaced lazy CRC32 table initialization with static const table (thread-safe)

#### CMake
- Fixed examples `target_link_libraries` alignment

### v0.3.0 Modules (carried forward)

- BitSet, RingBuffer, Bloom Filter, StringMap
- Vector, Hash, FileUtils, Timer, Benchmark
- Arena, ByteBuffer, StringView

### Known Limitations

- No SIMD optimizations
- No thread-safe variants
- No language bindings
- No JSON parser
- No lock-free queue
- Bloom Filter has false positives (documented)

## v0.3.0

### New Modules

- **BitSet**: Fixed-size bit array with set, clear, fill, count_ones operations. Used for state flags, Bloom Filter backend, and compressed boolean arrays.
- **RingBuffer**: Fixed-capacity FIFO ring buffer for fixed-size elements. Supports push, pop, peek, clear. Single-thread only, no auto-expansion, no overwrite-on-full.
- **Bloom Filter**: Probabilistic set membership filter using double hashing (FNV-1a64 + CRC-32). Supports add, might_contain, clear. Note: has false positives, no false negatives for inserted items.
- **StringMap**: String-to-string hash table with open addressing and linear probing. Supports put, get, contains, remove, clear. Automatic rehashing when load factor exceeds 0.7. Tombstone-based deletion.

### PR Workflow (Process Improvement)

Starting from v0.3.0, all modules follow a strict audit trail:
- Each Task = 1 Issue + 1 Branch + 1 Commit + 1 PR
- Every PR is created, reviewed, and squash-merged
- AUDIT_INDEX.md tracks all Issue/PR/Commit relationships

### v0.2.0 Modules (carried forward)

- **Vector**: Generic dynamic array
- **Hash**: Non-cryptographic hash utilities (FNV-1a, CRC-32)
- **FileUtils**: File read/write/size/exists utilities
- **Timer**: Cross-platform high-resolution timer
- **Benchmark**: Performance measurement suite

### v0.1.0 Modules (carried forward)

- **Arena**: Linear memory allocator
- **ByteBuffer**: Dynamic byte buffer
- **StringView**: Zero-copy string slice

### Known Limitations

- No SIMD optimizations
- No thread-safe variants
- No language bindings
- No JSON parser
- No lock-free queue
- Bloom Filter has false positives (documented)

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