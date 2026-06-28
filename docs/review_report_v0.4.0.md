# CoreBrick-C v0.4.0 Review Report

## Summary

This release adds CMake package support, version header, API policy documentation, installation guide, and CI install validation.

## Tasks

### Task 1: CMake Install / Export / Package Config

- Added `CMakePackageConfigHelpers`-based install with `CoreBrickConfig.cmake` and version file
- Created `cmake/CoreBrickConfig.cmake.in` template with `find_dependency` support
- Added `CoreBrick::corebrick_static` IMPORTED target for external `find_package` consumers
- Added `install()` rules for library, headers, CMake config, and targets export
- Added `GNUInstallDirs` integration for platform-appropriate install paths

### Task 2: Version Header / API Policy

- Added `include/corebrick_version.h` with `COREBRICK_VERSION_MAJOR`/`MINOR`/`PATCH`/`STRING` macros
- Added `docs/api_policy.md` documenting public API contract, deprecation policy, and versioning
- Embedded version string in all headers and library via compile-time generation

### Task 3: Minimal Example

- Added `examples/minimal_example.c` demonstrating minimal usage with all 11 modules
- Shows include, init, use, free pattern for each module

### Task 4: Install Test / CI Validation

- Added `tests/cmake_install_test/` as standalone CMake project for post-install verification
- Added `tests/test_install.c` using `find_package(CoreBrick CONFIG REQUIRED)` verifying all 11 modules are linkable
- Updated CI workflow with install test step

### Task 5: Release Package Contents

- Updated `docs/install.md` with build, install, and external project usage instructions
- Updated `docs/api_reference.md` with version API section
- Updated `README.md` with installation section
- Updated `AUDIT_INDEX.md` with v0.4.0 task traceability
- Updated `docs/release_notes.md` with v0.4.0 release notes

### Task 6: v0.4.0 Release

- Version bumped to 0.4.0
- Full build and test validation
- CI pipeline verification

## Audit Trail

| Task | Issue | PR | Branch | Commit | Status |
|---|---|---|---|---|---|
| Task 1 | #31 | #32 | task-1-cmake-install | d6727e7 | MERGED |
| Task 2 | #33 | #34 | task-2-version-policy | 82db565 | MERGED |
| Task 3 | #35 | #36 | task-3-minimal-example | 84a2766 | MERGED |
| Task 4 | #37 | #38 | task-4-install-test-ci | TBD | PENDING |
| Task 5 | #39 | #40 | task-5-release-package | TBD | PENDING |
| Task 6 | #41 | #42 | task-6-release-v0.4.0 | TBD | PENDING |