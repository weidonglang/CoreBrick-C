cmake_minimum_required(VERSION 3.15)
project(corebrick_embed_minimal LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Requires CoreBrick-C to be installed first.
# Point CMAKE_PREFIX_PATH to the install root:
#   Windows: cmake -S . -B build -DCMAKE_PREFIX_PATH="path/to/install/root"
#   Ubuntu:  cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/install/root
find_package(CoreBrick CONFIG REQUIRED)

add_executable(corebrick_embed_demo main.c)
target_link_libraries(corebrick_embed_demo PRIVATE CoreBrick::corebrick)