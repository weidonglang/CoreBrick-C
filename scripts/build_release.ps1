# Build release configuration
$ErrorActionPreference = "Stop"

$cmakePath = "cmake"
if (Test-Path "C:\CMake\bin\cmake.exe") {
    $cmakePath = "C:\CMake\bin\cmake.exe"
}

Write-Host "=== Building Release ===" -ForegroundColor Cyan

# Clean build directory
if (Test-Path "build") {
    Remove-Item -Recurse -Force "build"
}

& $cmakePath -S . -B build -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) {
    Write-Host "FAIL: cmake configure" -ForegroundColor Red
    exit 1
}

& $cmakePath --build build --config Release
if ($LASTEXITCODE -ne 0) {
    Write-Host "FAIL: cmake build" -ForegroundColor Red
    exit 1
}

Write-Host "OK: Release build successful" -ForegroundColor Green
exit 0