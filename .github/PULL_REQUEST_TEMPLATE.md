## Description

Brief description of the changes.

## Issue

Closes #

## Type of Change

- [ ] Bug fix
- [ ] New feature
- [ ] Documentation
- [ ] Refactor
- [ ] Test
- [ ] Build / CI
- [ ] Release

## Checklist

- [ ] Changes are limited to the scope of the associated Issue
- [ ] All tests pass: `cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure`
- [ ] No build artifacts staged (`build/`, `dist/`, `*.exe`, `*.lib`, etc.)
- [ ] Documentation updated if API changed
- [ ] AUDIT_INDEX.md updated for versioned changes
- [ ] Commit message follows conventional commit convention

## Verification

```
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
