# Implementation Details

## Overview

The compiler is implemented in C++ using LLVM libraries.

Core source files:

| File | Responsibility |
|--------|---------------|
| lexer.cpp | Lexical analysis |
| parser.cpp | Parsing and validation |
| instructions.h | Intermediate instruction representation |
| codegen.cpp | LLVM IR generation |
| main.cpp | Driver program |

---

## Lexical Analysis

The lexer converts source code into tokens.

Example:

~~~~text
10
20
+
print
~~~~

Generated tokens:

~~~~text
NUMBER(10)
NUMBER(20)
PLUS
PRINT
~~~~

Supported token classes:

- NUMBER
- IDENTIFIER
- Operators
- Assignment
- Keywords

---

## Intermediate Representation

The parser generates a vector of operations.

~~~~cpp
std::vector<Op>
~~~~

Instruction types:

~~~~cpp
PUSH_NUMBER
LOAD_VARIABLE
STORE_VARIABLE
ADD
SUB
MUL
DIV
GREATER
LESS
GREATER_EQUAL
LESS_EQUAL
EQUAL
NOT_EQUAL
PRINT
IF_ELSE
~~~~

This representation serves as the compiler's intermediate form.

---

## Parser Implementation

The parser performs two tasks simultaneously:

1. Instruction generation
2. Static stack verification

The parser maintains:

~~~~cpp
int stackDepth;
~~~~

Arithmetic instructions require:

~~~~cpp
requireStack(2)
~~~~

Print requires:

~~~~cpp
requireStack(1)
~~~~

This catches invalid programs before LLVM generation.

---

## LLVM Infrastructure

The compiler uses:

~~~~cpp
LLVMContext
Module
IRBuilder<>
Function
BasicBlock
~~~~

The main function is generated automatically:

~~~~llvm
define i32 @main()
~~~~

A declaration for printf is also generated:

~~~~llvm
declare i32 @printf(...)
~~~~

---

## Operand Stack Representation

Instead of storing integers, the compiler stores:

~~~~cpp
std::stack<llvm::Value*>
~~~~

Each stack element represents an SSA value.

~~~~cpp
std::stack<llvm::Value*> operandStack;
~~~~

---

## Stack to SSA Mapping

Consider:

~~~~text
10
20
+
~~~~

Generated LLVM:

~~~~llvm
%addtmp = add i32 10, 20
~~~~

Stack evolution:

~~~~text
[10]
[10,20]
[%addtmp]
~~~~

The operand stack therefore contains SSA values rather than concrete runtime values.

This is how stack-machine semantics are translated into LLVM's SSA form.

---

## Arithmetic Generation

Addition:

~~~~cpp
builder.CreateAdd(left, right, "addtmp");
~~~~

Subtraction:

~~~~cpp
builder.CreateSub(left, right, "subtmp");
~~~~

Multiplication:

~~~~cpp
builder.CreateMul(left, right, "multmp");
~~~~

Division:

~~~~cpp
builder.CreateSDiv(left, right, "divtmp");
~~~~

---

## Variable Generation

Variables are allocated in the entry block.

~~~~cpp
CreateAlloca(...)
~~~~

Store:

~~~~llvm
store i32 value, ptr %x
~~~~

Load:

~~~~llvm
%x_load = load i32, ptr %x
~~~~

---

## Comparison Generation

Comparisons generate LLVM i1 values.

Example:

~~~~llvm
%cmp = icmp sgt i32 %a, %b
~~~~

The boolean result is converted to i32:

~~~~llvm
%booltoint = zext i1 %cmp to i32
~~~~

This allows comparison results to remain compatible with the operand stack.

---

## Print Generation

Printing is implemented using:

~~~~cpp
printf("%d\n", value);
~~~~

LLVM IR:

~~~~llvm
call @printf(...)
~~~~

---

## If-Else Generation

The compiler creates three LLVM basic blocks:

~~~~text
then
else
ifcont
~~~~

Control flow:

~~~~text
entry
  |
 condition
 /       \
then     else
 \       /
  ifcont
~~~~

Condition generation:

~~~~llvm
icmp ne i32 conditionValue, 0
~~~~

Branch generation:

~~~~llvm
br i1 %cond, label %then, label %else
~~~~

---

## LLVM Verification

Before writing output:

~~~~cpp
verifyModule(...)
~~~~

This ensures the generated module is valid LLVM IR.

---