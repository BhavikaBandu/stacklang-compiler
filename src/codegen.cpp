#include "codegen.h"

#include <iostream>
#include <fstream>

#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/raw_ostream.h>

CodeGen::CodeGen(const std::vector<Token>& tokenList)
    : tokens(tokenList),
      pos(0),
      module(std::make_unique<llvm::Module>("StackLangModule", context)),
      builder(context),
      mainFunction(nullptr),
      printfFunction(nullptr) {
}

Token CodeGen::currentToken() {
    if (pos >= tokens.size()) {
        return {TokenType::END_OF_FILE, ""};
    }

    return tokens[pos];
}

Token CodeGen::peekToken() {
    if (pos + 1 >= tokens.size()) {
        return {TokenType::END_OF_FILE, ""};
    }

    return tokens[pos + 1];
}

void CodeGen::advance() {
    if (pos < tokens.size()) {
        pos++;
    }
}

llvm::Type* CodeGen::intType() {
    return llvm::Type::getInt32Ty(context);
}

bool CodeGen::ensureStackSize(int required, const std::string& operation) {
    if ((int)operandStack.size() < required) {
        std::cerr << "LLVM Error: Stack underflow during " << operation << "\n";
        return false;
    }

    return true;
}

void CodeGen::createMainFunction() {
    llvm::FunctionType* mainType =
        llvm::FunctionType::get(intType(), false);

    mainFunction =
        llvm::Function::Create(
            mainType,
            llvm::Function::ExternalLinkage,
            "main",
            module.get()
        );

    llvm::BasicBlock* entryBlock =
        llvm::BasicBlock::Create(context, "entry", mainFunction);

    builder.SetInsertPoint(entryBlock);
}

void CodeGen::createPrintfFunction() {
    std::vector<llvm::Type*> printfArgs;
    printfArgs.push_back(llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));

    llvm::FunctionType* printfType =
        llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context),
            printfArgs,
            true
        );

    printfFunction =
        llvm::Function::Create(
            printfType,
            llvm::Function::ExternalLinkage,
            "printf",
            module.get()
        );
}

llvm::AllocaInst* CodeGen::createEntryBlockAlloca(const std::string& varName) {
    llvm::IRBuilder<> tempBuilder(
        &mainFunction->getEntryBlock(),
        mainFunction->getEntryBlock().begin()
    );

    return tempBuilder.CreateAlloca(intType(), nullptr, varName);
}

void CodeGen::pushNumber(const Token& token) {
    int value = std::stoi(token.value);

    llvm::Value* numberValue =
        llvm::ConstantInt::get(intType(), value, true);

    operandStack.push(numberValue);

    std::cout << "[LLVM] PUSH NUMBER " << value << "\n";
}

void CodeGen::pushVariable(const Token& token) {
    std::string name = token.value;

    if (peekToken().type == TokenType::ASSIGN) {
        std::cout << "[LLVM] Identifier '" << name << "' waiting for assignment\n";
        return;
    }

    if (variables.find(name) == variables.end()) {
        std::cerr << "LLVM Error: Undefined variable '" << name << "'\n";
        return;
    }

    llvm::Value* loadedValue =
        builder.CreateLoad(
            intType(),
            variables[name],
            name + "_load"
        );

    operandStack.push(loadedValue);

    std::cout << "[LLVM] LOAD VARIABLE " << name << "\n";
}

void CodeGen::handleArithmetic(TokenType type) {
    if (!ensureStackSize(2, "arithmetic operation")) {
        return;
    }

    llvm::Value* right = operandStack.top();
    operandStack.pop();

    llvm::Value* left = operandStack.top();
    operandStack.pop();

    llvm::Value* result = nullptr;

    switch (type) {
        case TokenType::PLUS:
            result = builder.CreateAdd(left, right, "addtmp");
            std::cout << "[LLVM] ADD\n";
            break;

        case TokenType::MINUS:
            result = builder.CreateSub(left, right, "subtmp");
            std::cout << "[LLVM] SUB\n";
            break;

        case TokenType::STAR:
            result = builder.CreateMul(left, right, "multmp");
            std::cout << "[LLVM] MUL\n";
            break;

        case TokenType::SLASH:
            result = builder.CreateSDiv(left, right, "divtmp");
            std::cout << "[LLVM] DIV\n";
            break;

        default:
            std::cerr << "LLVM Error: Unknown arithmetic operator\n";
            return;
    }

    operandStack.push(result);
}

