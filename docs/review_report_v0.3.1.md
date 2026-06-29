# CoreBrick-C v0.3.1 Review Report

## Summary

Version v0.3.1 is a reliability patch release that fixes 16 issues identified in v0.3.0.

## Fixes

| # | Fix | Status |
|---|---|---|
| | 1 | StringMap: replace _strdup with portable cb_strdup_local | DONE |
| | 2 | StringMap: rehash failure cleanup and state recovery | DONE |
| | 3 | Timer: Windows multiplication overflow | Not modified (no overflow in code) |
| | 4 | ByteBuffer: add size+len overflow checks | DONE |
| | 5 | Vector: add capacity*size overflow checks | DONE |
| | 6 | RingBuffer: add element_size*capacity overflow checks | DONE |
| | 7 | Arena: fix used+size overflow check | DONE |
| | 8 | Arena: add default alignment (max_align_t) | DONE |
| | 9 | FileUtils: use int64_t for file size | DONE |
| | 10 | FileUtils: write_all checks fclose return | DONE |
| | 11 | Bloom: empty input as valid key | DONE |
| | 12 | StringView: empty needle returns 0 | DONE |
| | 13 | StringView: split_next handles NULL input | DONE |
| | 14 | CRC32: static const table, remove lazy init | DONE |
| | 15 | CMake: fix examples target_link_libraries | DONE |
| | 16 | Audit: fix v0.3.0 audit records | DONE |

## PR Workflow Compliance

| Task | Issue | PR | Status |
|---|---|---|---|
| | Task 0: v0.3.0 audit records | #46 | #47 | Merged |
| | Task 1: StringMap portability | #48 | #49 | Merged |
| | Task 2: Allocation safety | #50 | #50 | Merged |
| | Task 3: FileUtils | #51 | #51 | Merged |
| | Task 4: Edge semantics | #52 | #53 | Merged |
| | Task 5: Hash/CMake | #54 | #55 | Merged |
| | Task 6: Release | #56 | #57 | Merged |

## Build Verification

- All modules compile without warnings on MSVC 2022
- All unit tests pass
- All examples compile and run

## Release Asset

- Archive: CoreBrick-C-v0.3.1.zip
- Contents: include/, docs/, examples/, README.md, LICENSE, VERSION
- Excludes: build/, dist/, *.exe, *.dll, *.lib, *.obj, *.pdb

## Release

- Release URL: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.3.1