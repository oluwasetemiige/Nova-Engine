#!/usr/bin/env bash
# Nova Engine - Linux build script.  Usage: ./build.sh [Release|Debug|RelWithDebInfo]
set -euo pipefail

BUILD_TYPE="${1:-Release}"

command -v cmake >/dev/null || { echo "[Nova] CMake not found (need 3.20+)."; exit 1; }
command -v python3 >/dev/null || { echo "[Nova] python3 not found (GLAD needs it)."; exit 1; }

cmake -S . -B build -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
cmake --build build --parallel

echo
echo "[Nova] Build finished. Run the editor with: ./build/NovaEditor/NovaEditor"
