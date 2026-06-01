#!/bin/bash

echo "========================================"
echo " STACKLANG RUN SYSTEM"
echo "========================================"

if [ ! -f stack_compiler ]
then
    echo "Compiler executable not found."
    echo "Running build.sh first..."
    ./build.sh
fi

if [ ! -f stack_compiler ]
then
    echo "Build failed. Cannot start frontend."
    exit 1
fi

# ----------------------------------------
# CLI MODE
# ----------------------------------------
if [ $# -eq 1 ]; then

    INPUT_FILE="$1"

    # Allow:
    # ./run.sh 01_arithmetic.stack
    if [ -f "samples/$INPUT_FILE" ]; then
        INPUT_FILE="samples/$INPUT_FILE"
    fi

    if [ ! -f "$INPUT_FILE" ]; then
        echo ""
        echo "Error: File not found -> $INPUT_FILE"
        echo ""
        echo "Available sample programs:"
        ls samples/
        exit 1
    fi

    echo ""
    echo "Running StackLang program:"
    echo "$INPUT_FILE"
    echo ""

    ./stack_compiler "$INPUT_FILE"

    if [ $? -ne 0 ]; then
        echo ""
        echo "StackLang compilation failed."
        exit 1
    fi

    echo ""
    echo "========================================"
    echo " NATIVE COMPILATION"
    echo "========================================"

    clang output.ll -o program

    if [ $? -ne 0 ]; then
        echo "LLVM IR compilation failed."
        exit 1
    fi

    echo ""
    echo "========================================"
    echo " PROGRAM OUTPUT"
    echo "========================================"
    ./program
    exit 0
fi

# ----------------------------------------
# WEB MODE
# ----------------------------------------

echo ""
echo "Starting StackLang web interface..."
echo "Open this in your browser:"
echo "http://127.0.0.1:5000"
echo ""

python3 backend/server.py
