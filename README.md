# StackLang Compiler

## Overview

StackLang Compiler is a compiler for a custom stack-based programming language that translates StackLang source programs into LLVM Intermediate Representation (LLVM IR) and subsequently into native executables using Clang.

The project demonstrates a complete compiler pipeline including lexical analysis, parsing, semantic validation, intermediate instruction generation, LLVM IR generation, and native code execution.

---

## Team Members

- B M Manohara
- Bhavika Bandu
- Garvit Choudhary

---

## Platform Support

This project has been developed and tested on:

- Ubuntu 24.04 LTS

The repository assumes the availability of LLVM, Clang, and CMake through the Ubuntu package manager. Other operating systems or LLVM versions may require minor modifications.

---

## Repository Structure

```text
stacklang-compiler/
├── build.sh
├── run.sh
├── CMakeLists.txt
├── README.md
├── DESIGN.md
├── IMPLEMENTATION.md
├── EVALUATION.md
├── samples/
│   ├── 01_arithmetic.stack
│   ├── 02_variables.stack
│   ├── 03_ifelse_true_false.stack
│   ├── 04_error_stack_underflow.stack
│   ├── 05_error_undefined_variable.stack
│   └── 06_error_missing_endif.stack
└── src/
    ├── main.cpp
    ├── lexer.h
    ├── lexer.cpp
    ├── instructions.h
    ├── parser.h
    ├── parser.cpp
    ├── codegen.h
    └── codegen.cpp
```

---

## Features

- Custom lexer implementation
- Recursive-descent parser
- Stack-based intermediate instruction representation
- Operand stack validation
- Variable declaration and access
- Arithmetic operations
- Comparison operations
- Conditional branching using if-else
- LLVM IR generation
- Native executable generation through Clang
- Error detection and reporting
- Stack-to-SSA instruction mapping

---

## Prerequisites

Install the required dependencies:

```bash
sudo apt update

sudo apt install \
    clang \
    llvm \
    llvm-dev \
    cmake \
    build-essential \
    -y
```

Verify the installation:

```bash
clang --version
llvm-config --version
cmake --version
```

---

## Building the Compiler

Make the scripts executable:

```bash
chmod +x build.sh
chmod +x run.sh
```

Build the compiler:

```bash
./build.sh
```

---

## Running StackLang Programs

Run any StackLang source file using:

```bash
./run.sh <source_file.stack>
```

Example:

```bash
./run.sh samples/01_arithmetic.stack
```

Additional examples:

```bash
./run.sh samples/02_variables.stack
./run.sh samples/03_ifelse_true_false.stack
```

The execution pipeline is:

```text
StackLang Source
        ↓
Lexer
        ↓
Parser
        ↓
Instruction Generation
        ↓
LLVM IR Generation
        ↓
output.ll
        ↓
Clang
        ↓
Native Executable
        ↓
Program Output
```

---

## Documentation

Detailed project documentation is provided in the following files:

| Document | Description |
|-----------|-------------|
| DESIGN.md | Compiler architecture, design decisions, and alternatives considered |
| IMPLEMENTATION.md | Detailed implementation and LLVM-specific backend design |
| EVALUATION.md | Test cases, evaluation metrics, baseline comparison, and results |

---

## Test Programs

The repository includes multiple sample programs for validation:

| File | Purpose |
|--------|---------|
| 01_arithmetic.stack | Arithmetic operations |
| 02_variables.stack | Variable assignment and access |
| 03_ifelse_true_false.stack | Conditional execution |
| 04_error_stack_underflow.stack | Stack validation failure |
| 05_error_undefined_variable.stack | Undefined variable detection |
| 06_error_missing_endif.stack | Control-flow validation |

---

## Demonstration

The submission includes demonstration material showing:

- Successful compilation and execution
- LLVM IR generation
- Error handling scenarios
- Native executable generation

Screenshots and/or video demonstrations are included as part of the project submission.

---

## Academic Context

This project was developed as part of the HPE Compiler Design Project evaluation and demonstrates:

- Compiler construction principles
- Lexical analysis
- Parsing and semantic validation
- Stack-machine execution models
- LLVM IR generation
- Static Single Assignment (SSA) concepts
- Native code generation pipelines

---