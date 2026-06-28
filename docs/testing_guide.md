# Testing Guide

## Running Tests

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Or run the test executable directly:

```powershell
build\Debug\corebrick_tests.exe
```

## Test Structure

- `test_main.c` - Test runner with error module tests
- `test_arena.c` - Arena allocator tests
- `test_buffer.c` - Byte buffer tests
- `test_string_view.c` - String view tests
- `test_bitset.c` - BitSet tests
- `test_ring.c` - RingBuffer tests
- `test_bloom.c` - Bloom Filter tests
- `test_string_map.c` - StringMap tests
- `test_edge_cases.c` - Edge case and cross-module tests

## Adding Tests

1. Add test functions to the appropriate test file
2. Register the test call in `test_main.c`
3. Rebuild and run tests

## Writing Tests

Tests use a simple macro-based framework:

```c
extern int tests_passed;
extern int tests_failed;

#define TEST(name, expr) do { \
    if (!(expr)) { \
        printf("  FAIL: %s (%s:%d)\n", name, __FILE__, __LINE__); \
        tests_failed++; \
    } else { \
        printf("  PASS: %s\n", name); \
        tests_passed++; \
    } \
} while (0)
```