# Smoke test - verify builds and run basic checks
$ErrorActionPreference = "Stop"

$cmakePath = "cmake"
$vsCmake = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
if (Test-Path $vsCmake) {
    $cmakePath = $vsCmake
}

Write-Host "=== Smoke Test ===" -ForegroundColor Cyan

# 1. Verify VERSION
$version = (Get-Content "VERSION" -Raw).Trim()
if ($version -ne "0.1.0") {
    Write-Host "FAIL: VERSION mismatch" -ForegroundColor Red
    exit 1
}
Write-Host "PASS: VERSION = $version" -ForegroundColor Green

# 2. Verify cmake build
Write-Host "Configuring..." -ForegroundColor Yellow
& $cmakePath -S . -B build 2>&1 | Out-Null
if ($LASTEXITCODE -ne 0) {
    Write-Host "FAIL: cmake configure" -ForegroundColor Red
    exit 1
}

Write-Host "Building..." -ForegroundColor Yellow
& $cmakePath --build build 2>&1 | Out-Null
if ($LASTEXITCODE -ne 0) {
    Write-Host "FAIL: cmake build" -ForegroundColor Red
    exit 1
}

# 3. Run tests
Write-Host "Running tests..." -ForegroundColor Yellow
& $cmakePath --build build --target corebrick_tests 2>&1 | Out-Null

$testExe = "build\Debug\corebrick_tests.exe"
if (-not (Test-Path $testExe)) {
    Write-Host "FAIL: test executable not found" -ForegroundColor Red
    exit 1
}

$output = & $testExe 2>&1 | Out-String
Write-Host $output

if ($LASTEXITCODE -ne 0) {
    Write-Host "FAIL: tests failed with exit code $LASTEXITCODE" -ForegroundColor Red
    exit 1
}

# 4. Run CLI
$cliExe = "build\Debug\cbcli.exe"
if (Test-Path $cliExe) {
    $cliOutput = & $cliExe version 2>&1
    Write-Host "CLI version: $cliOutput"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "FAIL: CLI failed" -ForegroundColor Red
        exit 1
    }
    Write-Host "PASS: CLI works" -ForegroundColor Green
} else {
    Write-Host "WARN: cbcli.exe not found, skipping" -ForegroundColor Yellow
}

Write-Host "=== Smoke Test PASSED ===" -ForegroundColor Green
exit 0