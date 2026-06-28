# Review Guide

## 1. Build

```powershell
cmake -S . -B build
cmake --build build
```

## 2. Test

```powershell
ctest --test-dir build --output-on-failure
```

## 3. Scripts

```powershell
powershell -ExecutionPolicy Bypass -File scripts/check_version.ps1
powershell -ExecutionPolicy Bypass -File scripts/check_no_artifacts.ps1
powershell -ExecutionPolicy Bypass -File scripts/smoke_test.ps1
```

## 4. What to Review

* include/*.h public API
* src/*.c implementation
* tests/*.c coverage
* docs/*.md documentation
* examples/*.c usage samples
* AUDIT_INDEX.md traceability