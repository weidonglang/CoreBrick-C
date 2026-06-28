cmake_minimum_required(VERSION 3.15)
project(install_test C)

set(CMAKE_C_STANDARD 11)

# Find installed CoreBrick
find_package(CoreBrick CONFIG REQUIRED)

add_executable(test_install test_install.c)
target_link_libraries(test_install PRIVATE CoreBrick::corebrick_static)
