# CoreBrick-C Installation Guide

## Overview

CoreBrick-C supports standard CMake install and export, allowing other C/C++ projects to use it via `find_package(CoreBrick CONFIG REQUIRED)`.

## Build from Source

```powershell
# Windows (PowerShell)
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=build/install
cmake --build build --config Debug
cmake --install build --config Debug
```

```bash
# Ubuntu / Linux
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=build/install
cmake --build build
cmake --install build
```

## Verify Installation

After installation, the following files must exist:

```
build/install/include/corebrick.h
build/install/lib/cmake/CoreBrick/CoreBrickConfig.cmake
build/install/lib/cmake/CoreBrick/CoreBrickTargets.cmake
```

## Using CoreBrick-C in External Projects

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.15)
project(my_app LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(CoreBrick CONFIG REQUIRED)

add_executable(my_app main.c)
target_link_libraries(my_app PRIVATE CoreBrick::corebrick)
```

### Windows (PowerShell)

```powershell
# Assuming CoreBrick-C is installed at C:/libs/CoreBrick-C/build/install
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/libs/CoreBrick-C/build/install"
cmake --build build --config Debug
.\build\Debug\my_app.exe
```

### Ubuntu (Bash)

```bash
# Assuming CoreBrick-C is installed at ~/libs/CoreBrick-C/build/install
cmake -S . -B build -DCMAKE_PREFIX_PATH=~/libs/CoreBrick-C/build/install
cmake --build build
./build/my_app
```

## Finding the Installed Package

CMake uses `CMAKE_PREFIX_PATH` to locate installed packages. Point it to the root of the installation directory (the one containing `include/`, `lib/`, etc.).

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="path/to/install/root"
```

## Supported Platforms

- Windows (MSVC, Clang, GCC/MinGW)
- Ubuntu / Linux (GCC, Clang)
- macOS (Clang, GCC)