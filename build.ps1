# Build and Test Script for CwAPI3D.Ex
# This script builds the project and runs all tests

Write-Host "=== CwAPI3D.Ex Build Script ===" -ForegroundColor Cyan
Write-Host ""

# Check if CMake is available
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host "Error: CMake not found. Please install CMake." -ForegroundColor Red
    exit 1
}

Write-Host "CMake version:" -ForegroundColor Green
cmake --version
Write-Host ""

# Clean previous build
if (Test-Path "build") {
    Write-Host "Cleaning previous build..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force build
}

# Configure
Write-Host "Configuring project..." -ForegroundColor Green
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DBUILD_EXAMPLES=ON

if ($LASTEXITCODE -ne 0) {
    Write-Host "Configuration failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""

# Build
Write-Host "Building project..." -ForegroundColor Green
cmake --build build --config Release

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""

# Test
Write-Host "Running tests..." -ForegroundColor Green
Set-Location build
ctest -C Release --output-on-failure
$testResult = $LASTEXITCODE
Set-Location ..

Write-Host ""

if ($testResult -eq 0) {
    Write-Host "=== Build and Test Successful! ===" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Cyan
    Write-Host "  1. Run examples: build\bin\Release\geometry_example.exe"
    Write-Host "  2. Install: cmake --install build --prefix <install-path>"
    Write-Host "  3. See QUICKSTART.md for usage examples"
} else {
    Write-Host "=== Tests Failed ===" -ForegroundColor Red
    exit 1
}
