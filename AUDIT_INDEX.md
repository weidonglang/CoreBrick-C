# CoreBrick-C Audit Index

## Repository

- Repository: https://github.com/weidonglang/CoreBrick-C
- Default branch: main
- Latest release: v0.2.0
- Latest tag: v0.2.0

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
| Task 0 | PR Audit Guard | #1 |  | task-0-pr-audit-guard |  | IN PROGRESS |
| Task 1 | BitSet |  |  | task-1-bitset |  | PENDING |
| Task 2 | RingBuffer |  |  | task-2-ring-buffer |  | PENDING |
| Task 3 | Bloom Filter |  |  | task-3-bloom-filter |  | PENDING |
| Task 4 | StringMap |  |  | task-4-string-map |  | PENDING |
| Task 5 | Release |  |  | task-5-release-v0.3.0 |  | PENDING |

## Known Limitations

- v0.1.0 was delivered as a consolidated initial release.
- v0.2.0 used separate branches and commits per module, but GitHub pull requests were not created or are not available for audit.
- No HashMap, Bloom Filter, RingBuffer, or JSON Parser yet.
- No language bindings.

## Release

- Release URL: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.2.0
- Release asset: CoreBrick-C-v0.2.0.zip