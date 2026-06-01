#include "codegen.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/raw_ostream.h>

CodeGen::CodeGen()
    : module(std::make_unique<llvm::Module>("StackLangModule", context)),
      builder(context),
      mainFunction(nullptr),
      printfFunction(nullptr),
      stepCounter(1) {
}

llvm::Type* CodeGen::intType() {
    return llvm::Type::getInt32Ty(context);
}

bool CodeGen::ensureStackSize(int required, const std::string& operation) {
    if ((int)operandStack.size() < required) {
        std::cerr << "[CODEGEN ERROR] Stack underflow during " << operation
                  << ". Required: " << required
                  << ", Available: " << operandStack.size() << "\n";
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

std::string CodeGen::valueToString(llvm::Value* value) {
    if (!value) return "<null>";

    if (value->hasName()) {
        return "%" + value->getName().str();
    }

    std::string output;
    llvm::raw_string_ostream stream(output);
    value->print(stream);
    return stream.str();
}

std::string CodeGen::stackToString() {
    std::stack<llvm::Value*> temp = operandStack;
    std::vector<std::string> values;

    while (!temp.empty()) {
        values.push_back(valueToString(temp.top()));
        temp.pop();
    }

    std::ostringstream out;
    out << "[";

    for (int i = values.size() - 1; i >= 0; i--) {
        out << values[i];
        if (i != 0) out << ", ";
    }

    out << "]";
    return out.str();
}

void CodeGen::printTraceHeader() {
    std::cout << "\n====================================================================================================\n";
    std::cout << " LLVM IR GENERATION TRACE\n";
    std::cout << "====================================================================================================\n";
    std::cout << std::left
              << std::setw(6)  << "STEP"
              << std::setw(18) << "OPERATION"
              << std::setw(45) << "GENERATED LLVM IR"
              << "OPERAND STACK\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";
}

void CodeGen::printTraceRow(const std::string& operation, const std::string& irText) {
    std::cout << std::left
              << std::setw(6)  << stepCounter++
              << std::setw(18) << operation
              << std::setw(45) << irText
              << stackToString() << "\n";
}

void CodeGen::recordStackSSAMapping(
    const std::string& operation,
    const std::string& beforeStack,
    const std::string& afterStack,
    const std::string& ssaText
) {
    std::ostringstream out;

    out << std::left
        << std::setw(22) << operation
        << std::setw(42) << (beforeStack + " -> " + afterStack)
        << ssaText;

    stackSSAMapping.push_back(out.str());
}

void CodeGen::printCompactIR() {
    std::cout << "\n========================================\n";
    std::cout << " GENERATED LLVM IR INSTRUCTIONS\n";
    std::cout << "========================================\n";

    for (const std::string& line : compactIR) {
        std::cout << line << "\n";
    }
}

void CodeGen::printStackSSAMapping() {
    std::cout << "\n========================================\n";
    std::cout << " STACK TO SSA MAPPING\n";
    std::cout << "========================================\n";

    std::cout << std::left
              << std::setw(22) << "STACK OPERATION"
              << std::setw(42) << "STACK TRANSITION"
              << "LLVM / SSA VALUE\n";

    std::cout << "----------------------------------------------------------------------------------------------------\n";

    for (const std::string& line : stackSSAMapping) {
        std::cout << line << "\n";
    }
}

void CodeGen::pushNumber(const std::string& value) {
    std::string beforeStack = stackToString();

    int number = std::stoi(value);

    llvm::Value* numberValue =
        llvm::ConstantInt::get(intType(), number, true);

    operandStack.push(numberValue);

    std::string afterStack = stackToString();

    printTraceRow("PUSH " + value, "; constant");

    recordStackSSAMapping(
        "PUSH " + value,
        beforeStack,
        afterStack,
        valueToString(numberValue)
    );
}

void CodeGen::loadVariable(const std::string& name) {
    if (variables.find(name) == variables.end()) {
        std::cerr << "[CODEGEN ERROR] Undefined variable '" << name << "'\n";
        return;
    }

    std::string beforeStack = stackToString();

    llvm::Value* loadedValue =
        builder.CreateLoad(intType(), variables[name], name + "_load");

    std::string irLine = valueToString(loadedValue) + " = load i32, ptr %" + name;
    compactIR.push_back(irLine);

    operandStack.push(loadedValue);

    std::string afterStack = stackToString();

    printTraceRow("LOAD " + name, irLine);

    recordStackSSAMapping(
        "LOAD " + name,
        beforeStack,
        afterStack,
        irLine
    );
}

void CodeGen::storeVariable(const std::string& name) {
    if (!ensureStackSize(1, "store")) return;

    std::string beforeStack = stackToString();

    llvm::Value* value = operandStack.top();
    operandStack.pop();

    if (variables.find(name) == variables.end()) {
        variables[name] = createEntryBlockAlloca(name);
        compactIR.push_back("%" + name + " = alloca i32");
    }

    builder.CreateStore(value, variables[name]);

    std::string irLine = "store i32 " + valueToString(value) + ", ptr %" + name;
    compactIR.push_back(irLine);

    std::string afterStack = stackToString();

    printTraceRow("STORE " + name, irLine);

    recordStackSSAMapping(
        "STORE " + name,
        beforeStack,
        afterStack,
        irLine
    );
}

void CodeGen::emitArithmetic(OpType type) {
    if (!ensureStackSize(2, "arithmetic")) return;

    std::string beforeStack = stackToString();

    llvm::Value* right = operandStack.top();
    operandStack.pop();

    llvm::Value* left = operandStack.top();
    operandStack.pop();

    llvm::Value* result = nullptr;
    std::string operation;
    std::string llvmOp;

    switch (type) {
        case OpType::ADD:
            result = builder.CreateAdd(left, right, "addtmp");
            operation = "ADD";
            llvmOp = "add";
            break;

        case OpType::SUB:
            result = builder.CreateSub(left, right, "subtmp");
            operation = "SUB";
            llvmOp = "sub";
            break;

        case OpType::MUL:
            result = builder.CreateMul(left, right, "multmp");
            operation = "MUL";
            llvmOp = "mul";
            break;

        case OpType::DIV:
            result = builder.CreateSDiv(left, right, "divtmp");
            operation = "DIV";
            llvmOp = "sdiv";
            break;

        default:
            return;
    }

    std::string irLine =
        valueToString(result) + " = " + llvmOp + " i32 " +
        valueToString(left) + ", " + valueToString(right);

    compactIR.push_back(irLine);
    operandStack.push(result);

    std::string afterStack = stackToString();

    printTraceRow(operation, irLine);

    recordStackSSAMapping(
        operation,
        beforeStack,
        afterStack,
        irLine
    );
}

void CodeGen::emitComparison(OpType type) {
    if (!ensureStackSize(2, "comparison")) return;

    std::string beforeStack = stackToString();

    llvm::Value* right = operandStack.top();
    operandStack.pop();

    llvm::Value* left = operandStack.top();
    operandStack.pop();

    llvm::Value* cmp = nullptr;
    std::string operation;
    std::string pred;

    switch (type) {
        case OpType::GREATER:
            cmp = builder.CreateICmpSGT(left, right, "cmptmp");
            operation = "GREATER";
            pred = "sgt";
            break;

        case OpType::LESS:
            cmp = builder.CreateICmpSLT(left, right, "cmptmp");
            operation = "LESS";
            pred = "slt";
            break;

        case OpType::GREATER_EQUAL:
            cmp = builder.CreateICmpSGE(left, right, "cmptmp");
            operation = "GREATER_EQUAL";
            pred = "sge";
            break;

        case OpType::LESS_EQUAL:
            cmp = builder.CreateICmpSLE(left, right, "cmptmp");
            operation = "LESS_EQUAL";
            pred = "sle";
            break;

        case OpType::EQUAL:
            cmp = builder.CreateICmpEQ(left, right, "cmptmp");
            operation = "EQUAL";
            pred = "eq";
            break;

        case OpType::NOT_EQUAL:
            cmp = builder.CreateICmpNE(left, right, "cmptmp");
            operation = "NOT_EQUAL";
            pred = "ne";
            break;

        default:
            return;
    }

    std::string cmpLine =
        valueToString(cmp) + " = icmp " + pred + " i32 " +
        valueToString(left) + ", " + valueToString(right);

    llvm::Value* intResult =
        builder.CreateZExt(cmp, intType(), "booltoint");

    std::string zextLine =
        valueToString(intResult) + " = zext i1 " +
        valueToString(cmp) + " to i32";

    compactIR.push_back(cmpLine);
    compactIR.push_back(zextLine);

    operandStack.push(intResult);

    std::string afterStack = stackToString();

    printTraceRow(operation, cmpLine);

    recordStackSSAMapping(
        operation,
        beforeStack,
        afterStack,
        cmpLine + " ; " + zextLine
    );
}

void CodeGen::emitPrint() {
    if (!ensureStackSize(1, "print")) return;

    std::string beforeStack = stackToString();

    llvm::Value* value = operandStack.top();
    operandStack.pop();

    llvm::Value* formatString =
        builder.CreateGlobalStringPtr("%d\n", "fmt");

    builder.CreateCall(printfFunction, {formatString, value});

    std::string irLine = "call @printf(" + valueToString(value) + ")";
    compactIR.push_back(irLine);

    std::string afterStack = stackToString();

    printTraceRow("PRINT", irLine);

    recordStackSSAMapping(
        "PRINT",
        beforeStack,
        afterStack,
        irLine
    );
}

void CodeGen::emitIfElse(const Op& op) {
    if (!ensureStackSize(1, "if condition")) return;

    std::string beforeStack = stackToString();

    llvm::Value* conditionValue = operandStack.top();
    operandStack.pop();

    llvm::Value* zero =
        llvm::ConstantInt::get(intType(), 0, true);

    llvm::Value* condition =
        builder.CreateICmpNE(conditionValue, zero, "ifcond");

    std::string condLine =
        valueToString(condition) + " = icmp ne i32 " +
        valueToString(conditionValue) + ", 0";

    compactIR.push_back(condLine);

    std::string afterConditionStack = stackToString();

    recordStackSSAMapping(
        "IF_CONDITION",
        beforeStack,
        afterConditionStack,
        condLine
    );

    llvm::BasicBlock* thenBlock =
        llvm::BasicBlock::Create(context, "then", mainFunction);

    llvm::BasicBlock* elseBlock =
        llvm::BasicBlock::Create(context, "else");

    llvm::BasicBlock* mergeBlock =
        llvm::BasicBlock::Create(context, "ifcont");

    builder.CreateCondBr(condition, thenBlock, elseBlock);

    std::string branchLine = "br i1 " + valueToString(condition) +
                             ", label %then, label %else";

    compactIR.push_back(branchLine);

    printTraceRow("IF_CONDITION", branchLine);

    compactIR.push_back("then:");

    builder.SetInsertPoint(thenBlock);
    generateOps(op.thenOps);

    if (!builder.GetInsertBlock()->getTerminator()) {
        builder.CreateBr(mergeBlock);
        compactIR.push_back("br label %ifcont");
    }

    compactIR.push_back("else:");

    mainFunction->insert(mainFunction->end(), elseBlock);
    builder.SetInsertPoint(elseBlock);
    generateOps(op.elseOps);

    if (!builder.GetInsertBlock()->getTerminator()) {
        builder.CreateBr(mergeBlock);
        compactIR.push_back("br label %ifcont");
    }

    compactIR.push_back("ifcont:");

    mainFunction->insert(mainFunction->end(), mergeBlock);
    builder.SetInsertPoint(mergeBlock);

    printTraceRow("MERGE_IF", "; merge block");
}

void CodeGen::generateOp(const Op& op) {
    switch (op.type) {
        case OpType::PUSH_NUMBER:
            pushNumber(op.value);
            break;

        case OpType::LOAD_VARIABLE:
            loadVariable(op.value);
            break;

        case OpType::STORE_VARIABLE:
            storeVariable(op.value);
            break;

        case OpType::ADD:
        case OpType::SUB:
        case OpType::MUL:
        case OpType::DIV:
            emitArithmetic(op.type);
            break;

        case OpType::GREATER:
        case OpType::LESS:
        case OpType::GREATER_EQUAL:
        case OpType::LESS_EQUAL:
        case OpType::EQUAL:
        case OpType::NOT_EQUAL:
            emitComparison(op.type);
            break;

        case OpType::PRINT:
            emitPrint();
            break;

        case OpType::IF_ELSE:
            emitIfElse(op);
            break;
    }
}

void CodeGen::generateOps(const std::vector<Op>& ops) {
    for (const Op& op : ops) {
        generateOp(op);
    }
}

void CodeGen::generateIR(const std::vector<Op>& ops, const std::string& outputFile) {
    std::cout << "\n========================================\n";
    std::cout << " LLVM IR GENERATION PHASE\n";
    std::cout << "========================================\n";

    createPrintfFunction();
    createMainFunction();

    printTraceHeader();
    generateOps(ops);

    builder.CreateRet(llvm::ConstantInt::get(intType(), 0));

    if (llvm::verifyModule(*module, &llvm::errs())) {
        std::cerr << "[CODEGEN ERROR] LLVM module verification failed\n";
        return;
    }

    std::error_code errorCode;
    llvm::raw_fd_ostream outFile(outputFile, errorCode);

    if (errorCode) {
        std::cerr << "[CODEGEN ERROR] Could not write LLVM IR to " << outputFile << "\n";
        return;
    }

    module->print(outFile, nullptr);

    printCompactIR();
    printStackSSAMapping();

    std::cout << "\nLLVM IR file written to: " << outputFile << "\n";
}
