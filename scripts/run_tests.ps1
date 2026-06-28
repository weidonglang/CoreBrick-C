# Build and run tests
$ErrorActionPreference = "Stop"

$rootDir = Split-Path -Parent $PSScriptRoot
Set-Location $rootDir

$cmakePath = "cmake"
$vsCmake = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
if (Test-Path $vsCmake) { $cmakePath = $vsCmake }

Write-Host "=== Configuring ===" -ForegroundColor Cyan
& $cmakePath -S . -B build
if ($LASTEXITCODE -ne 0) { Write-Host "FAIL: cmake configure failed" -ForegroundColor Red; exit 1 }

Write-Host "=== Building ===" -ForegroundColor Cyan
& $cmakePath --build build
if ($LASTEXITCODE -ne 0) { Write-Host "FAIL: cmake build failed" -ForegroundColor Red; exit 1 }

Write-Host "=== Running tests ===" -ForegroundColor Cyan
& ctest --test-dir build --output-on-failure
if ($LASTEXITCODE -ne 0) { Write-Host "FAIL: tests failed" -ForegroundColor Red; exit 1 }

Write-Host "OK: All tests passed" -ForegroundColor Green
exit 0