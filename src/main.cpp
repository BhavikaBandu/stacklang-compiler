#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer.h"
#include "parser.h"
#include "codegen.h"

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

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    std::cout << "===== TOKEN OUTPUT =====\n";

    for (const Token& token : tokens) {
        std::cout << tokenTypeToString(token.type);

        if (!token.value.empty()) {
            std::cout << "(" << token.value << ")";
        }

        std::cout << "\n";
    }

    Parser parser(tokens);
    parser.parse();

    CodeGen codegen(tokens);
    codegen.generateIR("output.ll");

    return 0;
}
