# StackLang Compiler

A compiler for a minimal stack-based programming language that generates LLVM Intermediate Representation (LLVM IR) and native executables.

This project was developed as part of Assignment 2: Tiny Stack-Based Language Compiler to LLVM IR.

---

## Features

### Language Features

* Integer literals
* Named variables
* Variable assignment
* Arithmetic operations (`+`, `-`, `*`, `/`)
* Comparison operations (`>`, `<`, `>=`, `<=`, `==`, `!=`)
* Conditional execution (`if`, `else`, `endif`)
* Print statements

### Compiler Features

* Lexical analysis
* Recursive-descent parsing
* Stack validation during parsing
* LLVM IR generation using LLVM IRBuilder
* Native executable generation through Clang
* Error reporting
* LLVM IR generation trace
* Web-based visualization interface

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

## Documentation

| Document                               | Description                                |
| -------------------------------------- | ------------------------------------------ |
| [DESIGN.md](DESIGN.md)                 | Design decisions and compiler architecture |
| [IMPLEMENTATION.md](IMPLEMENTATION.md) | LLVM IR generation implementation details  |
| [EVALUATION.md](EVALUATION.md)         | Test cases, results, and evaluation        |

---

## Repository Structure

```text
.
├── backend
│   ├── runs
│   └── server.py
├── build.sh
├── DESIGN.md
├── EVALUATION.md
├── frontend
│   ├── css
│   ├── index.html
│   └── js
├── IMPLEMENTATION.md
├── README.md
├── run.sh
├── samples
│   ├── 01_arithmetic.stack
│   ├── 02_variables.stack
│   ├── 03_ifelse_true_false.stack
│   ├── 04_error_stack_underflow.stack
│   ├── 05_error_undefined_variable.stack
│   └── 06_error_missing_endif.stack
├── src
│   ├── codegen.cpp
│   ├── codegen.h
│   ├── instructions.h
│   ├── lexer.cpp
│   ├── lexer.h
│   ├── main.cpp
│   ├── parser.cpp
│   └── parser.h
└── stack_compiler
```

---

## Building

The project includes an automated build script:

```bash
./build.sh
```

The script performs the following tasks:

1. Updates the local package repository metadata.
2. Verifies that all required dependencies are installed.
3. Automatically installs missing packages.
4. Compiles the StackLang compiler source code.
5. Produces the compiler executable:

```text
stack_compiler
```

### Installed Dependencies

The build script automatically checks and installs:

| Package            | Purpose                           |
| ------------------ | --------------------------------- |
| build-essential    | GCC toolchain and build utilities |
| git                | Repository management             |
| clang              | LLVM frontend compiler            |
| llvm               | LLVM toolchain                    |
| llvm-dev           | LLVM development libraries        |
| libclang-dev       | Clang development headers         |
| g++-12             | GNU C++ compiler                  |
| libstdc++-12-dev   | C++ standard library headers      |
| python3            | Backend runtime                   |
| python3-flask      | Web server framework              |
| python3-flask-cors | Cross-origin request support      |


### Supported Platforms

The build script is designed for:

* Ubuntu 22.04 LTS
* Ubuntu 24.04 LTS
* Debian-based Linux distributions with APT package management

### Requirements

The build script assumes:

* Bash shell
* APT package manager
* Internet connectivity during dependency installation
* Sudo privileges for package installation

## Getting Started

### Clone the Repository

```bash
git clone https://github.com/BhavikaBandu/stacklang-compiler.git
cd stacklang-compiler
chmod +x build.sh run.sh
```

---

### Frontend Mode (Recommended)

Build the compiler and launch the web interface:

```bash
bash build.sh
bash run.sh
```

Open the following URL in your browser:

```text
http://127.0.0.1:5000
```

To test predefined programs:

1. Open the **Example Programs** dropdown.
2. Select a sample program.
3. Click **Compile** or **Run**.

---

### Command Line Mode

Execute a StackLang source file directly:

```bash
bash run.sh samples/<file_name>
```

Example:

```bash
bash run.sh samples/02_variables.stack
```

The script automatically:

1. Compiles the StackLang source program.
2. Generates LLVM IR (`output.ll`).
3. Produces a native executable using Clang.
4. Executes the generated program.

---

### Debugging

Rebuild the compiler:

```bash
bash build.sh
```

Run the compiler manually:

```bash
./stack_compiler samples/01_arithmetic.stack
```

Compile generated LLVM IR:

```bash
clang output.ll -o program
```

Execute the generated program:

```bash
./program
```

---

## Available Sample Programs

| File                              | Purpose                         |
| --------------------------------- | ------------------------------- |
| 01_arithmetic.stack               | Arithmetic operations           |
| 02_variables.stack                | Variable assignment and loading |
| 03_ifelse_true_false.stack        | Conditional execution           |
| 04_error_stack_underflow.stack    | Stack underflow error           |
| 05_error_undefined_variable.stack | Undefined variable error        |
| 06_error_missing_endif.stack      | Missing endif error             |

---

## Example Program

```text
10
20
+
print
```

Output:

```text
30
```

---

## LLVM IR Output

The compiler generates LLVM IR in:

```text
output.ll
```

This file can be inspected independently for analysis and debugging purposes.

---

## Error Handling

The compiler reports:

* Stack underflow
* Undefined variables
* Missing `endif`
* Unexpected tokens
* LLVM module verification failures

---

## Limitations

Current limitations:

* Integer-only values
* No loops
* No functions
* No user-defined procedures
* No type system
* No optimization passes

---

## Future Work

Potential extensions:

* While loops
* Functions and procedures
* Floating-point support
* Constant folding
* Dead code elimination
* LLVM optimization pipeline
* Register allocation analysis
* Control-flow graph visualization

---

## Conclusion

The compiler successfully translates a stack-oriented language into LLVM IR and native executables. The project implements the complete compilation pipeline from lexical analysis to executable generation while providing both command-line and web-based execution modes.

---

## Team Details

| Name             | USN        | GitHub ID    |
| ---------------- | ---------- | ------------ |
| Bhavika Bandu    | 1RV23CS064 | BhavikaBandu |
| B M Manohara     | 1RV23CS061 | Manohara-Ai  |
| Garvit Choudhary | 1RV23CS091 | Ggwp-code    |
