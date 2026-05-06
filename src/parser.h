#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <unordered_set>

#include "lexer.h"
#include "instructions.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t pos;

    int stackDepth;
    std::unordered_set<std::string> symbols;
    bool hadError;

    Token currentToken();
    Token peekToken();
    void advance();

    bool requireStack(int count, const std::string& operation);
    void pushStack();
    void popStack();
    void replaceBinaryOp();

    std::vector<Op> parseBlock(bool stopAtElseOrEndif);

    void printInstructionList(const std::vector<Op>& ops, int indent = 0);
    void printOp(const Op& op, int indent);

public:
    Parser(const std::vector<Token>& tokenList);

    std::vector<Op> parseProgram();
    bool hasError() const;

    void printInstructions(const std::vector<Op>& ops);
};

#endif
