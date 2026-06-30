## Summary

This patch release source package contents, hardens install_test for Windows and Ubuntu, and clarifies audit traceability.

## Tasks

### Task 1: Version Docs

- Aligned VERSION, CMake project version, and corebrick_version.h
- Updated README project positioning
- Updated docs/release_notes.md, docs/api_reference.md with correct version

### Task 2: Source Package

- Updated package_release.ps1 to include cmake/ directory
- Updated package_release.ps1 to exclude test executables and intermediate files
- Updated .gitignore patterns for release artifacts

### Task 3: Install Test

- Fixed install_test.ps1 path checks for CMake package files
- Added install_test.sh for Ubuntu validation
- Verified cmake_install_test external project builds correctly

### Task 4: CI Install Test

- Added CI job for install validation on Windows
- Added CI job for install validation on Ubuntu
- Both jobs verify find_package(CoreBrick CONFIG) works after install

### Task 5: Audit Cleanup

- Fixed AUDIT_INDEX.md v0.4.1 Issue #46 for Task 1
- Filled v0.4.2 task traceability with real Issue/PR/Commit entries

### Task 6: v0.4.2 Release

- Version bumped to 0.4.2
- Full build and test validation
- Release asset created and uploaded

## Audit Trail

| Task | Issue | PR | Branch | Commit | Status |
|---|---|---|---|---|---|
| | Task 1 | #62 | #63 | task-1-align-version-docs-v0.4.2 | f204e0c | MERGED |
| | Task 2 | #64 | #65 | task-2-source-release-package | 5267a5a | MERGED |
| | Task 3 | #66 | #67 | task-3-harden-install-test | 2e6dac0 | MERGED |
| | Task 4 | #68 | #69 | task-4-fix-ci-install-test | adb4823 | MERGED |
| | Task 5 | #70 | #71 | task-5-cleanup-audit | 80b057d | MERGED |
| | Task 6 | #72 | TBD | task-6-release-v0.4.2 | TBD | PENDING |