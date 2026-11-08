#!/usr/bin/env pwsh
# Configure and build the native desktop game.
# If a local .deps/ folder of SDL2 dev packages exists, it is used automatically.
param([string]$BuildDir = "build")
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
Push-Location $root
try {
    $cmakeArgs = @("-S", ".", "-B", $BuildDir, "-DCMAKE_BUILD_TYPE=Release")

    $deps = Join-Path $root ".deps"
    if (Test-Path $deps) {
        $prefixes = Get-ChildItem $deps -Directory | ForEach-Object {
            $mingw = Join-Path $_.FullName "x86_64-w64-mingw32"
            if (Test-Path $mingw) { $mingw } else { $_.FullName }
        }
        if ($prefixes) {
            $cmakeArgs += "-DCMAKE_PREFIX_PATH=" + ($prefixes -join ";")
            Write-Host "Using SDL2 from local .deps"
        }
    }

    cmake @cmakeArgs
    cmake --build $BuildDir
    Write-Host "`nBuild complete -> $BuildDir/reclaim"
} finally {
    Pop-Location
}
