#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include "lexer.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t pos;

    std::stack<int> operandStack;
    std::unordered_map<std::string, int> variables;

    Token currentToken();
    Token peekToken();
    void advance();

    void pushNumber(const Token& token);
    void pushVariable(const Token& token);

    void handleArithmetic(TokenType type);
    void handleComparison(TokenType type);
    void handleAssignment();
    void handlePrint();

    void printStackTrace(const std::string& action);
    bool ensureStackSize(int required, const std::string& operation);

public:
    Parser(const std::vector<Token>& tokenList);
    void parse();
};

#endif
