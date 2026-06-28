cmake_minimum_required(VERSION 3.15)
project(corebrick VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Sources
set(CB_SOURCES
    src/cb_error.c
    src/cb_arena.c
    src/cb_buffer.c
    src/cb_string_view.c
    src/cb_vector.c
src/cb_hash.c
)

set(CB_HEADERS
    include/corebrick.h
    include/cb_error.h
    include/cb_arena.h
    include/cb_buffer.h
    include/cb_string_view.h
    include/cb_vector.h
include/cb_hash.h
)

# Static library
add_library(corebrick_static STATIC ${CB_SOURCES} ${CB_HEADERS})
target_include_directories(corebrick_static PUBLIC include)

# CLI
add_executable(cbcli cli/cbcli.c)
target_link_libraries(cbcli PRIVATE corebrick_static)

# Tests
enable_testing()

add_executable(corebrick_tests
    tests/test_main.c
    tests/test_arena.c
    tests/test_buffer.c
    tests/test_string_view.c
tests/test_hash.c
    tests/test_vector.c
    tests/test_edge_cases.c
)
target_include_directories(corebrick_tests PRIVATE include)
target_link_libraries(corebrick_tests PRIVATE corebrick_static)

add_test(NAME corebrick_tests COMMAND corebrick_tests)

# Examples
add_executable(arena_example examples/arena_example.c)
target_link_libraries(arena_example PRIVATE corebrick_static)

add_executable(buffer_example examples/buffer_example.c)
target_link_libraries(buffer_example PRIVATE corebrick_static)

add_executable(string_view_example examples/string_view_example.c)
target_link_libraries(string_view_example PRIVATE corebrick_static)

add_executable(vector_example examples/vector_example.c)
target_link_libraries(vector_example PRIVATE corebrick_static)
add_executable(hash_example examples/hash_example.c)
target_link_libraries(hash_example PRIVATE corebrick_static)