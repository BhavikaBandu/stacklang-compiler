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

### Build Output

Successful compilation produces:

```text
stack_compiler
```

This executable is then used by both:

* Command-line execution mode (`./run.sh sample.stack`)
* Web interface mode (`./run.sh`)

### Verification

After building, verify the compiler exists:

```bash
ls -l stack_compiler
```

Expected output:

```text
-rwxr-xr-x ... stack_compiler
```

### Troubleshooting

#### Missing LLVM Development Libraries

If LLVM headers cannot be found:

```bash
sudo apt install llvm llvm-dev libclang-dev
```

#### Missing C++ Standard Library Headers

If errors similar to:

```text
fatal error: 'iostream' file not found
```

occur, install:

```bash
sudo apt install g++-12 libstdc++-12-dev
```

#### Build Failure

Re-run the build script:

```bash
./build.sh
```

and inspect the compiler output for detailed diagnostics.


---

## Running

### Mode 1: Command Line Execution

Run a sample program directly:

```bash
./run.sh 01_arithmetic.stack
```

or

```bash
./run.sh samples/01_arithmetic.stack
```

The script automatically:

1. Compiles the StackLang source
2. Generates LLVM IR
3. Compiles LLVM IR using Clang
4. Executes the generated native program
5. Displays the final output

---

### Mode 2: Web Interface

Start the frontend interface:

```bash
./run.sh
```

Then open:

```text
http://127.0.0.1:5000
```

The web interface allows users to:

* Enter custom StackLang programs
* Execute programs interactively
* View compiler outputs
* Select predefined sample programs from the Examples dropdown

---

## Available Sample Programs

| File                              | Purpose                         |
| --------------------------------- | ------------------------------- |
| 01_arithmetic.stack               | Arithmetic operations           |
| 02_variables.stack                | Variable assignment and loading |
| 03_ifelse_true_false.stack        | Conditional execution           |
| 04_error_stack_underflow.stack    | Stack underflow error           |
| 05_error_undefined_variable.stack | Undefined variable error        |
| 06_error_missing_endif.stack      | Parser error handling           |

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
