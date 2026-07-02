# Contributing to CoreBrick-C

Thank you for your interest in contributing to CoreBrick-C! This document outlines the development workflow, coding standards, and contribution process.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Development Workflow](#development-workflow)
- [Getting Started](#getting-started)
- [Coding Standards](#coding-standards)
- [Testing Requirements](#testing-requirements)
- [Documentation](#documentation)
- [Commit Message Convention](#commit-message-convention)
- [Pull Request Process](#pull-request-process)
- [Release Process](#release-process)

## Code of Conduct

Be respectful, constructive, and professional. We aim to maintain a welcoming environment for contributors of all experience levels.

## Development Workflow

CoreBrick-C follows a strict **one Task = one Issue + one Branch + one Commit + one PR** workflow.

### Step-by-Step

1. **Create an Issue** describing the problem or feature
2. **Create a branch** from `main` named `task-N-description`
3. **Make changes** following the coding standards
4. **Stage files individually** — never use `git add .` or `git add -A`
5. **Commit** with a conventional commit message
6. **Push** and create a Pull Request
7. **PR body must include** `Closes #ISSUE_NUMBER`
8. **Merge** after CI passes and review is complete

### Branch Naming

```
task-N-description
```

Examples:
- `task-0-fix-v0.3.0-audit`
- `task-1-fix-string-map`
- `task-2-fix-allocation-safety`

## Getting Started

### Prerequisites

- CMake 3.16+
- C11 compiler (MSVC, GCC, or Clang)
- Git
- PowerShell 7+ (Windows) or Bash (Linux)

### Setup

```powershell
# Clone the repository
git clone https://github.com/weidonglang/CoreBrick-C.git
cd CoreBrick-C

# Build and test
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Coding Standards

### Language

- C11 (ISO/IEC 9899:2011)
- No compiler-specific extensions unless guarded by `#ifdef`

### Style

- **Indentation**: 4 spaces (no tabs)
- **Naming**: `snake_case` for functions and variables
- **Macros**: `UPPER_SNAKE_CASE`
- **Types**: `cb_typedef_name_t` for public types
- **Functions**: `cb_module_name_action` (e.g., `cb_string_map_put`)
- **Error handling**: Return `CB_Error` enum values, not `-1` or `NULL` for failures
- **Comments**: Use `/* ... */` for C89 compatibility; `//` for inline comments is acceptable

### File Organization

```
src/cb_<module>.c     — Implementation
tests/test_<module>.c — Unit tests
include/corebrick.h   — Public header (single header)
docs/<module>.md      — Module documentation
```

### Header Guards

```c
#ifndef CB_MODULE_H
#define CB_MODULE_H
/* ... */
#endif /* CB_MODULE_H */
```

### Error Handling

All functions that can fail must return `CB_Error`:

```c
CB_Error cb_module_do_something(CB_SomeType *obj, int param);
```

Success is `CB_OK`. Error codes are defined in `include/corebrick.h`.

### Memory Management

- Use `cb_malloc` / `cb_free` wrappers for internal allocations
- Arena allocations are owned by the arena; no individual free
- ByteBuffer and Vector own their data; call `cb_buffer_free` / `cb_vector_free`

## Testing Requirements

### Test Framework

CoreBrick-C uses a custom lightweight test framework defined in `tests/test_common.h`.

### Test Functions

```c
static void test_something(void) {
    /* Arrange */
    CB_MyType obj;
    cb_my_type_init(&obj);

    /* Act */
    CB_Error err = cb_my_type_do_something(&obj, 42);

    /* Assert */
    TEST_ASSERT(err == CB_OK);
    TEST_ASSERT(obj.value == 42);

    /* Cleanup */
    cb_my_type_free(&obj);
}
```

### Registering Tests

```c
void register_my_type_tests(void) {
    TEST_REGISTER(test_something);
    TEST_REGISTER(test_edge_case);
}
```

### Running Tests

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### Test Coverage

- Every public function must have at least one test
- Edge cases (NULL inputs, zero sizes, overflow conditions) must be covered
- Error paths (allocation failure, invalid arguments) should be tested
- New bug fixes must include a regression test

## Documentation

### Module Documentation

Each module should have a corresponding `docs/<module>.md` file with:

- Overview / description
- Code example
- API reference (functions, parameters, return values)
- Error handling notes
- Thread safety notes (if applicable)

### Code Comments

- Public API functions must have doc comments explaining parameters and return values
- Internal functions should have brief comments explaining non-obvious logic
- Complex algorithms should include references or explanations

## Commit Message Convention

```
<type>: <short description>
```

### Types

| Type | Usage |
|---|---|
| `feat` | New feature or module |
| `fix` | Bug fix |
| `docs` | Documentation only |
| `test` | Adding or modifying tests |
| `refactor` | Code restructuring without behavior change |
| `style` | Formatting, whitespace, style fixes |
| `chore` | Build, CI, scripts, tooling |
| `release` | Version release preparation |

### Examples

```
fix: harden string map rehash safety
docs: add CI guide with workflow and script documentation
test: add overflow checks for vector and ring buffer
release: finalize CoreBrick-C v0.4.3
```

## Pull Request Process

1. **Ensure CI passes**: All tests must pass on Windows and Ubuntu
2. **Run local validation** before pushing:

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
powershell -ExecutionPolicy Bypass -File scripts/check_version.ps1
powershell -ExecutionPolicy Bypass -File scripts/check_no_artifacts.ps1
powershell -ExecutionPolicy Bypass -File scripts/smoke_test.ps1
```

3. **PR body must contain** `Closes #ISSUE_NUMBER` on its own line
4. **PR title** should match the commit message
5. **One PR per task** — no bundled changes
6. **Wait for review** before merging

### PR Checklist

- [ ] Changes are limited to the scope of the associated Issue
- [ ] All tests pass
- [ ] No build artifacts staged (`build/`, `dist/`, `*.exe`, `*.lib`, etc.)
- [ ] Documentation updated if API changed
- [ ] AUDIT_INDEX.md updated for versioned changes
- [ ] Commit message follows convention
- [ ] PR body includes `Closes #ISSUE_NUMBER`

## Release Process

See [docs/release_package.md](./docs/release_package.md) for the full release guide.

### Summary

1. Verify version consistency
2. Build and test (Debug + Release)
3. Run smoke test and artifact check
4. Update version files (`VERSION`, `CMakeLists.txt`, `include/corebrick_version.h`)
5. Update `AUDIT_INDEX.md` with Task Traceability
6. Create `docs/review_report_v<version>.md`
7. Update `docs/release_notes.md`
8. Package release archive
9. Create GitHub Release with `gh release create`

## Questions?

Open an Issue or start a Discussion on GitHub.