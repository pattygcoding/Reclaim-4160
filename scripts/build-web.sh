#!/usr/bin/env bash
# Build the WebAssembly version with Emscripten.
# Requires the emsdk to be active (source <emsdk>/emsdk_env.sh).
set -euo pipefail
cd "$(dirname "$0")/.."

BUILD_DIR="${1:-build-web}"

if ! command -v emcmake >/dev/null 2>&1; then
  echo "error: emcmake not found. Activate the emsdk first: source <emsdk>/emsdk_env.sh" >&2
  exit 1
fi

emcmake cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR"

cat <<EOF

Build complete -> $BUILD_DIR/reclaim.html

Serve the build over HTTP (browsers block file:// + wasm):
    cd $BUILD_DIR
    python3 -m http.server 8000
then open http://localhost:8000/reclaim.html
EOF
