# IMPLEMENTATION

## Overview

The StackLang compiler is implemented in C++ and uses LLVM as the backend infrastructure for Intermediate Representation (IR) generation.

The implementation is divided into four primary modules:

```text
Lexer
Parser
Instruction Representation
LLVM Backend
```

---

## Lexer

Files:

```text
lexer.h
lexer.cpp
```

### Responsibilities

The lexer converts raw source code into tokens.

Supported token categories:

- Numbers
- Identifiers
- Arithmetic operators
- Comparison operators
- Assignment operator
- Keywords

Examples:

```text
NUMBER(5)
IDENTIFIER(x)
PLUS(+)
ASSIGN(=)
```

Whitespace is ignored during tokenization.

---

## Parser

Files:

```text
parser.h
parser.cpp
```

### Responsibilities

The parser performs:

- Syntax analysis
- Semantic analysis
- Operand stack validation
- Variable validation
- Control-flow validation

The parser produces a sequence of intermediate stack instructions.

---

### Instruction Generation

Example source:

```text
5 3 +
x =
```

Generated instructions:

```text
PUSH_NUMBER 5
PUSH_NUMBER 3
ADD
STORE_VARIABLE x
```

---

## Intermediate Representation

File:

```text
instructions.h
```

### Purpose

Defines the instruction format exchanged between:

```text
Parser
    ↓
LLVM Backend
```

Instruction types include:

```text
PUSH_NUMBER
ADD
SUB
MUL
DIV
LOAD_VARIABLE
STORE_VARIABLE
PRINT
COMPARE
IF
ELSE
ENDIF
```

---

## LLVM Backend

Files:

```text
codegen.h
codegen.cpp
```

### Responsibilities

The backend consumes stack instructions and emits LLVM IR.

Major LLVM components used:

- LLVMContext
- Module
- IRBuilder
- Function
- BasicBlock
- Value

---

## Arithmetic Generation

Example:

```text
5 3 +
```

Generated LLVM IR:

```llvm
%addtmp = add i32 5, 3
```

Supported operations:

- add
- sub
- mul
- sdiv

---

## Variable Handling

Variables are allocated using:

```llvm
alloca
```

Values are written using:

```llvm
store
```

Values are read using:

```llvm
load
```

Example:

```llvm
%x = alloca i32
store i32 5, ptr %x
%tmp = load i32, ptr %x
```

---

## Comparison Generation

Supported comparisons:

```text
>
<
>=
<=
==
!=
```

LLVM implementation:

```llvm
icmp
```

Example:

```llvm
%cmp = icmp sgt i32 %a, %b
```

---

## Conditional Branching

Conditional execution is implemented using LLVM Basic Blocks.

Structure:

```text
Condition
    ↓
Then Block
    ↓
Merge Block
```

If-Else structure:

```text
Condition
   ↓
Then Block
   ↓
Else Block
   ↓
Merge Block
```

LLVM instructions used:

```llvm
br
condbr
```

---

## Print Support

Output is implemented through:

```llvm
printf
```

The backend generates:

- format string
- external printf declaration
- call instruction

Example:

```llvm
call i32 (ptr, ...) @printf(...)
```

---

## Stack-to-SSA Conversion

The source language follows a stack-machine model.

LLVM follows Static Single Assignment (SSA).

The backend maintains an internal operand stack while generating LLVM values.

Example:

Source:

```text
5 3 +
```

Stack execution:

```text
Push 5
Push 3
Add
```

LLVM:

```llvm
%addtmp = add i32 5, 3
```

This demonstrates stack-to-SSA translation.

---

## Error Detection

The parser detects:

- Stack underflow
- Undefined variables
- Invalid assignments
- Missing endif
- Unexpected else
- Unexpected endif

Errors are reported before LLVM generation.

---

## Build System

The project uses:

- CMake
- Clang
- LLVM

Build process:

```bash
./build.sh
```

Execution process:

```bash
./run.sh samples/01_arithmetic.stack
```

---