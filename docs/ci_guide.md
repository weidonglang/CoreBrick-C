# CI Guide

This document describes the Continuous Integration (CI) setup for CoreBrick-C, including the GitHub Actions workflow, available scripts, and how to run them locally.

## Overview

CoreBrick-C uses **GitHub Actions** for CI. The workflow is defined in `.github/workflows/ci.yml` and runs on every push to `main` and every pull request targeting `main`.

The CI pipeline covers:

| Step | Windows | Ubuntu |
|---|---|---|
| Configure CMake | ✅ | ✅ |
| Build (Debug) | ✅ | ✅ |
| Run tests (ctest) | ✅ | ✅ |
| Install via CMake | ✅ | ✅ |
| Install validation test | ✅ (PowerShell) | ✅ (Bash) |
| Version consistency check | ✅ | — |
| Artifact detection check | ✅ | — |
| Smoke test | ✅ | — |

## GitHub Actions Workflow

**File:** `.github/workflows/ci.yml`

**Trigger:**
- `push` on `main`
- `pull_request` on `main`

**Strategy matrix:**
- `windows-latest`
- `ubuntu-latest`

### Pipeline Steps

1. **Checkout** — Clone the repository via `actions/checkout@v4`
2. **Configure CMake** — `cmake -S . -B build` (`-A x64` on Windows)
3. **Build** — `cmake --build build` (`--config Debug` on Windows)
4. **Test** — `ctest --test-dir build -C Debug --output-on-failure`
5. **Install** — `cmake --install build --prefix <temp>/corebrick-install`
6. **Install Test (cmake_install_test)** — Ubuntu: build and run `tests/cmake_install_test`
7. **Install Test Script (Windows)** — runs `scripts/install_test.ps1`
8. **Install Test Script (Ubuntu)** — runs `scripts/install_test.sh`
9. **Windows Scripts** — runs `check_version.ps1`, `check_no_artifacts.ps1`, `smoke_test.ps1`

## Available Scripts

All scripts are located in the `scripts/` directory.

### 1. `run_tests.ps1`

Build and run all tests in one command.

```powershell
powershell -ExecutionPolicy Bypass -File scripts/run_tests.ps1
```

**Steps:**
1. `cmake -S . -B build`
2. `cmake --build build`
3. `ctest --test-dir build --output-on-failure`

---

### 2. `check_version.ps1`

Verify that the `VERSION` file matches the version declared in `CMakeLists.txt`.

```powershell
powershell -ExecutionPolicy Bypass -File scripts/check_version.ps1
```

**Checks:**
- `VERSION` file exists and is non-empty
- `CMakeLists.txt` contains a `project(corebrick VERSION x.y.z)` declaration
- Both versions are identical

**Exit codes:**
- `0` — OK (versions match)
- `1` — FAIL (version mismatch or parse error)

---

### 3. `check_no_artifacts.ps1`

Check that no build artifacts are tracked by git.

```powershell
powershell -ExecutionPolicy Bypass -File scripts/check_no_artifacts.ps1
```

**Detects tracked files matching patterns:**
- `*.exe`, `*.dll`, `*.lib`, `*.obj`, `*.pdb`, `*.zip`, `*.log`

**Detects tracked directories:**
- `build/`, `dist/`, `.vscode/`, `.idea/`

**Exit codes:**
- `0` — OK (no artifacts detected)
- `1` — FAIL (artifacts found tracked)

---

### 4. `smoke_test.ps1`

Comprehensive smoke test that verifies the build and basic functionality.

```powershell
powershell -ExecutionPolicy Bypass -File scripts/smoke_test.ps1
```

**Steps:**
1. Verify `VERSION` matches `CMakeLists.txt`
2. `cmake -S . -B build` and `cmake --build build`
3. Run `corebrick_tests.exe` (test executable)
4. Run `corebrick_bench.exe` if present (benchmark, info only)
5. Run `cbcli.exe version` if present (CLI tool)

**Exit codes:**
- `0` — All checks pass
- `1` — Any check fails

---

### 5. `install_test.ps1` (Windows)

Verify that the installed CoreBrick-C package can be found via CMake `find_package`.

```powershell
powershell -ExecutionPolicy Bypass -File scripts/install_test.ps1
```

Optionally specify a pre-installed prefix:

```powershell
powershell -ExecutionPolicy Bypass -File scripts/install_test.ps1 -InstallPrefix "C:\path\to\corebrick-install"
```

**Steps (without prefix):**
1. `cmake -S . -B build_install` with `CMAKE_INSTALL_PREFIX`
2. `cmake --build build_install --config Debug`
3. `cmake --install build_install --config Debug`
4. Verify install layout (headers, cmake config files)
5. Build `tests/cmake_install_test` against the installed package
6. Run `test_install.exe`

**Exit codes:**
- `0` — Install validated
- `1` — Any step fails

---

### 6. `install_test.sh` (Ubuntu / Linux)

Same as `install_test.ps1` but for Linux/bash environments.

```bash
chmod +x scripts/install_test.sh
./scripts/install_test.sh
```

Optionally specify a pre-installed prefix:

```bash
./scripts/install_test.sh /path/to/corebrick-install
```

---

### 7. `build_release.ps1`

Build the project in Release configuration from scratch.

```powershell
powershell -ExecutionPolicy Bypass -File scripts/build_release.ps1
```

**Steps:**
1. Remove existing `build/` directory
2. `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`
3. `cmake --build build --config Release`

---

### 8. `package_release.ps1`

Create a distribution zip archive.

```powershell
powershell -ExecutionPolicy Bypass -File scripts/package_release.ps1
```

**Produces:** `dist/CoreBrick-C-v<version>.zip`

**Archive contents:**
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

## Running CI Checks Locally

### Full validation (recommended before PR)

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
powershell -ExecutionPolicy Bypass -File scripts/check_version.ps1
powershell -ExecutionPolicy Bypass -File scripts/check_no_artifacts.ps1
powershell -ExecutionPolicy Bypass -File scripts/smoke_test.ps1
```

### Quick build + test

```powershell
powershell -ExecutionPolicy Bypass -File scripts/run_tests.ps1
```

### Verify install

```powershell
powershell -ExecutionPolicy Bypass -File scripts/install_test.ps1
```

### Build release + package

```powershell
powershell -ExecutionPolicy Bypass -File scripts/build_release.ps1
powershell -ExecutionPolicy Bypass -File scripts/package_release.ps1
```

## CI Status Badge

[![CI](https://github.com/weidonglang/CoreBrick-C/actions/workflows/ci.yml/badge.svg)](https://github.com/weidonglang/CoreBrick-C/actions/workflows/ci.yml)

Add to `README.md`:

```markdown
[![CI](https://github.com/weidonglang/CoreBrick-C/actions/workflows/ci.yml/badge.svg)](https://github.com/weidonglang/CoreBrick-C/actions/workflows/ci.yml)
```

## Troubleshooting

### Build fails on Windows

Ensure you are running from a **Developer Command Prompt for VS 2022** or have the MSVC environment initialized:

```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64
```

### ctest fails

Run tests directly to see detailed output:

```powershell
build\Debug\corebrick_tests.exe
```

### Install test fails with "CoreBrickConfig.cmake not found"

The install prefix must point to a directory that contains the correct layout. On Windows, cmake config files are installed to `lib/cmake/CoreBrick/`; on Ubuntu, to `share/cmake/CoreBrick/`.

### Git not clean

If `git status` shows untracked files from `build/`, `dist/`, etc., add them to `.gitignore` or remove them before running CI checks.

```powershell
git clean -fd