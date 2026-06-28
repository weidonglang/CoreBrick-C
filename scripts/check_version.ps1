# Check VERSION file matches project version
$ErrorActionPreference = "Stop"

$versionFile = "VERSION"
if (-not (Test-Path $versionFile)) {
    Write-Host "FAIL: VERSION file not found" -ForegroundColor Red
    exit 1
}

$version = (Get-Content $versionFile -Raw).Trim()
$expected = "0.1.0"

if ($version -ne $expected) {
    Write-Host "FAIL: VERSION is '$version', expected '$expected'" -ForegroundColor Red
    exit 1
}

Write-Host "OK: VERSION is $version" -ForegroundColor Green
exit 0