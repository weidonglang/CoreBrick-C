# CoreBrick-C Audit Index

## Repository

- Repository: https://github.com/weidonglang/CoreBrick-C
- Default branch: main
- Latest release: v0.3.3
- Latest tag: v0.3.3

## v0.1.0 Task Traceability

| Task | Module | Commit | Main Files | Tests | Docs | Status |
|---|---|---|---|---|---|---|
| Task 0 | Bootstrap | 5fd5b89 | CMakeLists.txt, README.md | tests/test_main.c | docs/architecture.md | PASS |
| Task 1 | Arena | 5fd5b89 | include/cb_arena.h, src/cb_arena.c | tests/test_arena.c | docs/arena.md | PASS |
| Task 2 | ByteBuffer | 5fd5b89 | include/cb_buffer.h, src/cb_buffer.c | tests/test_buffer.c | docs/buffer.md | PASS |
| Task 3 | StringView | 5fd5b89 | include/cb_string_view.h, src/cb_string_view.c | tests/test_string_view.c | docs/string_view.md | PASS |
| Task 4 | Release | 5fd5b89 | README.md, docs/review_report_v0.1.0.md | smoke_test.ps1 | docs/release_notes.md | PASS |

## v0.2.0 Task Traceability

| Task | Module | Branch | Commit | Main Files | Tests | Docs | Status |
|---|---|---|---|---|---|---|---|
| Task 1 | Vector | task-1-vector | f8215ee | include/cb_vector.h, src/cb_vector.c | tests/test_vector.c | docs/vector.md | PASS |
| Task 2 | Hash | task-2-hash | 16555ab | include/cb_hash.h, src/cb_hash.c | tests/test_hash.c | docs/hash.md | PASS |
| Task 3 | FileUtils | task-3-file-utils | 19bf3d2 | include/cb_file.h, src/cb_file.c | tests/test_file.c | docs/file.md | PASS |
| Task 4 | Timer/Benchmark | task-4-timer-benchmark | e29c7a2 | include/cb_timer.h, src/cb_timer.c | tests/test_timer.c | docs/timer.md | PASS |
| Task 5 | Release | task-5-release-v0.2.0 | c602213 | README.md, docs/review_report_v0.2.0.md | smoke_test.ps1 | docs/release_notes.md | PASS |

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
|---|---|---|---|---|---|---|
| Task 0 | PR Audit Guard | #1 | #2 | task-0-pr-audit-guard | 58a567a | MERGED |
| Task 1 | BitSet | #3 | #4 | task-1-bitset | 3aaa0b0 | MERGED |
| Task 2 | RingBuffer | #5 | #6 | task-2-ring-buffer | 2740fd4 | MERGED |
| Task 3 | Bloom Filter | #7 | #8 | task-3-bloom-filter | aa43af8 | MERGED |
| Task 4 | StringMap | #9 | #11 | task-4-string-map | 98d40e6 | MERGED |
| Task 5 | Release | #12 | #13 | task-5-release-v0.3.0 | 4b651d2 | MERGED |

## v0.3.1 Task Traceability

| Task | Scope | Issue | PR | Branch | Commit | Status |
|---|---|---|---|---|---|---|
| Task 0 | v0.3.0 audit records | (in commit 7139176) | - | main | 7139176 | MERGED |
| Task 1 | StringMap portability / rehash | #14 | #15 | task-1-fix-string-map | 129511c | MERGED |
| Task 2 | Allocation overflow / alignment | #16 | #17 | task-2-fix-allocation-safety | c8a8a6e | MERGED |
| Task 3 | FileUtils size / close handling | #18 | #19 | task-3-fix-file-utils | 3662fe8 | MERGED |
| Task 4 | Bloom / StringView edge semantics | #20 | #21 | task-4-fix-edge-semantics | 720a771 | MERGED |
| Task 5 | Hash / CMake cleanup | #22 | #23 | task-5-fix-hash-cmake | e073031 | MERGED |
| Task 6 | Release | #24 | | task-6-release-v0.3.1 | 7710219 | MERGED |

## v0.3.2 Task Traceability

| Task | Scope | Issue | PR | Branch | Commit | Status |
|---|---|---|---|---|---|---|
| Task 1 | Timer Windows overflow fix | #25 | #26 | task-1-fix-timer-overflow | 7710219 | MERGED |
| Task 2 | Release v0.3.2 | #27 | #28 | task-6-release-v0.3.2 | f22466c | MERGED |

## v0.3.3 Task Traceability

| Task | Scope | Issue | PR | Branch | Commit | Status |
|---|---|---|---|---|---|---|
| Task 1 | Restore tests and version scripts | #27 | #28 | task-1-restore-ci-scripts | 9262984 | MERGED |
| Task 2 | Release v0.3.3 | #29 | | task-2-release-v0.3.3 | | PENDING |

## Known Limitations

- v0.1.0 was delivered as a consolidated initial release.
- v0.2.0 used separate branches and commits per module, but GitHub pull requests were not created or are not available for audit.
- No language bindings.

## Release (v0.3.0)

- Release URL: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.3.0
- Release asset: CoreBrick-C-v0.3.0.zip

## Release (v0.3.1)

- Release URL: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.3.1
- Release asset: CoreBrick-C-v0.3.1.zip

## Release (v0.3.2)

- Release URL: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.3.2
- Release asset: CoreBrick-C-v0.3.2.zip

## Release (v0.3.3)

- Release URL: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.3.3
- Release asset: CoreBrick-C-v0.3.3.zip
