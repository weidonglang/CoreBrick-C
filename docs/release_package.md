# Release Package Guide

This document describes how to build, package, and publish a CoreBrick-C release.

## Prerequisites

- CMake 3.15+
- Visual Studio 2022 (Windows) or GCC/Clang (Linux)
- PowerShell 7+ (Windows) or Bash (Linux)
- GitHub CLI (`gh`) — for creating releases

## Quick Start

```powershell
# Full release pipeline (Windows)
powershell -ExecutionPolicy Bypass -File scripts/build_release.ps1
powershell -ExecutionPolicy Bypass -File scripts/package_release.ps1
```

## Step-by-Step

### 1. Verify Version Consistency

Ensure the `VERSION` file and `CMakeLists.txt` agree:

```powershell
powershell -ExecutionPolicy Bypass -File scripts/check_version.ps1
```

Expected output:
```
OK: VERSION = 0.4.3 (matches CMakeLists.txt)
```

### 2. Run All Tests

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

All tests must pass before proceeding.

### 3. Run Smoke Test

```powershell
powershell -ExecutionPolicy Bypass -File scripts/smoke_test.ps1
```

### 4. Check No Artifacts in Repository

```powershell
powershell -ExecutionPolicy Bypass -File scripts/check_no_artifacts.ps1
```

Expected output:
```
OK: No artifacts detected in repository.
```

### 5. Build Release Configuration

```powershell
powershell -ExecutionPolicy Bypass -File scripts/build_release.ps1
```

This script:
1. Removes the existing `build/` directory
2. Runs `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`
3. Runs `cmake --build build --config Release`

### 6. Package Release Archive

```powershell
powershell -ExecutionPolicy Bypass -File scripts/package_release.ps1
```

This creates `dist/CoreBrick-C-v<version>.zip` containing:

```
CMakeLists.txt
README.md
LICENSE
VERSION
cmake/
include/
docs/
examples/
scripts/
```

**Archive does NOT include:**
- `build/` — build artifacts
- `dist/` — distribution directory
- `*.exe`, `*.dll`, `*.lib`, `*.obj`, `*.pdb` — compiled binaries
- `*.zip` — nested archives
- `*.log` — log files
- `.vscode/`, `.idea/` — IDE settings

### 7. Create GitHub Release

```powershell
# Create tag and release
$version = (Get-Content VERSION -Raw).Trim()
$zipPath = "dist/CoreBrick-C-v$version.zip"

# Create release with notes
gh release create "v$version" `
  --title "CoreBrick-C v$version" `
  --notes "See docs/release_notes.md for details." `
  "$zipPath"
```

### 8. Verify Release

```powershell
gh release view "v$version"
gh release view "v$version" --json assets
```

## Full Release Script

For convenience, here is a combined script that runs all steps:

```powershell
# run_full_release.ps1 — WARNING: destructive, removes build/
$ErrorActionPreference = "Stop"

$rootDir = Split-Path -Parent $PSScriptRoot
Set-Location $rootDir

Write-Host "=== Full Release Pipeline ===" -ForegroundColor Cyan

# 1. Version check
powershell -ExecutionPolicy Bypass -File scripts/check_version.ps1
if ($LASTEXITCODE -ne 0) { throw "Version check failed" }

# 2. Build and test
cmake -S . -B build_test
cmake --build build_test
ctest --test-dir build_test --output-on-failure
if ($LASTEXITCODE -ne 0) { throw "Tests failed" }

# 3. Smoke test
powershell -ExecutionPolicy Bypass -File scripts/smoke_test.ps1
if ($LASTEXITCODE -ne 0) { throw "Smoke test failed" }

# 4. Artifact check
powershell -ExecutionPolicy Bypass -File scripts/check_no_artifacts.ps1
if ($LASTEXITCODE -ne 0) { throw "Artifact check failed" }

# 5. Build release
powershell -ExecutionPolicy Bypass -File scripts/build_release.ps1
if ($LASTEXITCODE -ne 0) { throw "Release build failed" }

# 6. Package
powershell -ExecutionPolicy Bypass -File scripts/package_release.ps1
if ($LASTEXITCODE -ne 0) { throw "Packaging failed" }

Write-Host "=== Release package ready ===" -ForegroundColor Green
```

## Release Archive Contents Explained

| Path | Description |
|---|---|
| `CMakeLists.txt` | Top-level CMake build file |
| `README.md` | Project overview |
| `LICENSE` | MIT license |
| `VERSION` | Version file (plain text) |
| `cmake/` | CMake config and install targets |
| `include/` | Public C headers |
| `docs/` | Documentation |
| `examples/` | Example programs |
| `scripts/` | Utility scripts |

## Dependency-Free Distribution

The release archive is fully self-contained. Users can build from source using:

```powershell
# Extract archive
Expand-Archive CoreBrick-C-v<version>.zip -DestinationPath .
cd CoreBrick-C-v<version>

# Build
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure

# Install (optional)
cmake --install build --prefix path/to/install
```

No external dependencies, no package manager required.