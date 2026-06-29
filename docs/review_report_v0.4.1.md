# CoreBrick-C v0.4.1 Review Report

## Summary

This patch release fixes the exported CMake target name to match `find_package` documentation.

## Tasks

### Task 1: Fix CMake Target Name

- Changed exported target name from `CoreBrick::corebrick_static` to `CoreBrick::corebrick` in `set_target_properties(EXPORT_NAME)`
- All examples and tests already use `corebrick_static` which works correctly as the library target
- External `find_package(CoreBrick CONFIG REQUIRED)` consumers now use `target_link_libraries(... CoreBrick::corebrick)`

### Task 2: Fix Install Test Target

- Updated `tests/cmake_install_test/CMakeLists.txt` to use `CoreBrick::corebrick` instead of `CoreBrick::corebrick_static`
- Updated `tests/cmake_install_test/test_install.c` to use `CoreBrick::corebrick` instead of `CoreBrick::corebrick_static`

### Task 3: v0.4.1 Release

- Version bumped to 0.4.1
- Full build and test validation
- Install test validation

## Audit Trail

| Task | Issue | PR | Branch | Commit | Status |
|---|---|---|---|---|---|
| | Task 1 | #38 | #58 | task-1-fix-cmake-target | 65a9804 | MERGED |
| | Task 2 | #38 | #59 | task-2-fix-install-test-target | a8cfe72 | MERGED |
| | Task 3 | #60 | #61 | task-6-release-v0.4.1 | 205ac9c | MERGED |
