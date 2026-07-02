# Examples Guide

This guide describes each example program in CoreBrick-C, what it demonstrates, how to build and run it, and expected output.

## Overview

CoreBrick-C provides standalone example programs for each module, plus a `minimal_example` that demonstrates all modules in a single program. There is also an `embed_minimal_project/` directory showing how to use CoreBrick-C as an installed CMake package in an external project.

All examples are built automatically when you configure and build the project:

```powershell
cmake -S . -B build
cmake --build build
```

## Running Examples

After a successful build (assuming Debug configuration), run any example from the project root:

```powershell
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
build\Debug\minimal_example.exe
```

On Ubuntu (or other Unix-like systems):

```bash
./build/arena_example
./build/buffer_example
# ... etc
```

## Example Details

### 1. `arena_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/arena_example.c` |
| **Header** | `cb_arena.h` |
| **Module** | Arena allocator |

**What it demonstrates:**

- Creating an arena with `cb_arena_create`
- Allocating memory with `cb_arena_alloc`
- Using allocated memory as an int array
- Checking arena usage with `cb_arena_used` / `cb_arena_capacity`
- Destroying the arena with `cb_arena_destroy`

**Expected output:**

```
=== Arena Example ===
Values: 0 100 200 300 400 500 600 700 800 900
Used: 40, Capacity: 4096
```

---

### 2. `buffer_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/buffer_example.c` |
| **Header** | `cb_buffer.h` |
| **Module** | ByteBuffer |

**What it demonstrates:**

- Initializing a buffer with `cb_buffer_init`
- Appending a C string with `cb_buffer_append_cstr`
- Appending single bytes with `cb_buffer_append_u8`
- Appending multi-byte integers with `cb_buffer_append_u32_le`
- Checking buffer size and capacity
- Clearing the buffer with `cb_buffer_clear`
- Freeing the buffer with `cb_buffer_free`

**Expected output:**

```
=== Buffer Example ===
Buffer: Hello, C World!
Size: 18, Capacity: 32
LE bytes: 78 56 34 12
After clear - size: 0, capacity: 32
After free - data: 000000000000, size: 0, capacity: 0
```

---

### 3. `bitset_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/bitset_example.c` |
| **Header** | `cb_bitset.h` |
| **Module** | BitSet |

**What it demonstrates:**

- Initializing a bitset with `cb_bitset_init`
- Setting individual bits with `cb_bitset_set`
- Getting individual bits with `cb_bitset_get`
- Counting set bits with `cb_bitset_count_ones`
- Freeing the bitset with `cb_bitset_free`

**Expected output:**

```
=== BitSet Example ===
Bit 0: 1
Bit 10: 1
Bit 63: 1
Ones: 3
```

---

### 4. `ring_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/ring_example.c` |
| **Header** | `cb_ring.h` |
| **Module** | RingBuffer |

**What it demonstrates:**

- Initializing a ring buffer with `cb_ring_init`
- Pushing elements with `cb_ring_push`
- Checking if full with `cb_ring_is_full`
- Checking if empty with `cb_ring_is_empty`
- Popping elements with `cb_ring_pop`
- Freeing the ring buffer with `cb_ring_free`

**Expected output:**

```
=== RingBuffer Example ===
Pushed: 1
Pushed: 2
Pushed: 3
Pushed: 4
Full: yes
Popped: 1
Popped: 2
Popped: 3
Popped: 4
```

---

### 5. `bloom_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/bloom_example.c` |
| **Header** | `cb_bloom.h` |
| **Module** | Bloom Filter |

**What it demonstrates:**

- Initializing a bloom filter with `cb_bloom_init`
- Adding items with `cb_bloom_add`
- Checking membership with `cb_bloom_might_contain`
- Checking inserted count
- Freeing the bloom filter with `cb_bloom_free`

**Expected output:**

```
=== Bloom Filter Example ===
Added: apple
Added: banana
Added: cherry
Contains 'apple': maybe
Contains 'durian': no
Inserted count: 3
```

---

### 6. `string_map_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/string_map_example.c` |
| **Header** | `cb_string_map.h` |
| **Module** | StringMap |

**What it demonstrates:**

- Creating a string map with `cb_string_map_create`
- Putting key-value pairs with `cb_string_map_put`
- Getting values with `cb_string_map_get`
- Checking map size with `cb_string_map_size`
- Destroying the map with `cb_string_map_destroy`

**Expected output:**

```
=== StringMap Example ===
name: CoreBrick
Size: 3
```

---

### 7. `string_view_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/string_view_example.c` |
| **Header** | `cb_string_view.h` |
| **Module** | StringView |

**What it demonstrates:**

- Creating a string view from a C string with `cb_sv_from_cstr`
- Trimming ASCII whitespace with `cb_sv_trim_ascii`
- Checking prefix with `cb_sv_starts_with`
- Parsing integers with `cb_sv_to_i64`
- Parsing floats with `cb_sv_to_f64`
- Splitting by delimiter with `cb_sv_split` / `cb_sv_split_next`

**Expected output:**

