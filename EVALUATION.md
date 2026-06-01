# Evaluation

## Evaluation Objectives

The evaluation focuses on:

1. Correctness
2. Error handling
3. LLVM IR generation
4. Native executable generation

---

## Test Environment

Compiler:

~~~~text
clang
LLVM
g++
~~~~

Platform:

~~~~text
Ubuntu Linux
~~~~
---

## Test Cases

| Test Case | Description | Expected Result |
|------------|------------|----------------|
| 01_arithmetic | Arithmetic operations | Correct numerical output |
| 02_variables | Variable assignment and access | Correct variable handling |
| 03_ifelse_true_false | Conditional branching | Correct branch execution |
| 04_error_stack_underflow | Invalid stack usage | Error reported |
| 05_error_undefined_variable | Undefined variable access | Error reported |
| 06_error_missing_endif | Missing endif | Error reported |

---

## Results

| Test Case | Status |
|------------|---------|
| 01_arithmetic | PASS |
| 02_variables | PASS |
| 03_ifelse_true_false | PASS |
| 04_error_stack_underflow | PASS |
| 05_error_undefined_variable | PASS |
| 06_error_missing_endif | PASS |

Overall:

~~~~text
Passed: 6 / 6
Success Rate: 100%
~~~~

---

## Baseline Comparison

### Baseline

Simple Stack Interpreter

Characteristics:

- Executes instructions directly
- No LLVM IR generation
- No native compilation

### Proposed System

LLVM-Based Compiler

Characteristics:

- Generates LLVM IR
- Produces native executables
- Uses LLVM infrastructure
- Verifies generated IR

| Metric | Interpreter | LLVM Compiler |
|----------|-------------|---------------|
| Generates LLVM IR | No | Yes |
| Produces Native Binary | No | Yes |
| Uses SSA Representation | No | Yes |
| LLVM Verification | No | Yes |
| Native Execution | No | Yes |

---

## Error Handling Evaluation

### Stack Underflow

Input:

~~~~text
+
~~~~

Result:

~~~~text
[PARSER ERROR] Stack underflow during addition
~~~~

### Undefined Variable

Input:

~~~~text
x
~~~~

Result:

~~~~text
[PARSER ERROR] Undefined variable 'x'
~~~~

### Missing Endif

Input:

~~~~text
if
...
~~~~

Result:

~~~~text
[PARSER ERROR] Missing endif for if statement
~~~~

---

## LLVM IR Correctness

Generated modules are validated using:

~~~~cpp
llvm::verifyModule(...)
~~~~

Verification status:

~~~~text
PASS
~~~~

---