void CodeGen::handleComparison(TokenType type) {
    if (!ensureStackSize(2, "comparison operation")) {
        return;
    }

    llvm::Value* right = operandStack.top();
    operandStack.pop();

    llvm::Value* left = operandStack.top();
    operandStack.pop();

    llvm::Value* cmpResult = nullptr;

    switch (type) {
        case TokenType::GREATER:
            cmpResult = builder.CreateICmpSGT(left, right, "cmptmp");
            std::cout << "[LLVM] GREATER THAN\n";
            break;

        case TokenType::LESS:
            cmpResult = builder.CreateICmpSLT(left, right, "cmptmp");
            std::cout << "[LLVM] LESS THAN\n";
            break;

        case TokenType::EQUAL_EQUAL:
            cmpResult = builder.CreateICmpEQ(left, right, "cmptmp");
            std::cout << "[LLVM] EQUAL EQUAL\n";
            break;

        default:
            std::cerr << "LLVM Error: Unknown comparison operator\n";
            return;
    }

    llvm::Value* intResult =
        builder.CreateZExt(cmpResult, intType(), "booltoint");

    operandStack.push(intResult);
}

void CodeGen::handleAssignment() {
    if (pos == 0) {
        std::cerr << "LLVM Error: Assignment without variable name\n";
        return;
    }

    Token previous = tokens[pos - 1];

    if (previous.type != TokenType::IDENTIFIER) {
        std::cerr << "LLVM Error: Assignment must come after variable name\n";
        return;
    }

    if (!ensureStackSize(1, "assignment")) {
        return;
    }

    llvm::Value* value = operandStack.top();
    operandStack.pop();

    std::string varName = previous.value;

    if (variables.find(varName) == variables.end()) {
        variables[varName] = createEntryBlockAlloca(varName);
        std::cout << "[LLVM] ALLOCA VARIABLE " << varName << "\n";
    }

    builder.CreateStore(value, variables[varName]);

    std::cout << "[LLVM] STORE VARIABLE " << varName << "\n";
}

void CodeGen::handlePrint() {
    if (!ensureStackSize(1, "print")) {
        return;
    }

    llvm::Value* value = operandStack.top();
    operandStack.pop();

    llvm::Value* formatString =
        builder.CreateGlobalStringPtr("%d\n", "fmt");

    builder.CreateCall(printfFunction, {formatString, value});

    std::cout << "[LLVM] PRINT\n";
}

void CodeGen::generateIR(const std::string& outputFile) {
    std::cout << "\n===== LLVM IR GENERATION =====\n";

    createPrintfFunction();
    createMainFunction();

    while (currentToken().type != TokenType::END_OF_FILE) {
        Token token = currentToken();

        switch (token.type) {
            case TokenType::NUMBER:
                pushNumber(token);
                break;

            case TokenType::IDENTIFIER:
                pushVariable(token);
                break;

            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::STAR:
            case TokenType::SLASH:
                handleArithmetic(token.type);
                break;

            case TokenType::GREATER:
            case TokenType::LESS:
            case TokenType::EQUAL_EQUAL:
                handleComparison(token.type);
                break;

            case TokenType::ASSIGN:
                handleAssignment();
                break;

            case TokenType::PRINT:
                handlePrint();
                break;

            case TokenType::IF:
            case TokenType::ELSE:
            case TokenType::ENDIF:
                std::cout << "[LLVM] IF-ELSE will be implemented on Day 4\n";
                break;

            case TokenType::UNKNOWN:
                std::cerr << "LLVM Error: Unknown token '" << token.value << "'\n";
                break;

            default:
                break;
        }

        advance();
    }

    builder.CreateRet(llvm::ConstantInt::get(intType(), 0));

    if (llvm::verifyModule(*module, &llvm::errs())) {
        std::cerr << "LLVM Error: Module verification failed\n";
        return;
    }

    std::error_code errorCode;
    llvm::raw_fd_ostream outFile(outputFile, errorCode);

    if (errorCode) {
        std::cerr << "Error: Could not write LLVM IR to file " << outputFile << "\n";
        return;
    }

    module->print(outFile, nullptr);

    std::cout << "LLVM IR written to " << outputFile << "\n";
    std::cout << "===== LLVM IR GENERATION COMPLETE =====\n";
}
