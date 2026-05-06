#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "instructions.h"

void printHeader(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << " " << title << "\n";
    std::cout << "========================================\n";
}

void printTokens(const std::vector<Token>& tokens) {
    printHeader("LEXER PHASE");

    std::cout << std::left << std::setw(6) << "NO."
              << std::setw(22) << "TOKEN TYPE"
              << "VALUE\n";

    std::cout << "----------------------------------------\n";

    int index = 1;

    for (const Token& token : tokens) {
        if (token.type == TokenType::END_OF_FILE) {
            continue;
        }

        std::cout << std::left << std::setw(6) << index++
                  << std::setw(22) << tokenTypeToString(token.type)
                  << token.value << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./stack_compiler <source_file>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    printHeader("STACKLANG COMPILER");
    std::cout << "Source file: " << argv[1] << "\n";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    printTokens(tokens);

    Parser parser(tokens);
    std::vector<Op> instructions = parser.parseProgram();

    parser.printInstructions(instructions);

    if (parser.hasError()) {
        std::cerr << "\nCompilation stopped due to parser errors.\n";
        return 1;
    }

    CodeGen codegen;
    codegen.generateIR(instructions, "output.ll");

    printHeader("COMPILATION COMPLETE");

    std::cout << "Generated file : output.ll\n";
    std::cout << "Compile IR     : clang output.ll -o program\n";
    std::cout << "Run executable : ./program\n";

    return 0;
}
