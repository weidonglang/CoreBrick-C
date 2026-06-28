# CoreBrick-C Audit Index

## Repository

- Repository: https://github.com/weidonglang/CoreBrick-C
- Default branch: main
- Latest release: v0.1.0
- Latest tag: v0.1.0

## Task Traceability

| Task | Module | Issue | Commit | Main Files | Tests | Docs | Status |
|---|---|---|---|---|---|---|---|
| Task 0 | Bootstrap | — | f8a5221 | CMakeLists.txt, README.md | tests/test_main.c | docs/architecture.md | PASS |
| Task 1 | Arena | — | f8a5221 | include/cb_arena.h, src/cb_arena.c | tests/test_arena.c | docs/arena.md | PASS |
| Task 2 | ByteBuffer | — | f8a5221 | include/cb_buffer.h, src/cb_buffer.c | tests/test_buffer.c | docs/buffer.md | PASS |
| Task 3 | StringView | — | f8a5221 | include/cb_string_view.h, src/cb_string_view.c | tests/test_string_view.c | docs/string_view.md | PASS |
| Task 4 | Release | — | f8a5221 | README.md, docs/review_report_v0.1.0.md | smoke_test.ps1 | docs/release_notes.md | PASS |

## Validation Commands

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
powershell -ExecutionPolicy Bypass -File scripts/check_version.ps1
powershell -ExecutionPolicy Bypass -File scripts/check_no_artifacts.ps1
powershell -ExecutionPolicy Bypass -File scripts/smoke_test.ps1
```

## Known Limitations

- v0.1.0 was delivered as a consolidated initial release. Future versions will strictly enforce one issue, one branch, one commit, and one pull request per module.
- Only Arena, ByteBuffer, and StringView are included.
- No HashMap, Bloom Filter, RingBuffer, FileUtils, Timer, or language bindings yet.

## Release

* Release URL: https://github.com/weidonglang/CoreBrick-C/releases/tag/v0.1.0
* Release asset: CoreBrick-C-v0.1.0.zip