# StackLang Compiler

## 📌 Overview

This project implements a **mini compiler for a stack-based programming language (StackLang)**.

The compiler processes custom source code and converts it into:

> **LLVM Intermediate Representation (IR)** → which is then compiled into a **native executable using clang**

This demonstrates a complete compiler pipeline — from high-level source code to low-level machine-executable programs.

---

## 🧠 Key Idea

Unlike traditional compilers that rely heavily on Abstract Syntax Trees (ASTs), this project is built around a:

> **Stack-based execution model (similar to JVM / WebAssembly)**

Instead of evaluating expressions using expression trees, operations are represented as **stack instructions** and later translated into **LLVM SSA-based IR**.

The parser performs:

* syntax validation
* semantic checks
* operand stack validation
* instruction generation

The LLVM IR generator then maintains a real operand stack during IR emission and converts stack-machine operations into SSA-form LLVM instructions.

### Example:

```txt
5 3 +
```

Conceptual stack behavior:

```txt
Push 5
Push 3
Pop → 3
Pop → 5
Add → 8
Push result
```

LLVM IR equivalent:

```llvm
%addtmp = add i32 5, 3
```

This demonstrates how stack-machine operations map into LLVM SSA instructions.

---

## ⚙️ Features

* Custom lexer (no external tools)
* Recursive-descent parser
* Stack-instruction generation
* Operand stack validation
* Arithmetic operations (`+ - * /`)
* Comparison operations (`> < >= <= == !=`)
* Variable assignment and access
* Print support
* If-else control flow
* LLVM IR generation
* Native executable generation using `clang`
* LLVM IR trace visualization
* Operand stack visualization during IR generation

---

## 📂 Project Structure

```txt
stacklang-compiler/
│
├── src/
│   ├── main.cpp
│   ├── lexer.h
│   ├── lexer.cpp
│   ├── instructions.h
│   ├── parser.h
│   ├── parser.cpp
│   ├── codegen.h
│   └── codegen.cpp
│
├── samples/
│   ├── 01_arithmetic.stack
│   ├── 02_variables.stack
│   ├── 03_ifelse_true_false.stack
│   ├── 04_error_stack_underflow.stack
│   ├── 05_error_undefined_variable.stack
│   └── 06_error_missing_endif.stack
│
├── README.md
└── .gitignore
```

### 📁 Structure Overview

* **`src/main.cpp`**
  Entry point of the compiler. Reads the source file and coordinates the lexer, parser, stack-instruction generation, and LLVM IR generation.

* **`src/lexer.h`**
  Defines token types, token structures, and lexer interface.

* **`src/lexer.cpp`**
  Implements lexical analysis by converting source code into tokens.

* **`src/instructions.h`**
  Defines the intermediate stack-instruction representation used between the parser and LLVM IR generator.

* **`src/parser.h`**
  Defines the recursive-descent parser interface.

* **`src/parser.cpp`**
  Implements parsing, syntax validation, semantic checking, operand stack validation, and stack-instruction generation.

* **`src/codegen.h`**
  Defines the LLVM IR generator interface.

* **`src/codegen.cpp`**
  Generates LLVM IR using LLVM APIs. Maintains the operand stack during IR emission and handles arithmetic, variables, print, comparisons, and if-else basic blocks.

* **`samples/`**
  Contains sample StackLang programs for testing arithmetic, variables, control flow, and error handling.

---

## 📁 File Description

### `main.cpp`

Acts as the **entry point** of the compiler.

It:

* reads the input source file
* invokes the lexer to generate tokens
* runs the parser for syntax and semantic validation
* generates stack instructions
* invokes the LLVM code generator to produce IR
* prints compiler pipeline visualization

---

### `lexer.h` / `lexer.cpp`

Responsible for **lexical analysis**.

* Defines token types (NUMBER, IDENTIFIER, operators, keywords)
* Converts raw source code into a sequence of tokens
* Ignores whitespace and structures input for parsing

---

### `instructions.h`

Defines the intermediate stack-instruction representation used between:

```txt
Parser → LLVM IR Generator
```

Contains:

* instruction types (`OpType`)
* instruction structure (`Op`)

---

### `parser.h` / `parser.cpp`

Implements the **recursive-descent parser and semantic validation phase**.

Responsibilities include:

* syntax checking
* semantic validation
* operand stack validation
* variable validation
* if-else parsing
* generation of stack instructions

The parser does **not execute values directly**.
Instead, it generates a structured instruction list for the LLVM backend.

---

### `codegen.h` / `codegen.cpp`

Implements the **LLVM backend**.

* Maintains the operand stack during IR emission
* Converts stack instructions into LLVM IR instructions
* Handles:

  * arithmetic → `add`, `sub`, `mul`, `sdiv`
  * variables → `alloca`, `store`, `load`
  * comparisons → `icmp`
  * print → `printf`

