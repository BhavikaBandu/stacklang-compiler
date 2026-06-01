#!/bin/bash

set -e

if [ $# -ne 1 ]; then
    echo "Usage: ./run.sh <source.stack>"
    exit 1
fi

SOURCE_FILE="$1"

if [ ! -f "$SOURCE_FILE" ]; then
    echo "[ERROR] Source file not found: $SOURCE_FILE"
    exit 1
fi

echo "[BUILD] Building StackLang compiler..."
cmake -S . -B build > /dev/null
cmake --build build -j > /dev/null

echo "[COMPILER] Running compiler..."
./build/stack_compiler "$SOURCE_FILE"

echo "[LLVM] Building generated IR..."
clang output.ll -o program

echo "[RUN] Executing program..."
echo "----------------------------------------"
./program
echo "----------------------------------------"