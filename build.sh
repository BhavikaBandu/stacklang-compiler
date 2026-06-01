#!/bin/bash

set -e

for cmd in cmake clang llvm-config; do
    if ! command -v "$cmd" >/dev/null 2>&1; then
        echo "[ERROR] Missing dependency: $cmd"
        echo
        echo "Install dependencies with:"
        echo "sudo apt update"
        echo "sudo apt install clang llvm llvm-dev cmake build-essential -y"
        exit 1
    fi
done

cmake -S . -B build
cmake --build build -j