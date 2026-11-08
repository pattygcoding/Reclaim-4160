#!/usr/bin/env bash
# Configure and build the native desktop game (macOS / Linux).
# Requires SDL2, SDL2_image, SDL2_ttf and SDL2_mixer development packages.
set -euo pipefail
cd "$(dirname "$0")/.."

BUILD_DIR="${1:-build}"
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR"

echo
echo "Build complete -> $BUILD_DIR/reclaim"
