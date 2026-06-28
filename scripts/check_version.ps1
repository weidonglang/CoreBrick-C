# Check VERSION file matches CMakeLists.txt project version
$ErrorActionPreference = "Stop"

$rootDir = Split-Path -Parent $PSScriptRoot

# Read VERSION file
$versionFile = Join-Path $rootDir "VERSION"
if (-not (Test-Path $versionFile)) {
    Write-Host "FAIL: VERSION file not found at $versionFile" -ForegroundColor Red
    exit 1
}
$versionFromFile = (Get-Content $versionFile -Raw).Trim()

# Extract version from CMakeLists.txt project() declaration
$cmakeFile = Join-Path $rootDir "CMakeLists.txt"
if (-not (Test-Path $cmakeFile)) {
    Write-Host "FAIL: CMakeLists.txt not found at $cmakeFile" -ForegroundColor Red
    exit 1
}
$cmakeContent = Get-Content $cmakeFile -Raw
$match = [regex]::Match($cmakeContent, 'project\s*\(\s*corebrick\s+VERSION\s+(\d+\.\d+\.\d+)', [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
if (-not $match.Success) {
    Write-Host "FAIL: Could not parse VERSION from CMakeLists.txt project()" -ForegroundColor Red
    exit 1
}
$versionFromCMake = $match.Groups[1].Value

# Compare
if ($versionFromFile -ne $versionFromCMake) {
    Write-Host "FAIL: VERSION file says '$versionFromFile' but CMakeLists.txt says '$versionFromCMake'" -ForegroundColor Red
    exit 1
}

Write-Host "OK: VERSION = $versionFromFile (matches CMakeLists.txt)" -ForegroundColor Green
exit 0
