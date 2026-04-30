# StackLang Compiler

## 📌 Overview

This project implements a **mini compiler for a stack-based programming language (StackLang)**.

The compiler processes custom source code and converts it into:

> **LLVM Intermediate Representation (IR)** → which is then compiled into a **native executable using clang**

This demonstrates a complete compiler pipeline — from high-level source code to low-level machine-executable programs.

---

## 🧠 Key Idea

Unlike traditional compilers that rely on Abstract Syntax Trees (ASTs), this project is built around a:

> **Stack-based execution model (similar to JVM / WebAssembly)**

Instead of constructing expression trees, operations are evaluated using an **operand stack**, making parsing simpler while still enabling powerful code generation.

### Example:

```
5 3 +
```

Execution:

```
Push 5  
Push 3  
Pop → 3  
Pop → 5  
Add → 8  
Push result  
```

---

## ⚙️ Features

* Custom lexer (no external tools)
* Stack-based parser and execution model
* Arithmetic operations (`+ - * /`)
* Comparison operations (`> < ==`)
* Variable assignment and access
* Print support
* If-else control flow
* LLVM IR generation
* Native executable generation using `clang`

---

## 📂 Project Structure

```
stacklang-compiler/
│
├── src/
│   ├── main.cpp
│   ├── lexer.h
│   ├── lexer.cpp
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
  Entry point of the compiler. Reads the source file and coordinates the lexer, parser, and LLVM code generator.

* **`src/lexer.h`**
  Defines token types, token structures, and lexer interface.

* **`src/lexer.cpp`**
  Implements lexical analysis by converting source code into tokens.

* **`src/parser.h`**
  Defines the parser and stack execution interface.

* **`src/parser.cpp`**
  Implements parsing, stack execution, arithmetic, variables, print, and if-else logic with trace output.

* **`src/codegen.h`**
  Defines the LLVM IR generator interface.

* **`src/codegen.cpp`**
  Generates LLVM IR using LLVM APIs. Handles arithmetic, variables, print, comparisons, and if-else using basic blocks.

* **`samples/`**
  Contains sample StackLang programs for testing arithmetic, variables, control flow, and error handling.

---

## 📁 File Description

### `main.cpp`

Acts as the **entry point** of the compiler.
It:

* reads the input source file
* invokes the lexer to generate tokens
* runs the parser for stack execution and trace
* invokes the LLVM code generator to produce IR

---

### `lexer.h` / `lexer.cpp`

Responsible for **lexical analysis**.

* Defines token types (NUMBER, IDENTIFIER, operators, keywords)
* Converts raw source code into a sequence of tokens
* Ignores whitespace and structures input for parsing

---

### `parser.h` / `parser.cpp`

Implements the **core stack-based execution model**.

* Maintains an operand stack
* Executes operations like arithmetic and comparisons
* Handles variables (store/load)
* Processes `if-else` logic at execution level
* Produces stack traces for debugging and understanding

---

### `codegen.h` / `codegen.cpp`

Implements the **LLVM backend**.

* Converts stack operations into LLVM IR instructions
* Handles:

  * arithmetic → `add`, `sub`, `mul`, `sdiv`
  * variables → `alloca`, `store`, `load`
  * comparisons → `icmp`
  * print → `printf`
* Implements control flow using:

  * LLVM Basic Blocks
  * conditional branching (`br`)
* Generates `output.ll`

---

## 🔄 Compiler Pipeline

```
Source Code
   ↓
Lexer (Tokenization)
   ↓
Parser (Stack-based execution)
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

```
sudo apt update
sudo apt install clang llvm llvm-dev build-essential -y
```

Verify:

```
clang --version
llvm-config --version
```

---

## 📥 Clone the Repository

```
git clone https://github.com/BhavikaBandu/stacklang-compiler.git
cd stacklang-compiler
```

---

## ▶️ Build the Compiler

```
clang++ src/main.cpp src/lexer.cpp src/parser.cpp src/codegen.cpp \
$(llvm-config --cxxflags --ldflags --system-libs --libs core) \
-o stack_compiler
```

---

## ▶️ Run the Compiler

You can run the compiler on **any StackLang source file**:

```
./stack_compiler samples/<file_name>.stack
```

Examples:

```
./stack_compiler samples/01_arithmetic.stack
./stack_compiler samples/02_variables.stack
./stack_compiler samples/03_ifelse_true_false.stack
```

This will:

* display tokens
* show stack execution trace
* generate `output.ll`

---

## ▶️ Generate Executable

After running the compiler:

```
clang output.ll -o program
./program
```

This compiles the generated LLVM IR into a native executable and runs it.

---

## 🔍 Full Execution Walkthrough

Consider the program:

```
5 3 + x =
x 2 * y =
y print
```

### Step 1: Lexer Output

The input is converted into tokens:

```
NUMBER(5), NUMBER(3), PLUS, IDENTIFIER(x), ASSIGN ...
```

---

### Step 2: Parser Execution

The stack evolves as:

```
[5]
[5, 3]
[8]
x = 8
[8, 2]
[16]
y = 16
PRINT: 16
```

---

### Step 3: LLVM IR Generation

Equivalent IR is generated:

```
%x = alloca i32
store i32 8, ptr %x
%multmp = mul i32 %x_load, 2
```

---

### Step 4: Native Execution

```
clang output.ll -o program
./program
```

Final output:

```
16
```

---

## 📊 Stack to SSA Mapping

Stack-based:

```
5 3 +
```

LLVM SSA:

```
%1 = add i32 5, 3
```

👉 Stack operations are converted into SSA-based instructions used by LLVM.

---

## 🧠 Concepts Demonstrated

* Compiler Design
* Lexical Analysis
* Parsing
* Stack-based computation model
* LLVM IR generation
* Static Single Assignment (SSA)
* Control flow using basic blocks
* End-to-end compilation pipeline

---

## 🏁 Summary

This project demonstrates how a simple stack-based language can be compiled into efficient low-level code using LLVM, covering all major stages of a compiler:

* Frontend (Lexer + Parser)
* Execution Model (Operand Stack)
* Backend (LLVM IR + Native Code)

