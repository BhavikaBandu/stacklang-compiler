#ifndef CODEGEN_H
#define CODEGEN_H

#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <memory>

#include "instructions.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>

class CodeGen {
private:
    llvm::LLVMContext context;
    std::unique_ptr<llvm::Module> module;
    llvm::IRBuilder<> builder;

    llvm::Function* mainFunction;
    llvm::Function* printfFunction;

    std::stack<llvm::Value*> operandStack;
    std::unordered_map<std::string, llvm::AllocaInst*> variables;

    int stepCounter;
    std::vector<std::string> compactIR;
    std::vector<std::string> stackSSAMapping;

    llvm::Type* intType();

    void createMainFunction();
    void createPrintfFunction();

    llvm::AllocaInst* createEntryBlockAlloca(const std::string& varName);

    void generateOps(const std::vector<Op>& ops);
    void generateOp(const Op& op);

    void pushNumber(const std::string& value);
    void loadVariable(const std::string& name);
    void storeVariable(const std::string& name);

    void emitArithmetic(OpType type);
    void emitComparison(OpType type);
    void emitPrint();
    void emitIfElse(const Op& op);

    bool ensureStackSize(int required, const std::string& operation);

    std::string valueToString(llvm::Value* value);
    std::string stackToString();

    void printTraceRow(const std::string& operation, const std::string& irText);
    void recordStackSSAMapping(
        const std::string& operation,
        const std::string& beforeStack,
        const std::string& afterStack,
        const std::string& ssaText
    );

    void printTraceHeader();
    void printCompactIR();
    void printStackSSAMapping();

public:
    CodeGen();
    void generateIR(const std::vector<Op>& ops, const std::string& outputFile);
};

#endif
