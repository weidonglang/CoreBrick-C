# Build release configuration
$ErrorActionPreference = "Stop"

$rootDir = Split-Path -Parent $PSScriptRoot
Set-Location $rootDir

$cmakePath = "cmake"
$vsCmake = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
if (Test-Path $vsCmake) { $cmakePath = $vsCmake }

Write-Host "=== Building Release ===" -ForegroundColor Cyan

if (Test-Path "build") { Remove-Item -Recurse -Force "build" }

& $cmakePath -S . -B build -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) { Write-Host "FAIL: cmake configure" -ForegroundColor Red; exit 1 }

& $cmakePath --build build --config Release
if ($LASTEXITCODE -ne 0) { Write-Host "FAIL: cmake build" -ForegroundColor Red; exit 1 }

Write-Host "OK: Release build successful" -ForegroundColor Green
exit 0