#!/usr/bin/env bash
# Install test - verify that installed CoreBrick-C can be found via find_package
set -euo pipefail

INSTALL_PREFIX="${1:-}"

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT_DIR"

INSTALL_ROOT="${INSTALL_PREFIX:-$ROOT_DIR/build_install_root}"
INSTALL_TEST_DIR="$ROOT_DIR/build_install_test"

echo "=== Install Test ==="

if [ -z "$INSTALL_PREFIX" ]; then
    # Step 1: Configure and build CoreBrick-C
    echo "Configuring CoreBrick-C..."
    cmake -S . -B build_install -DCMAKE_INSTALL_PREFIX="$INSTALL_ROOT" -DCMAKE_BUILD_TYPE=Release

    echo "Building CoreBrick-C..."
    cmake --build build_install

    # Step 2: Install
    echo "Installing CoreBrick-C..."
    cmake --install build_install
else
    echo "Using pre-installed prefix: $INSTALL_ROOT"
fi

# Step 3: Verify install layout
echo "Verifying install layout..."
REQUIRED_PATHS=(
    "include/corebrick.h"
    "include/corebrick_version.h"
)

if [ -d "$INSTALL_ROOT/share/cmake" ]; then
    REQUIRED_PATHS+=("share/cmake/CoreBrick/CoreBrickConfig.cmake")
    REQUIRED_PATHS+=("share/cmake/CoreBrick/CoreBrickConfigVersion.cmake")
    REQUIRED_PATHS+=("share/cmake/CoreBrick/CoreBrickTargets.cmake")
fi
if [ -d "$INSTALL_ROOT/lib/cmake" ]; then
    REQUIRED_PATHS+=("lib/cmake/CoreBrick/CoreBrickConfig.cmake")
    REQUIRED_PATHS+=("lib/cmake/CoreBrick/CoreBrickConfigVersion.cmake")
    REQUIRED_PATHS+=("lib/cmake/CoreBrick/CoreBrickTargets.cmake")
fi

for rel_path in "${REQUIRED_PATHS[@]}"; do
    full_path="$INSTALL_ROOT/$rel_path"
    if [ ! -f "$full_path" ]; then
        echo "FAIL: missing $rel_path"
        exit 1
    fi
    echo "  Found: $rel_path"
done

# Step 4: Build the install test project
echo "Configuring install test..."
cmake -S tests/cmake_install_test -B "$INSTALL_TEST_DIR" -DCMAKE_PREFIX_PATH="$INSTALL_ROOT"
echo "Building install test..."
cmake --build "$INSTALL_TEST_DIR"

# Step 5: Run the install test
echo "Running install test..."
"$INSTALL_TEST_DIR/test_install"
echo "=== Install Test PASSED ==="