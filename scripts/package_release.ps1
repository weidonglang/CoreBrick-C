# Create release archive
$ErrorActionPreference = "Stop"

$rootDir = Split-Path -Parent $PSScriptRoot
$version = (Get-Content (Join-Path $rootDir "VERSION") -Raw).Trim()
$packageName = "CoreBrick-C-v$version"
$distDir = Join-Path $rootDir "dist"

Write-Host "=== Packaging $packageName ===" -ForegroundColor Cyan

# Create dist directory
if (Test-Path $distDir) { Remove-Item -Recurse -Force $distDir }
New-Item -ItemType Directory -Path $distDir -Force | Out-Null

# Create archive directory structure
$archiveDir = Join-Path $distDir $packageName
New-Item -ItemType Directory -Path (Join-Path $archiveDir "cmake") -Force | Out-Null
New-Item -ItemType Directory -Path (Join-Path $archiveDir "include") -Force | Out-Null
New-Item -ItemType Directory -Path (Join-Path $archiveDir "docs") -Force | Out-Null
New-Item -ItemType Directory -Path (Join-Path $archiveDir "examples") -Force | Out-Null
New-Item -ItemType Directory -Path (Join-Path $archiveDir "scripts") -Force | Out-Null

# Copy files
Copy-Item (Join-Path $rootDir "CMakeLists.txt") $archiveDir
Copy-Item (Join-Path $rootDir "README.md") $archiveDir
Copy-Item (Join-Path $rootDir "LICENSE") $archiveDir
Copy-Item (Join-Path $rootDir "VERSION") $archiveDir
Copy-Item (Join-Path $rootDir "cmake\*") (Join-Path $archiveDir "cmake\")
Copy-Item (Join-Path $rootDir "include\*") (Join-Path $archiveDir "include\")
Copy-Item (Join-Path $rootDir "docs\*") (Join-Path $archiveDir "docs\")
Copy-Item (Join-Path $rootDir "examples\*") (Join-Path $archiveDir "examples\")
Copy-Item (Join-Path $rootDir "scripts\*") (Join-Path $archiveDir "scripts\")

# Create zip
$zipPath = Join-Path $distDir "$packageName.zip"
if (Test-Path $zipPath) { Remove-Item $zipPath }

Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::CreateFromDirectory($archiveDir, $zipPath)

Write-Host "OK: Created $zipPath" -ForegroundColor Green

Remove-Item -Recurse -Force $archiveDir
exit 0