# CoreBrick-C Audit Index

## Repository

- Repository: https://github.com/weidonglang/CoreBrick-C
- Default branch: main
- Latest release: v0.4.1
- Latest tag: v0.4.1

## v0.1.0 Task Traceability

| Task | Module | Commit | Main Files | Tests | Docs | Status |
|---|---|---|---|---|---|---|---|
| | Task 0 | Bootstrap | 5fd5b89 | CMakeLists.txt, README.md | tests/test_main.c | docs/architecture.md | PASS |
| | Task 1 | Arena | 5fd5b89 | include/cb_arena.h, src/cb_arena.c | tests/test_arena.c | docs/arena.md | PASS |
| | Task 2 | ByteBuffer | 5fd5b89 | include/cb_buffer.h, src/cb_buffer.c | tests/test_buffer.c | docs/buffer.md | PASS |
| | Task 3 | StringView | 5fd5b89 | include/cb_string_view.h, src/cb_string_view.c | tests/test_string_view.c | docs/string_view.md | PASS |
| | Task 4 | Release | 5fd5b89 | README.md, docs/review_report_v0.1.0.md | smoke_test.ps1 | docs/release_notes.md | PASS |

## v0.2.0 Task Traceability

| Task | Module | Branch | Commit | Main Files | Tests | Docs | Status |
|---|---|---|---|---|---|---|---|---|
| | Task 1 | Vector | task-1-vector | f8215ee | include/cb_vector.h, src/cb_vector.c | tests/test_vector.c | docs/vector.md | PASS |
| | Task 2 | Hash | task-2-hash | 16555ab | include/cb_hash.h, src/cb_hash.c | tests/test_hash.c | docs/hash.md | PASS |
| | Task 3 | FileUtils | task-3-file-utils | 19bf3d2 | include/cb_file.h, src/cb_file.c | tests/test_file.c | docs/file.md | PASS |
| | Task 4 | Timer/Benchmark | task-4-timer-benchmark | e29c7a2 | include/cb_timer.h, src/cb_timer.c | tests/test_timer.c | docs/timer.md | PASS |
| | Task 5 | Release | task-5-release-v0.2.0 | c602213 | README.md, docs/review_report_v0.2.0.md | smoke_test.ps1 | docs/release_notes.md | PASS |

## Validation Commands

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
powershell -ExecutionPolicy Bypass -File scripts/check_version.ps1
powershell -ExecutionPolicy Bypass -File scripts/check_no_artifacts.ps1
powershell -ExecutionPolicy Bypass -File scripts/smoke_test.ps1
```

## v0.3.0 Task Traceability

| Task | Module | Issue | PR | Branch | Commit | Status |
|---|---|---|---|---|---|---|---|
| | Task 0 | PR Audit Guard | #1 | #2 | task-0-pr-audit-guard | 58a567a | MERGED |
| | Task 1 | BitSet | #3 | #4 | task-1-bitset | 3aaa0b0 | MERGED |
| | Task 2 | RingBuffer | #5 | #6 | task-2-ring-buffer | 2740fd4 | MERGED |
| | Task 3 | Bloom Filter | #7 | #8 | task-3-bloom-filter | aa43af8 | MERGED |
| | Task 4 | StringMap | #9 | #11 | task-4-string-map | 98d40e6 | MERGED |
| | Task 5 | Release | #12 | #13 | task-5-release-v0.3.0 | 4b651d2 | MERGED |

## Known Limitations

- v0.1.0 was delivered as a consolidated initial release.
- v0.2.0 used separate branches and commits per module, but GitHub pull requests were not created or are not available for audit.
- No language bindings.

## v0.3.1 Task Traceability

| Task | Module | Issue | PR | Branch | Commit | Status |
|---|---|---|---|---|---|---|
| | Task 0 | Fix Audit Records | #46 | #47 | task-0-fix-v0.3.0-audit | b72a476 | MERGED |
| | Task 1 | StringMap | #48 | #49 | task-1-fix-string-map | 21810d3 | MERGED |
| | Task 2 | Allocation Safety | #50 | #50 | task-2-fix-allocation-safety | d4355f2 | MERGED |
| | Task 3 | FileUtils | #51 | #51 | task-3-fix-file-utils | 1032012 | MERGED |
| | Task 4 | Bloom/StringView | #52 | #53 | task-4-fix-edge-semantics | 388aa08 | MERGED |
| | Task 5 | Hash/CMake | #54 | #55 | task-5-fix-hash-cmake | 7eebbb1 | MERGED |
| | Task 6 | Release | #60 | #60 | task-6-release-v0.3.1 | 4b651d2 | MERGED |

## v0.4.0 Task Traceability

| Task | Module | Issue | PR | Branch | Commit | Status |
|---|---|---|---|---|---|---|
| | Task 1 | Timer Overflow Fix | #56 | #56 | task-1-fix-timer-overflow | TBD | MERGED |
| | Task 2 | Install+CI+API Policy | #57 | #57 | task-2-version-api-policy | TBD | MERGED |
| | Task 3 | Embed Minimal Project | #58 | #58 | task-3-minimal-example | TBD | MERGED |
| | Task 4 | CI Install Validation | #59 | #59 | task-4-add-ci-install-validation | TBD | MERGED |
| | Task 5 | Restore CI Scripts | #60 | #60 | task-1-restore-ci-scripts | TBD | MERGED |
| | Task 6 | Release | #61 | #61 | task-6-release-v0.4.0 | TBD | MERGED |

## v0.4.1 Task Traceability

| Task | Module | Issue | PR | Branch | Commit | Status |
|---|---|---|---|---|---|---|
| | Task 1 | Fix CMake Target Name | #38 | #58 | task-1-fix-cmake-target | 65a9804 | MERGED |
| | Task 2 | Fix Install Test Target | #38 | #59 | task-2-fix-install-test-target | a8cfe72 | MERGED |
| | Task 3 | Release | #60 | #61 | task-6-release-v0.4.1 | TBD-after-merge | PENDING |

## Release (v0.3.0)

- Release URL: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.3.0
- Release asset: CoreBrick-C-v0.3.0.zip