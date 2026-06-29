# CoreBrick-C Embed Minimal Project

This directory contains a minimal external CMake project that demonstrates how to use CoreBrick-C via `find_package(CoreBrick CONFIG REQUIRED)`.

## Prerequisites

CoreBrick-C must be installed first. See the [Installation Guide](../../docs/install.md) for details.

## Usage

### Windows (PowerShell)

```powershell
# Install CoreBrick-C (from the repo root)
cmake -S . -B build-install -DCMAKE_INSTALL_PREFIX="$pwd/build-install-root"
cmake --build build-install --config Debug
cmake --install build-install --config Debug

# Build and run this embedded project
cmake -S examples/embed_minimal_project -B build-embed -DCMAKE_PREFIX_PATH="$pwd/build-install-root"
cmake --build build-embed --config Debug
.\build-embed\Debug\corebrick_embed_demo.exe
```

### Ubuntu (Bash)

```bash
# Install CoreBrick-C (from the repo root)
cmake -S . -B build-install -DCMAKE_INSTALL_PREFIX=$PWD/build-install-root
cmake --build build-install
cmake --install build-install

# Build and run this embedded project
cmake -S examples/embed_minimal_project -B build-embed -DCMAKE_PREFIX_PATH=$PWD/build-install-root
cmake --build build-embed
./build-embed/corebrick_embed_demo
```

## Expected Output

```
CoreBrick embed demo: 5 passed, 0 failed
CoreBrick embed demo: OK
```

## Modules Demonstrated

- Arena
- ByteBuffer
- StringView
- Hash (FNV-1a, CRC32)
- StringMap