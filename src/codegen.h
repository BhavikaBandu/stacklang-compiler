#ifndef CODEGEN_H
#define CODEGEN_H

#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <memory>
#include "lexer.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>

class CodeGen {
private:
    std::vector<Token> tokens;
    size_t pos;

    llvm::LLVMContext context;
    std::unique_ptr<llvm::Module> module;
    llvm::IRBuilder<> builder;

    llvm::Function* mainFunction;
    llvm::Function* printfFunction;

    std::stack<llvm::Value*> operandStack;
    std::unordered_map<std::string, llvm::AllocaInst*> variables;

    Token currentToken();
    Token peekToken();
    void advance();

    llvm::Type* intType();
    llvm::Type* boolType();

    void createMainFunction();
    void createPrintfFunction();

    llvm::AllocaInst* createEntryBlockAlloca(const std::string& varName);

    void pushNumber(const Token& token);
    void pushVariable(const Token& token);

    void handleArithmetic(TokenType type);
    void handleComparison(TokenType type);
    void handleAssignment();
    void handlePrint();
    void handleIf();

    void generateBlock(bool stopAtElseOrEndif);

    bool ensureStackSize(int required, const std::string& operation);

public:
    CodeGen(const std::vector<Token>& tokenList);
    void generateIR(const std::string& outputFile);
};

#endif
