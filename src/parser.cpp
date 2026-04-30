#include "parser.h"
#include <iostream>

Parser::Parser(const std::vector<Token>& tokenList) {
    tokens = tokenList;
    pos = 0;
}

Token Parser::currentToken() {
    if (pos >= tokens.size()) {
        return {TokenType::END_OF_FILE, ""};
    }
    return tokens[pos];
}

Token Parser::peekToken() {
    if (pos + 1 >= tokens.size()) {
        return {TokenType::END_OF_FILE, ""};
    }
    return tokens[pos + 1];
}

void Parser::advance() {
    if (pos < tokens.size()) {
        pos++;
    }
}

bool Parser::ensureStackSize(int required, const std::string& operation) {
    if ((int)operandStack.size() < required) {
        std::cerr << "Error: Stack underflow during " << operation << "\n";
        return false;
    }
    return true;
}

void Parser::printStackTrace(const std::string& action) {
    std::stack<int> temp = operandStack;
    std::vector<int> values;

    while (!temp.empty()) {
        values.push_back(temp.top());
        temp.pop();
    }

    std::cout << "[STACK] " << action << " -> [";

    for (int i = values.size() - 1; i >= 0; i--) {
        std::cout << values[i];
        if (i != 0) {
            std::cout << ", ";
        }
    }

    std::cout << "]\n";
}

void Parser::pushNumber(const Token& token) {
    int value = std::stoi(token.value);
    operandStack.push(value);
    printStackTrace("PUSH NUMBER " + token.value);
}

void Parser::pushVariable(const Token& token) {
    std::string name = token.value;

    if (peekToken().type == TokenType::ASSIGN) {
        std::cout << "[PARSER] Identifier '" << name << "' marked for assignment\n";
        return;
    }

    if (variables.find(name) == variables.end()) {
        std::cerr << "Error: Undefined variable '" << name << "'\n";
        return;
    }

    operandStack.push(variables[name]);
    printStackTrace("LOAD VARIABLE " + name + " = " + std::to_string(variables[name]));
}

void Parser::handleArithmetic(TokenType type) {
    if (!ensureStackSize(2, "arithmetic operation")) {
        return;
    }

    int right = operandStack.top();
    operandStack.pop();

    int left = operandStack.top();
    operandStack.pop();

    int result = 0;
    std::string opName;

    switch (type) {
        case TokenType::PLUS:
            result = left + right;
            opName = "ADD";
            break;

        case TokenType::MINUS:
            result = left - right;
            opName = "SUB";
            break;

        case TokenType::STAR:
            result = left * right;
            opName = "MUL";
            break;

        case TokenType::SLASH:
            if (right == 0) {
                std::cerr << "Error: Division by zero\n";
                return;
            }
            result = left / right;
            opName = "DIV";
            break;

        default:
            std::cerr << "Error: Unknown arithmetic operator\n";
            return;
    }

    operandStack.push(result);
    printStackTrace(opName + " " + std::to_string(left) + " and " + std::to_string(right));
}

void Parser::handleComparison(TokenType type) {
    if (!ensureStackSize(2, "comparison operation")) {
        return;
    }

    int right = operandStack.top();
    operandStack.pop();

    int left = operandStack.top();
    operandStack.pop();

    int result = 0;
    std::string opName;

    switch (type) {
        case TokenType::GREATER:
            result = left > right;
            opName = "GREATER";
            break;

        case TokenType::LESS:
            result = left < right;
            opName = "LESS";
            break;

        case TokenType::EQUAL_EQUAL:
            result = left == right;
            opName = "EQUAL_EQUAL";
            break;

        default:
            std::cerr << "Error: Unknown comparison operator\n";
            return;
    }

    operandStack.push(result);
    printStackTrace(opName + " " + std::to_string(left) + " and " + std::to_string(right));
}

void Parser::handleAssignment() {
    if (pos == 0) {
        std::cerr << "Error: Assignment without variable name\n";
        return;
    }

    Token previous = tokens[pos - 1];

    if (previous.type != TokenType::IDENTIFIER) {
        std::cerr << "Error: Assignment must come after variable name\n";
        return;
    }

    if (!ensureStackSize(1, "assignment")) {
        return;
    }

    int value = operandStack.top();
    operandStack.pop();

    variables[previous.value] = value;

    std::cout << "[ASSIGN] " << previous.value << " = " << value << "\n";
    printStackTrace("STORE VARIABLE " + previous.value);
}

void Parser::handlePrint() {
    if (!ensureStackSize(1, "print")) {
        return;
    }

    int value = operandStack.top();
    operandStack.pop();

    std::cout << "PRINT: " << value << "\n";
    printStackTrace("PRINT POP");
}

void Parser::handleIf() {
    if (!ensureStackSize(1, "if condition")) {
        return;
    }

    int condition = operandStack.top();
    operandStack.pop();

    std::cout << "[IF] Condition value = " << condition << "\n";

    advance();

    if (condition != 0) {
        std::cout << "[IF] Executing THEN block\n";
        parseBlock(true, true);

        if (currentToken().type == TokenType::ELSE) {
            advance();
            parseBlock(false, true);
        }
    } else {
        std::cout << "[IF] Skipping THEN block\n";
        parseBlock(false, true);

        if (currentToken().type == TokenType::ELSE) {
            std::cout << "[IF] Executing ELSE block\n";
            advance();
            parseBlock(true, true);
        }
    }

    if (currentToken().type == TokenType::ENDIF) {
        std::cout << "[IF] ENDIF reached\n";
    } else {
        std::cerr << "Error: Missing endif\n";
    }
}

void Parser::parseBlock(bool execute, bool stopAtElseOrEndif) {
    while (currentToken().type != TokenType::END_OF_FILE) {
        Token token = currentToken();

        if (stopAtElseOrEndif &&
            (token.type == TokenType::ELSE || token.type == TokenType::ENDIF)) {
            return;
        }

        if (!execute) {
            if (token.type == TokenType::IF) {
                advance();
                parseBlock(false, true);

                if (currentToken().type == TokenType::ELSE) {
                    advance();
                    parseBlock(false, true);
                }
            }

            advance();
            continue;
        }

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
                handleIf();
                break;

            case TokenType::UNKNOWN:
                std::cerr << "Error: Unknown token '" << token.value << "'\n";
                break;

            default:
                break;
        }

        advance();
    }
}

void Parser::parse() {
    std::cout << "\n===== PARSER + STACK TRACE =====\n";
    parseBlock(true, false);
    std::cout << "===== PARSING COMPLETE =====\n";
}