* Implements control flow using:

  * LLVM Basic Blocks
  * conditional branching (`br`)

* Generates `output.ll`
* Visualizes stack-to-SSA mapping during IR generation

---

## 🔄 Compiler Pipeline

```txt
Source Code
   ↓
Lexer (Tokenization)
   ↓
Parser (Syntax + Semantic Validation)
   ↓
Stack Instruction Generation
   ↓
LLVM IR Generation
   ↓
output.ll
   ↓
clang
   ↓
Native Executable
```

---

## ⚙️ Setup & Installation

Install dependencies (Ubuntu):

```bash
sudo apt update
sudo apt install clang llvm llvm-dev build-essential -y
```

Verify:

```bash
clang --version
llvm-config --version
```

---

## 📥 Clone the Repository

```bash
git clone https://github.com/BhavikaBandu/stacklang-compiler.git
cd stacklang-compiler
```

---

## ▶️ Build the Compiler

```bash
clang++ src/main.cpp src/lexer.cpp src/parser.cpp src/codegen.cpp \
$(llvm-config --cxxflags --ldflags --system-libs --libs core) \
-o stack_compiler
```

---

## ▶️ Run the Compiler

You can run the compiler on **any StackLang source file**:

```bash
./stack_compiler samples/<file_name>.stack
```

Examples:

```bash
./stack_compiler samples/01_arithmetic.stack
./stack_compiler samples/02_variables.stack
./stack_compiler samples/03_ifelse_true_false.stack
```

This will:

* display tokens
* show stack instructions
* visualize LLVM IR generation
* show operand stack transitions
* generate `output.ll`

---

## ▶️ Generate Executable

After running the compiler:

```bash
clang output.ll -o program
./program
```

This compiles the generated LLVM IR into a native executable and runs it.

---

## 🔍 Full Execution Walkthrough

Consider the program:

```txt
5 3 + x =
x 2 * y =
y print
```

### Step 1: Lexer Output

The input is converted into tokens:

```txt
NUMBER(5)
NUMBER(3)
PLUS(+)
IDENTIFIER(x)
ASSIGN(=)
...
```

---

### Step 2: Parser Output

The parser validates the program and generates stack instructions:

```txt
PUSH_NUMBER      5
PUSH_NUMBER      3
ADD
STORE_VARIABLE   x

LOAD_VARIABLE    x
PUSH_NUMBER      2
MUL
STORE_VARIABLE   y

LOAD_VARIABLE    y
PRINT
```

---

### Step 3: LLVM IR Generation

During IR generation, the compiler visualizes:

* generated LLVM SSA instructions
* operand stack transitions

Example trace:

```txt
STEP  OPERATION        GENERATED LLVM IR                    OPERAND STACK
--------------------------------------------------------------------------------
1     PUSH 5           ; constant                           [5]
2     PUSH 3           ; constant                           [5, 3]
3     ADD              %addtmp = add i32 5, 3              [%addtmp]
4     STORE x          store i32 %addtmp, ptr %x           []
5     LOAD x           %x_load = load i32, ptr %x          [%x_load]
6     PUSH 2           ; constant                           [%x_load, 2]
7     MUL              %multmp = mul i32 %x_load, 2        [%multmp]
```

Generated LLVM IR instructions:

```llvm
%x = alloca i32
store i32 %addtmp, ptr %x
%x_load = load i32, ptr %x
%multmp = mul i32 %x_load, 2
store i32 %multmp, ptr %y
%y_load = load i32, ptr %y
call @printf(%y_load)
```

---

### Step 4: Native Execution

```bash
clang output.ll -o program
./program
```

Final output:

```txt
16
```

---

## 📊 Stack to SSA Mapping

Stack-based:

```txt
5 3 +
```

LLVM SSA:

```llvm
%addtmp = add i32 5, 3
```

👉 Stack operations are converted into SSA-based instructions during LLVM IR generation.

---

## ⚠️ Error Detection

The parser performs several structural and semantic checks:

* Stack underflow detection
* Undefined variable detection
* Invalid assignment detection
* Missing `endif`
* Unexpected `else` / `endif`
* Unknown token detection
* Stack consistency validation across if-else branches

Example:

```txt
5 +
```

Produces:

```txt
[PARSER ERROR] Stack underflow during addition
```

---

## 🧠 Concepts Demonstrated

* Compiler Design
* Lexical Analysis
* Recursive-descent Parsing
* Semantic Validation
* Stack-machine computation model
* LLVM IR generation
* Static Single Assignment (SSA)
* Operand stack management
* LLVM basic blocks
* Control flow generation
* End-to-end compilation pipeline

---

## 🏁 Summary

This project demonstrates how a stack-based programming language can be compiled into LLVM IR and native machine code using a structured compiler pipeline.

The implementation showcases:

* frontend compiler construction
* parser-driven instruction generation
* operand stack management
* LLVM SSA generation
* control flow construction
* native executable generation using LLVM and clang
