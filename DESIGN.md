# DESIGN

## Project Objective

The objective of this project is to design and implement a compiler for a custom stack-based programming language named StackLang.

The compiler translates StackLang programs into LLVM Intermediate Representation (LLVM IR), which is subsequently compiled into native machine executables using Clang.

The project demonstrates the complete compilation pipeline from source code to executable code while exposing key concepts in compiler construction and LLVM-based code generation.

---

## Language Design

StackLang follows a stack-machine execution model.

Instead of expressing computations through infix expressions and Abstract Syntax Trees (ASTs), operations are represented through stack manipulation.

Example:

```text
5 3 +
```

Execution:

```text
Push 5
Push 3
Pop 3
Pop 5
Add
Push 8
```

This model is conceptually similar to:

- Java Virtual Machine (JVM)
- WebAssembly (WASM)
- Forth-like languages

---

## Compiler Architecture

The compiler consists of four primary stages:

```text
Source Code
      ↓
Lexer
      ↓
Parser
      ↓
Stack Instruction Generation
      ↓
LLVM IR Generation
      ↓
Native Executable
```

### Lexer

Responsible for converting raw source text into a stream of tokens.

Examples:

- Numbers
- Identifiers
- Operators
- Keywords

---

### Parser

The parser performs:

- Syntax validation
- Semantic validation
- Operand stack validation
- Variable validation
- Control-flow validation

The parser generates a structured intermediate instruction representation rather than directly generating LLVM IR.

---

### Intermediate Representation

A stack-based instruction representation is used between the parser and LLVM backend.

Examples:

```text
PUSH_NUMBER
ADD
STORE_VARIABLE
LOAD_VARIABLE
PRINT
```

This representation isolates frontend logic from backend implementation.

---

### LLVM Backend

The backend consumes stack instructions and generates LLVM IR.

Responsibilities include:

- Arithmetic instruction generation
- Variable management
- Comparison generation
- Branch generation
- Basic block construction
- Native code preparation

---

## Design Decisions

### Choice of Stack-Based Language

The stack-machine model was selected because:

- Simpler frontend implementation
- Natural instruction sequencing
- Easier semantic validation
- Demonstrates stack-to-SSA translation

---

### Separation of Frontend and Backend

The compiler uses an intermediate instruction layer between parsing and LLVM generation.

Advantages:

- Cleaner architecture
- Easier debugging
- Backend independence
- Improved maintainability

---

### LLVM as Backend

LLVM was selected because:

- Industry-standard compiler infrastructure
- Mature IR design
- Strong optimization support
- Native code generation support

---

## Alternatives Considered

### Alternative 1: Direct LLVM Generation During Parsing

Description:

Generate LLVM instructions directly while parsing.

Advantages:

- Fewer compilation stages

Disadvantages:

- Tight frontend-backend coupling
- Difficult debugging
- Reduced extensibility

Decision:

Not selected.

---

### Alternative 2: AST-Based Compiler

Description:

Generate an Abstract Syntax Tree before code generation.

Advantages:

- Standard compiler architecture
- Easier optimization passes

Disadvantages:

- Additional complexity
- Less natural for stack-based languages

Decision:

Not selected.

---

### Alternative 3: Stack Instruction Intermediate Layer (Chosen)

Description:

Generate stack instructions before LLVM IR generation.

Advantages:

- Natural mapping from source language
- Cleaner architecture
- Easier validation
- Easier debugging

Disadvantages:

- Additional translation stage

Decision:

Selected.

---

## Error Handling Strategy

Errors are detected during parsing whenever possible.

Supported checks:

- Stack underflow
- Undefined variables
- Invalid assignments
- Missing endif
- Unexpected else
- Unexpected endif
- Invalid operations

This prevents invalid LLVM IR generation.

---

## Scalability Considerations

The architecture allows future support for:

- Loops
- Functions
- User-defined procedures
- Additional data types
- Optimization passes
- Static analysis

---