# Build and run tests
$ErrorActionPreference = "Stop"

$cmakePath = "cmake"
if (Test-Path "C:\CMake\bin\cmake.exe") {
    $cmakePath = "C:\CMake\bin\cmake.exe"
}

Write-Host "=== Configuring ===" -ForegroundColor Cyan
& $cmakePath -S . -B build
if ($LASTEXITCODE -ne 0) {
    Write-Host "FAIL: cmake configure failed" -ForegroundColor Red
    exit 1
}

Write-Host "=== Building ===" -ForegroundColor Cyan
& $cmakePath --build build
if ($LASTEXITCODE -ne 0) {
    Write-Host "FAIL: cmake build failed" -ForegroundColor Red
    exit 1
}

Write-Host "=== Running tests ===" -ForegroundColor Cyan
& $cmakePath --build build --target corebrick_tests 2>$null
if ($LASTEXITCODE -ne 0) {
    # Try running directly
    if (Test-Path "build\Debug\corebrick_tests.exe") {
        & "build\Debug\corebrick_tests.exe"
    } else {
        & .\build\Debug\corebrick_tests.exe 2>$null
    }
}

& ctest --test-dir build --output-on-failure
if ($LASTEXITCODE -ne 0) {
    Write-Host "FAIL: tests failed" -ForegroundColor Red
    exit 1
}

Write-Host "OK: All tests passed" -ForegroundColor Green
exit 0