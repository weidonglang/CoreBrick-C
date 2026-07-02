# validate_all.ps1 — Comprehensive pre-release validation
# Run this before any release to verify build, tests, packaging, and repository hygiene.
# Exit code: 0 = all checks pass, 1 = any check fails.

$ErrorActionPreference = "Stop"

$rootDir = Split-Path -Parent $PSScriptRoot
Set-Location $rootDir

$cmakePath = "cmake"
$vsCmake = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
if (Test-Path $vsCmake) { $cmakePath = $vsCmake }

$passed = 0
$failed = 0

function Write-Step {
    param([string]$Label)
    Write-Host ""
    Write-Host "=== $Label ===" -ForegroundColor Cyan
}

function Write-Pass {
    param([string]$Message)
    Write-Host "PASS: $Message" -ForegroundColor Green
    $script:passed++
}

function Write-Fail {
    param([string]$Message)
    Write-Host "FAIL: $Message" -ForegroundColor Red
    $script:failed++
    $script:anyFailed = $true
}

$anyFailed = $false

Write-Host "============================================" -ForegroundColor Magenta
Write-Host "  CoreBrick-C Pre-Release Validation Suite" -ForegroundColor Magenta
Write-Host "============================================" -ForegroundColor Magenta

# ──────────────────────────────────────────────
# 1. Version consistency check
# ──────────────────────────────────────────────
Write-Step "Step 1/8: Version Consistency"

$versionFile = "VERSION"
$cmakeFile = "CMakeLists.txt"

if (-not (Test-Path $versionFile)) {
    Write-Fail "VERSION file not found"
} else {
    $version = (Get-Content $versionFile -Raw).Trim()
    if ([string]::IsNullOrEmpty($version)) {
        Write-Fail "VERSION file is empty"
    } else {
        $cmakeContent = Get-Content $cmakeFile -Raw
        $match = [regex]::Match($cmakeContent, 'project\s*\(\s*corebrick\s+VERSION\s+(\d+\.\d+\.\d+)', [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
        if (-not $match.Success) {
            Write-Fail "Could not parse VERSION from CMakeLists.txt"
        } else {
            $cmakeVersion = $match.Groups[1].Value
            if ($version -ne $cmakeVersion) {
                Write-Fail "VERSION='$version' but CMakeLists.txt='$cmakeVersion'"
            } else {
                Write-Pass "VERSION = $version (matches CMakeLists.txt)"
            }
        }
    }
}

# ──────────────────────────────────────────────
# 2. Clean CMake configure
# ──────────────────────────────────────────────
Write-Step "Step 2/8: CMake Configure"

if (Test-Path "build_validate") {
    Remove-Item -Recurse -Force "build_validate"
}
$configureOutput = & $cmakePath -S . -B build_validate 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Fail "cmake configure failed"
    Write-Host $configureOutput
} else {
    Write-Pass "cmake configure succeeded"
}

# ──────────────────────────────────────────────
# 3. CMake build
# ──────────────────────────────────────────────
Write-Step "Step 3/8: CMake Build"

$buildOutput = & $cmakePath --build build_validate 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Fail "cmake build failed"
    Write-Host $buildOutput
} else {
    Write-Pass "cmake build succeeded"
}

# ──────────────────────────────────────────────
# 4. Run all tests via ctest
# ──────────────────────────────────────────────
Write-Step "Step 4/8: CTest"

$testOutput = & $cmakePath --build build_validate --target corebrick_tests 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Fail "test target build failed"
    Write-Host $testOutput
} else {
    $ctestOutput = & ctest --test-dir build_validate -C Debug --output-on-failure 2>&1
    if ($LASTEXITCODE -ne 0) {
        Write-Fail "ctest reported failures"
        Write-Host $ctestOutput
    } else {
        # Extract summary line: "# tests passed out of #"
        $summaryLine = $ctestOutput | Select-String "tests passed" | Select-Object -Last 1
        if ($summaryLine) {
            Write-Pass $summaryLine.ToString().Trim()
        } else {
            Write-Pass "all tests passed"
        }
    }
}

# ──────────────────────────────────────────────
# 5. Smoke test
# ──────────────────────────────────────────────
Write-Step "Step 5/8: Smoke Test"

$smokeOutput = powershell -ExecutionPolicy Bypass -File scripts/smoke_test.ps1 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Fail "smoke test failed"
    Write-Host $smokeOutput
} else {
    Write-Pass "smoke test passed"
}

# ──────────────────────────────────────────────
# 6. Check no artifacts in repository
# ──────────────────────────────────────────────
Write-Step "Step 6/8: Artifact Check"

$artifactOutput = powershell -ExecutionPolicy Bypass -File scripts/check_no_artifacts.ps1 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Fail "artifact check failed"
    Write-Host $artifactOutput
} else {
    Write-Pass "no artifacts detected in repository"
}

# ──────────────────────────────────────────────
# 7. Release build
# ──────────────────────────────────────────────
Write-Step "Step 7/8: Release Build"

$releaseBuildOutput = powershell -ExecutionPolicy Bypass -File scripts/build_release.ps1 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Fail "release build failed"
    Write-Host $releaseBuildOutput
} else {
    Write-Pass "release build succeeded"
}

# ──────────────────────────────────────────────
# 8. Package release archive
# ──────────────────────────────────────────────
Write-Step "Step 8/8: Release Packaging"

$packageOutput = powershell -ExecutionPolicy Bypass -File scripts/package_release.ps1 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Fail "release packaging failed"
    Write-Host $packageOutput
} else {
    # Verify the zip file exists
    $version = (Get-Content $versionFile -Raw).Trim()
    $zipPath = "dist/CoreBrick-C-v$version.zip"
    if (Test-Path $zipPath) {
        $zipSize = (Get-Item $zipPath).Length
        Write-Pass "release archive created: $zipPath ($zipSize bytes)"
    } else {
        Write-Fail "release archive not found at $zipPath"
    }
}

# ──────────────────────────────────────────────
# Summary
# ──────────────────────────────────────────────
Write-Host ""
Write-Host "============================================" -ForegroundColor Magenta
Write-Host "  Validation Summary" -ForegroundColor Magenta
Write-Host "============================================" -ForegroundColor Magenta
Write-Host "  Passed: $passed" -ForegroundColor Green
Write-Host "  Failed: $failed" -ForegroundColor Red

if ($anyFailed) {
    Write-Host ""
    Write-Host "  VALIDATION FAILED — review errors above before releasing." -ForegroundColor Red
    Write-Host ""
    exit 1
} else {
    Write-Host ""
    Write-Host "  ALL CHECKS PASSED — release is ready." -ForegroundColor Green
    Write-Host ""
    exit 0
}