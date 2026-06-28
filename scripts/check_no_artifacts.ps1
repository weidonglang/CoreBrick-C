# Check no build artifacts are tracked by git
$ErrorActionPreference = "Stop"

$rootDir = Split-Path -Parent $PSScriptRoot
Set-Location $rootDir

# Patterns to check
$patterns = @(
    "*.exe",
    "*.dll",
    "*.lib",
    "*.obj",
    "*.pdb",
    "*.zip",
    "*.log"
)

# Directories to check
$directories = @(
    "build/",
    "dist/",
    ".vscode/",
    ".idea/"
)

$errors = 0

# Check tracked files for artifact patterns
foreach ($pattern in $patterns) {
    $files = git ls-files -- "$pattern" 2>$null
    if ($LASTEXITCODE -eq 0 -and $files) {
        Write-Host "ERROR: Found $pattern files tracked:" -ForegroundColor Red
        foreach ($f in $files) {
            Write-Host "  $f"
        }
        $errors++
    }
}

# Check if build/ directory is tracked
foreach ($dir in $directories) {
    $entries = git ls-files -- "$dir*" 2>$null
    if ($LASTEXITCODE -eq 0 -and $entries) {
        Write-Host "ERROR: Found tracked entries in $dir" -ForegroundColor Red
        foreach ($e in $entries) {
            Write-Host "  $e"
        }
        $errors++
    }
}

if ($errors -gt 0) {
    Write-Host "FAIL: Artifacts detected in repository." -ForegroundColor Red
    exit 1
}

Write-Host "OK: No artifacts detected in repository." -ForegroundColor Green
exit 0