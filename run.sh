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

echo ""
echo "Starting StackLang web interface..."
echo "Open this in your browser:"
echo "http://127.0.0.1:5000"
echo ""

python3 backend/server.py