```
=== StringView Example ===
Original: "  Hello, CoreBrick World!  "
Trimmed: "Hello, CoreBrick World!"
Starts with 'Hello': yes
Parsed i64: -42
Parsed f64: 3.141590
Split: "apple" "banana" "cherry"
```

---

### 8. `vector_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/vector_example.c` |
| **Header** | `cb_vector.h` |
| **Module** | Vector |

**What it demonstrates:**

- Initializing a vector with `cb_vector_init`
- Pushing elements with `cb_vector_push`
- Getting elements with `cb_vector_get`
- Popping elements with `cb_vector_pop`
- Freeing the vector with `cb_vector_free`

**Expected output:**

```
=== Vector Example ===
Vector size: 10, capacity: 16
Elements: 0 1 2 3 4 5 6 7 8 9
Popped: 9, new size: 9
```

---

### 9. `hash_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/hash_example.c` |
| **Header** | `cb_hash.h` |
| **Module** | Hash (FNV-1a, CRC32) |

**What it demonstrates:**

- Computing FNV-1a 64-bit hash with `cb_hash_fnv1a64`
- Computing FNV-1a 32-bit hash with `cb_hash_fnv1a32`
- Computing CRC-32 hash with `cb_crc32`

**Expected output:**

```
=== Hash Example ===
FNV-1a 64: 3cde5e748c1f27be
FNV-1a 32: 827a6426
CRC-32:    f21c9e4c
```

---

### 10. `file_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/file_example.c` |
| **Header** | `cb_file.h` |
| **Module** | FileUtils |

**What it demonstrates:**

- Writing a file with `cb_file_write_all`
- Checking file existence with `cb_file_exists`
- Getting file size with `cb_file_size`
- Reading a file with `cb_file_read_all`

**Expected output:**

```
=== File Example ===
Write: OK
Exists: yes
Size: 22 bytes
Read: Hello, CoreBrick File!
```

---

### 11. `timer_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/timer_example.c` |
| **Header** | `cb_timer.h` |
| **Module** | Timer |

**What it demonstrates:**

- Starting a timer with `cb_timer_start`
- Reading elapsed time in milliseconds with `cb_timer_elapsed_ms`

**Expected output (approximate):**

```
=== Timer Example ===
Sum: 499999500000
Elapsed: 4.532 ms
```

The elapsed time will vary depending on the CPU speed and system load.

---

### 12. `minimal_example`

| Aspect | Details |
|---|---|
| **Source** | `examples/minimal_example.c` |
| **Header** | `corebrick.h` (umbrella) |
| **Module** | All modules |

**What it demonstrates:**

All CoreBrick-C modules used in a single, compact example:

- Arena (allocate int, check usage)
- ByteBuffer (append string, u8, cstr, free)
- StringView (trim, find, split)
- Vector (push ints, get, pop)
- Hash (FNV-1a, CRC32)
- BitSet (set, get)
- RingBuffer (push, peek)
- Bloom Filter (add, check)
- StringMap (put, get)
- Timer (start, elapsed)
- FileUtils (write, size, read, cleanup)

**Expected output:**

```
=== CoreBrick-C Minimal Example ===

Version: 0.4.2

Arena: allocated int = 42
Arena used: 8 / 1024
Buffer: [Hello! World] (len=12, cap=16)
StringView trimmed: "Hello, World!" (len=13)
StringView find 'World' at pos=7
StringView split: "a" "b" "c"
Vector: [10, 20, 30] (size=3)
Hash FNV-1a(hello) = 13298651257414081766
Hash CRC32(hello)  = 0x3610A686
BitSet: bit 3 = 1
BitSet: bit 5 = 0
Ring: empty=0, full=0
Ring peek = 100
Bloom: key1 might_contain = 1
Bloom: key2 might_contain = 0 (probably 0)
StringMap: name = Alice
StringMap size = 2
Timer: small loop took 8.50 us
File write_all: OK
File size: 21 bytes
File read_all: [Hello from CoreBrick!]

=== All modules demonstrated successfully! ===
```

---

## Embed Minimal Project

The `examples/embed_minimal_project/` directory demonstrates how to use CoreBrick-C as an **installed CMake package** in an external project.

**Files:**

- `CMakeLists.txt` — External project build file using `find_package(CoreBrick CONFIG)`
- `main.c` — Simple program that uses arena, buffer, and string view
- `README.md` — Build instructions

**Build steps (after installing CoreBrick-C):**

```powershell
cd examples/embed_minimal_project
cmake -S . -B build -DCMAKE_PREFIX_PATH="/path/to/CoreBrick-C/build/install"
cmake --build build
build\Debug\embed_minimal_example.exe
```

## Building All Examples

All examples are defined in the root `CMakeLists.txt` and are built automatically. No additional configuration is needed.

To build only the examples (without running tests):

```powershell
cmake --build build --target examples
```

## Adding a New Example

To add a new example:

1. Create `examples/your_module_example.c`
2. Add to `CMakeLists.txt`:

```cmake
add_executable(your_module_example examples/your_module_example.c)
target_link_libraries(your_module_example PRIVATE corebrick_static)
```

3. Add a section to this guide describing the new example.