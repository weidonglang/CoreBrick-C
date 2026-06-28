# Create release archive
$ErrorActionPreference = "Stop"

$version = (Get-Content "VERSION" -Raw).Trim()
$packageName = "CoreBrick-C-v$version"
$distDir = "dist"

Write-Host "=== Packaging $packageName ===" -ForegroundColor Cyan

# Create dist directory
if (Test-Path $distDir) {
    Remove-Item -Recurse -Force $distDir
}
New-Item -ItemType Directory -Path $distDir -Force | Out-Null

# Create archive directory structure
$archiveDir = "$distDir\$packageName"
New-Item -ItemType Directory -Path "$archiveDir\include" -Force | Out-Null
New-Item -ItemType Directory -Path "$archiveDir\docs" -Force | Out-Null
New-Item -ItemType Directory -Path "$archiveDir\examples" -Force | Out-Null

# Copy files
Copy-Item "README.md" "$archiveDir\"
Copy-Item "LICENSE" "$archiveDir\"
Copy-Item "VERSION" "$archiveDir\"
Copy-Item "include\*" "$archiveDir\include\"
Copy-Item "docs\*" "$archiveDir\docs\"
Copy-Item "examples\*" "$archiveDir\examples\"

# Create zip
$zipPath = "$distDir\$packageName.zip"
if (Test-Path $zipPath) {
    Remove-Item $zipPath
}

Add-Type -AssemblyName System.IO.Compression.FileSystem
[System.IO.Compression.ZipFile]::CreateFromDirectory($archiveDir, $zipPath)

Write-Host "OK: Created $zipPath" -ForegroundColor Green

# Clean up temp directory
Remove-Item -Recurse -Force $archiveDir

exit 0