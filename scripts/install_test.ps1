# Install test - verify that installed CoreBrick-C can be found via find_package
param(
    [string]$InstallPrefix = ""
)

$ErrorActionPreference = "Stop"

$rootDir = Split-Path -Parent $PSScriptRoot
Set-Location $rootDir

$cmakePath = "cmake"
$vsCmake = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
if (Test-Path $vsCmake) { $cmakePath = $vsCmake }

if ($InstallPrefix -eq "") {
    $installRoot = Join-Path $rootDir "build_install_root"
} else {
    $installRoot = $InstallPrefix
}
$installTestDir = Join-Path $rootDir "build_install_test"

Write-Host "=== Install Test ===" -ForegroundColor Cyan

if ($InstallPrefix -eq "") {
    # Step 1: Configure and build CoreBrick-C
    Write-Host "Configuring CoreBrick-C..." -ForegroundColor Yellow
    & $cmakePath -S . -B build_install -DCMAKE_INSTALL_PREFIX="$installRoot" 2>&1 | Out-Null
    if ($LASTEXITCODE -ne 0) { Write-Host "FAIL: cmake configure" -ForegroundColor Red; exit 1 }

    Write-Host "Building CoreBrick-C..." -ForegroundColor Yellow
    & $cmakePath --build build_install --config Debug 2>&1 | Out-Null
    if ($LASTEXITCODE -ne 0) { Write-Host "FAIL: cmake build" -ForegroundColor Red; exit 1 }

    # Step 2: Install
    Write-Host "Installing CoreBrick-C..." -ForegroundColor Yellow
    & $cmakePath --install build_install --config Debug 2>&1 | Out-Null
    if ($LASTEXITCODE -ne 0) { Write-Host "FAIL: cmake install" -ForegroundColor Red; exit 1 }
} else {
    Write-Host "Using pre-installed prefix: $installRoot" -ForegroundColor Yellow
}

# Step 3: Verify install layout
Write-Host "Verifying install layout..." -ForegroundColor Yellow
$requiredPaths = @(
    "include/corebrick.h",
    "include/corebrick_version.h"
)

# cmake config files are installed to share/cmake/ on Ubuntu and lib/cmake/ on Windows
$cmakeDir = "share/cmake/CoreBrick"
if (-not (Test-Path (Join-Path (Join-Path $installRoot $cmakeDir) "CoreBrickConfig.cmake"))) {
    $cmakeDir = "lib/cmake/CoreBrick"
}
$requiredPaths += "$cmakeDir/CoreBrickConfig.cmake"
$requiredPaths += "$cmakeDir/CoreBrickConfigVersion.cmake"
$requiredPaths += "$cmakeDir/CoreBrickTargets.cmake"

foreach ($relPath in $requiredPaths) {
    $fullPath = Join-Path $installRoot $relPath
    if (-not (Test-Path $fullPath)) {
        Write-Host "FAIL: missing $relPath" -ForegroundColor Red
        exit 1
    }
    Write-Host "  Found: $relPath" -ForegroundColor Green
}

# Step 4: Build the install test project
Write-Host "Configuring install test..." -ForegroundColor Yellow
& $cmakePath -S tests/cmake_install_test -B $installTestDir -DCMAKE_PREFIX_PATH="$installRoot" 2>&1 | Out-Null
if ($LASTEXITCODE -ne 0) { Write-Host "FAIL: install test configure" -ForegroundColor Red; exit 1 }

Write-Host "Building install test..." -ForegroundColor Yellow
& $cmakePath --build $installTestDir --config Debug 2>&1 | Out-Null
if ($LASTEXITCODE -ne 0) { Write-Host "FAIL: install test build" -ForegroundColor Red; exit 1 }

# Step 5: Run the install test
Write-Host "Running install test..." -ForegroundColor Yellow
$testExe = "test_install.exe"
if (Test-Path "$installTestDir\Debug\$testExe") {
    $output = & "$installTestDir\Debug\$testExe" 2>&1 | Out-String
} elseif (Test-Path "$installTestDir\$testExe") {
    $output = & "$installTestDir\$testExe" 2>&1 | Out-String
} else {
    Write-Host "FAIL: test_install.exe not found" -ForegroundColor Red
    exit 1
}
Write-Host $output
if ($LASTEXITCODE -ne 0) { Write-Host "FAIL: install test run" -ForegroundColor Red; exit 1 }

Write-Host "=== Install Test PASSED ===" -ForegroundColor Green
exit 0