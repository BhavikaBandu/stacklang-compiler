#!/bin/bash

echo "========================================"
echo " STACKLANG BUILD SYSTEM"
echo "========================================"

install_if_missing() {
    if ! dpkg -s "$1" >/dev/null 2>&1
    then
        echo "[INSTALLING] $1"
        sudo apt install -y "$1"
    else
        echo "[FOUND] $1"
    fi
}

echo ""
echo "Checking and installing dependencies..."
sudo apt update

install_if_missing build-essential
install_if_missing git
install_if_missing clang
install_if_missing llvm
install_if_missing llvm-dev
install_if_missing libclang-dev
install_if_missing python3
install_if_missing python3-flask
install_if_missing python3-flask-cors
install_if_missing g++-12
install_if_missing libstdc++-12-dev

echo ""
echo "Building StackLang compiler..."

clang++ src/main.cpp src/lexer.cpp src/parser.cpp src/codegen.cpp \
$(llvm-config --cxxflags --ldflags --system-libs --libs core) \
-o stack_compiler

if [ $? -eq 0 ]
then
    echo ""
    echo "Build successful."
    echo "Generated executable: stack_compiler"
else
    echo ""
    echo "Build failed."
    exit 1
fi
