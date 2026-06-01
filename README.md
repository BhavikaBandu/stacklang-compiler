# StackLang Compiler

A compiler for a minimal stack-based programming language that generates LLVM Intermediate Representation (LLVM IR) and native executables.

This project was developed as part of Assignment 2: Tiny Stack-Based Language Compiler to LLVM IR.

--- 

## Features

### Language Features

- Integer literals
- Named variables
- Variable assignment
- Arithmetic operations (+, -, *, /)
- Comparison operations (>, <, >=, <=, ==, !=)
- Conditional execution (if, else, endif)
- Print statements

### Compiler Features

- Lexical analysis
- Recursive-descent parsing
- Stack validation during parsing
- LLVM IR generation using LLVM IRBuilder
- Native executable generation through clang
- Error reporting
- LLVM IR generation trace

---

## Compiler Pipeline

```text
Source Program
↓
Lexer
↓
Tokens
↓
Parser
↓
Stack Instruction List
↓
LLVM IR Generator
↓
output.ll
↓
clang
↓
Native Executable
```

---

## DOCUMENTATION
| Document | Description |
|-----------|-------------|
| [DESIGN.md](DESIGN.md) | Design decisions and architecture |
| [IMPLEMENTATION.md](IMPLEMENTATION.md) | LLVM implementation details |
| [EVALUATION.md](EVALUATION.md) | Test cases and evaluation results |

---

## Repository Structure

~~~~
.
├── README.md
├── DESIGN.md
├── IMPLEMENTATION.md
├── EVALUATION.md
├── build.sh
├── run.sh
├── samples
│   ├── 01_arithmetic.stack
│   ├── 02_variables.stack
│   ├── 03_ifelse_true_false.stack
│   ├── 04_error_stack_underflow.stack
│   ├── 05_error_undefined_variable.stack
│   └── 06_error_missing_endif.stack
└── src
    ├── lexer.cpp
    ├── parser.cpp
    ├── codegen.cpp
    ├── main.cpp
    └── instructions.h
~~~~

---

## Building

~~~~bash
./build.sh
~~~~

---

## Running

~~~~bash
./run.sh samples/01_arithmetic.stack
~~~~

---

## Example Program

~~~~text
10
20
+
print
~~~~

Output:

~~~~text
30
~~~~

---

## Generated LLVM IR

The compiler emits LLVM IR to:

~~~~text
output.ll
~~~~

Compile and execute:

~~~~bash
clang output.ll -o program
./program
~~~~

---

## Error Handling

The compiler reports:

- Stack underflow
- Undefined variables
- Missing endif
- Unexpected tokens
- LLVM module verification failures

---

## Limitations

Current limitations:

- Integer-only values
- No loops
- No functions
- No user-defined procedures
- No type system
- No optimization passes

---

## Future Work

Potential extensions:

- While loops
- Functions
- Floating-point support
- Constant folding
- Dead code elimination
- LLVM optimization pipeline

---

## Conclusion

The compiler successfully translates a stack-oriented language into valid LLVM IR and native executables. All required language features were implemented and all evaluation test cases passed successfully.

---

## Team Details
| Name | USN | GitHub ID | 
|------|------|------|
| Bhavika Bandu | 1RV23CS064 | BhavikaBandu
| B M Manohara | 1RV23CS061 | Manohara-Ai
| Garvit Choudhary | 1RV23CS091 | Ggwp-code
