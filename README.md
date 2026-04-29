# StackLang Compiler

## 📌 Overview

This project implements a **mini compiler for a stack-based programming language**.
The compiler processes source code written in a custom language (**StackLang**) and performs:

* Lexical Analysis (Lexer)
* Parsing
* Stack-based execution simulation

Future phases will extend this into **LLVM IR generation and native execution**.

---

## 🧠 Key Idea

Unlike traditional compilers that use expression trees (ASTs), this project uses a:

> **Stack-based execution model**

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

## ⚙️ Features Implemented (Day 1 & Day 2)

### ✅ Day 1 – Lexer

* Custom lexer (no external tools like Flex)
* Tokenizes:

  * Numbers
  * Identifiers (variables)
  * Operators: `+ - * /`
  * Comparisons: `> < ==`
  * Assignment: `=`
  * Keywords: `if`, `else`, `endif`, `print`
* File-based input support
* Token output display

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
* Basic error handling:

  * Stack underflow
  * Division by zero
  * Undefined variables

---

## 📂 Project Structure

```
stacklang-compiler/
│
├── src/
│   ├── main.cpp        # Entry point
│   ├── lexer.h         # Token definitions & lexer interface
│   ├── lexer.cpp       # Lexer implementation
│   ├── parser.h        # Parser interface
│   └── parser.cpp      # Parser + stack engine
│
├── samples/
│   ├── test.stack
│   ├── arithmetic.stack
│   ├── error.stack
│   └── undefined.stack
│
└── README.md
```

---

## 🧪 Sample Programs

### Example 1: Variables

```
5 3 + x =
x 2 * y =
y print
```

Output:

```
PRINT: 16
```

---

### Example 2: Arithmetic

```
10 5 + print
20 4 / print
7 3 - print
6 8 * print
```

Output:

```
PRINT: 15
PRINT: 5
PRINT: 4
PRINT: 48
```

---

## 🔄 Execution Flow

```
Source Code
   ↓
Lexer (Tokenization)
   ↓
Parser (Instruction Processing)
   ↓
Stack Execution
   ↓
Output
```

---

## 📊 Example Stack Trace

```
[STACK] PUSH NUMBER 10 -> [10]
[STACK] PUSH NUMBER 5 -> [10, 5]
[STACK] ADD 10 and 5 -> [15]
PRINT: 15
[STACK] PRINT POP -> []
```

---

## 🛠️ Compilation & Execution

### Compile:

```
clang++ src/main.cpp src/lexer.cpp src/parser.cpp -o stack_compiler
```

### Run:

```
./stack_compiler samples/test.stack
```

---

## ⚠️ Error Handling

### Stack Underflow

```
Input:
5 +

Output:
Error: Stack underflow during arithmetic operation
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

## 🚀 Future Work (Day 3 & Day 4)

* LLVM IR generation
* Conversion from stack model → SSA form
* Native executable generation using `clang`
* Full if-else implementation using LLVM basic blocks

---

## 🧠 Concepts Demonstrated

* Compiler Design Basics
* Lexical Analysis
* Parsing Techniques
* Stack-based Execution Model
* Intermediate Representation (upcoming: LLVM IR)

---

## 🏁 Current Status

```
Day 1 ✅ Complete
Day 2 ✅ Complete
Day 3 🔜 LLVM IR Generation
Day 4 🔜 Control Flow (if-else)
```

