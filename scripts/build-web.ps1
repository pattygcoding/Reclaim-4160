#!/usr/bin/env pwsh
# Build the WebAssembly version with Emscripten.
# Requires the emsdk to be activated (emcc / emcmake on PATH):
#   <emsdk>\emsdk_env.ps1
param([string]$BuildDir = "build-web")
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
Push-Location $root
try {
    if (-not (Get-Command emcmake -ErrorAction SilentlyContinue)) {
        throw "Emscripten not found on PATH. Activate the emsdk first (run `<emsdk>\emsdk_env.ps1`)."
    }

    emcmake cmake -S . -B $BuildDir -DCMAKE_BUILD_TYPE=Release
    cmake --build $BuildDir

    Write-Host @"

Build complete -> $BuildDir/reclaim.html

To play, serve the build folder over HTTP (browsers block file:// + wasm):
    cd $BuildDir
    python -m http.server 8000
then open http://localhost:8000/reclaim.html
"@
} finally {
    Pop-Location
}
