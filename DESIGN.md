# Design Document

## Overview

StackLang is a small stack-oriented programming language that compiles directly to LLVM IR.

Unlike traditional expression-tree compilers, StackLang uses an operand stack to represent intermediate values during parsing and code generation. This design mirrors execution models used by systems such as:

- Forth
- JVM Bytecode
- WebAssembly

The objective was to understand how stack-machine semantics map onto LLVM's Static Single Assignment (SSA) representation.

---

## Design Goals

1. Keep the language minimal.
2. Support variables and arithmetic.
3. Support conditional execution.
4. Generate valid LLVM IR.
5. Demonstrate stack-to-SSA translation.
6. Detect common programming errors early.

---

## Language Model

The language is stack-based.

Values are pushed onto an operand stack.

Operations consume operands from the stack and push results back.

Example:

~~~~text
10
20
+
~~~~

Stack evolution:

~~~~text
[]
[10]
[10,20]
[30]
~~~~

---

## Compiler Architecture

~~~~text
Source Code
    ↓
Lexer
    ↓
Token Stream
    ↓
Parser
    ↓
Instruction List
    ↓
LLVM IR Generator
    ↓
LLVM Module
    ↓
Native Executable
~~~~

---

## Lexer Design

The lexer performs a single linear scan over the source program.

Responsibilities:

- Recognize numbers
- Recognize identifiers
- Recognize operators
- Recognize keywords

Keywords:

~~~~text
if
else
endif
print
~~~~

---

## Parser Design

The parser uses recursive-descent parsing.

Instead of generating an Abstract Syntax Tree (AST), the parser produces a flat instruction representation.

Advantages:

- Simpler implementation
- Naturally matches stack-machine semantics
- Easier LLVM code generation

---

## Stack Validation

The parser maintains a virtual stack depth.

This allows detection of errors before code generation.

Example:

~~~~text
+
~~~~

Parser result:

~~~~text
Stack underflow during addition
~~~~

This prevents invalid LLVM IR generation.

---

## Variable Design

Variables are stored using LLVM stack allocations.

Assignment:

~~~~text
10
x =
~~~~

Usage:

~~~~text
x
~~~~

Internally:

~~~~text
store i32 ...
load i32 ...
~~~~

---

## Conditional Design

Conditions evaluate to integer values.

Zero represents false.

Non-zero represents true.

Structure:

~~~~text
condition

if
    ...
else
    ...
endif
~~~~

LLVM control flow:

~~~~text
entry
  |
cond
 /  \
T    F
|    |
then else
 \  /
 merge
~~~~

---

## Alternatives Considered

### AST-Based Compiler

Advantages:

- Standard compiler architecture
- Easier semantic analysis

Disadvantages:

- Less representative of stack-machine execution

Rejected because the objective was to explore stack-based code generation.

### Bytecode Interpreter

Advantages:

- Simpler implementation

Disadvantages:

- No LLVM IR generation
- No native executable output

Rejected because LLVM compilation was a project requirement.

---

## Error Handling Strategy

Errors are detected as early as possible.

Parser-level checks:

- Undefined variables
- Stack underflow
- Missing endif
- Unexpected tokens

Codegen-level checks:

- Runtime stack underflow
- Undefined variable accesses
- LLVM verification failures

---