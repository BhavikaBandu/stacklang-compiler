# StackLang Compiler

## 📌 Overview

This project implements a **mini compiler for a stack-based programming language (StackLang)**.

The compiler processes custom source code and converts it into:

> **LLVM Intermediate Representation (IR)** → which is then compiled into a **native executable using clang**

---

## 🧠 Key Idea

Unlike traditional compilers that rely on Abstract Syntax Trees (ASTs), this project uses a:

> **Stack-based execution model (similar to JVM / WebAssembly)**

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

## ⚙️ Features Implemented

---

### ✅ Day 1 – Lexer

* Custom-built lexer (no external tools like Flex)
* Tokenizes:

  * Numbers
  * Identifiers (variables)
  * Operators: `+ - * /`
  * Comparisons: `> < ==`
  * Assignment: `=`
  * Keywords: `if`, `else`, `endif`, `print`
* File-based input
* Token stream output

---

### ✅ Day 2 – Parser + Stack Engine

* Sequential parsing (recursive-descent style)
* Operand stack simulation
* Arithmetic operations:

  * Addition, Subtraction, Multiplication, Division
* Comparison operations:

  * Greater than, Less than, Equality
* Variable handling:

  * Assignment (`x =`)
  * Load (`x`)
* Print statement:

  * `print`
* Stack trace visualization
* Error handling:

  * Stack underflow
  * Division by zero
  * Undefined variables

---

### ✅ Day 3 – LLVM IR Generation

* Full LLVM backend integration
* Conversion of stack operations → LLVM SSA form
* LLVM IR generation for:

  * Arithmetic instructions (`add`, `sub`, `mul`, `sdiv`)
  * Variable handling (`alloca`, `store`, `load`)
  * Comparison operations (`icmp`)
  * Print using `printf`
* Generation of `output.ll`
* Compilation using `clang` to produce native executable

---

## 📂 Project Structure

```
stacklang-compiler/
│
├── src/
│   ├── main.cpp        # Entry point
│   ├── lexer.h         # Token definitions & lexer
│   ├── lexer.cpp
│   ├── parser.h        # Parser + stack simulation
│   ├── parser.cpp
│   ├── codegen.h       # LLVM IR generator
│   └── codegen.cpp
│
├── samples/
│   ├── test.stack
│   ├── arithmetic.stack
│   ├── llvm_test.stack
│   ├── error.stack
│   └── undefined.stack
│
├── output.ll           # Generated LLVM IR
└── README.md
```

---

## 🔄 Compiler Pipeline

```
Source Code
   ↓
Lexer (Tokenization)
   ↓
Parser (Stack-based execution model)
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

## 🧪 Sample Program

```
5 3 + x =
x 2 * y =
y print
```

---

## 📤 Generated LLVM IR (Example)

```llvm
%x = alloca i32
store i32 8, ptr %x
%x_load = load i32, ptr %x
%multmp = mul i32 %x_load, 2
store i32 %multmp, ptr %y
```

---

## ▶️ Execution

### Step 1: Compile Compiler

```
clang++ src/main.cpp src/lexer.cpp src/parser.cpp src/codegen.cpp \
$(llvm-config --cxxflags --ldflags --system-libs --libs core) \
-o stack_compiler
```

---

### Step 2: Generate LLVM IR

```
./stack_compiler samples/test.stack
```

---

### Step 3: Compile LLVM IR

```
clang output.ll -o program
```

---

### Step 4: Run Executable

```
./program
```

Output:

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

👉 Stack values are converted into **SSA temporaries**

---

## ⚠️ Error Handling

### Stack Underflow

```
Input:
5 +

Output:
Error: Stack underflow
```

---

### Undefined Variable

```
Input:
x print

Output:
Error: Undefined variable 'x'
```

---

## 🚀 Future Work (Day 4)

* Full `if-else` implementation using:

  * LLVM basic blocks
  * Conditional branching
* Control flow graph generation

---

## 🧠 Concepts Demonstrated

* Compiler Design
* Lexical Analysis
* Parsing
* Stack-based computation model
* LLVM IR generation
* Static Single Assignment (SSA)
* Code generation pipeline

---

## 🏁 Current Status

```
Day 1 ✅ Complete
Day 2 ✅ Complete
Day 3 ✅ Complete (LLVM Integration)
Day 4 🔜 Control Flow (if-else)
```

