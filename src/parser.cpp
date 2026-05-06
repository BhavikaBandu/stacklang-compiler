#include "parser.h"
#include <iostream>
#include <iomanip>

std::string opTypeToString(OpType type) {
    switch (type) {
        case OpType::PUSH_NUMBER: return "PUSH_NUMBER";
        case OpType::LOAD_VARIABLE: return "LOAD_VARIABLE";
        case OpType::STORE_VARIABLE: return "STORE_VARIABLE";

        case OpType::ADD: return "ADD";
        case OpType::SUB: return "SUB";
        case OpType::MUL: return "MUL";
        case OpType::DIV: return "DIV";

        case OpType::GREATER: return "GREATER";
        case OpType::LESS: return "LESS";
        case OpType::GREATER_EQUAL: return "GREATER_EQUAL";
        case OpType::LESS_EQUAL: return "LESS_EQUAL";
        case OpType::EQUAL: return "EQUAL";
        case OpType::NOT_EQUAL: return "NOT_EQUAL";

        case OpType::PRINT: return "PRINT";
        case OpType::IF_ELSE: return "IF_ELSE";
    }

    return "UNKNOWN_OP";
}

Parser::Parser(const std::vector<Token>& tokenList) {
    tokens = tokenList;
    pos = 0;
    stackDepth = 0;
    hadError = false;
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

bool Parser::hasError() const {
    return hadError;
}

bool Parser::requireStack(int count, const std::string& operation) {
    if (stackDepth < count) {
        std::cerr << "[PARSER ERROR] Stack underflow during " << operation
                  << ". Required: " << count
                  << ", Available: " << stackDepth << "\n";
        hadError = true;
        return false;
    }

    return true;
}

void Parser::pushStack() {
    stackDepth++;
}

void Parser::popStack() {
    if (stackDepth > 0) {
        stackDepth--;
    }
}

void Parser::replaceBinaryOp() {
    stackDepth -= 2;
    stackDepth += 1;
}

std::vector<Op> Parser::parseProgram() {
    std::cout << "\n========================================\n";
    std::cout << " PARSER PHASE\n";
    std::cout << "========================================\n";
    std::cout << "Generating stack-instruction list...\n";

    std::vector<Op> ops = parseBlock(false);

    if (!hadError) {
        std::cout << "Parser completed successfully.\n";
    } else {
        std::cout << "Parser completed with errors.\n";
    }

    return ops;
}

std::vector<Op> Parser::parseBlock(bool stopAtElseOrEndif) {
    std::vector<Op> ops;

    while (currentToken().type != TokenType::END_OF_FILE) {
        Token token = currentToken();

        if (stopAtElseOrEndif &&
            (token.type == TokenType::ELSE || token.type == TokenType::ENDIF)) {
            return ops;
        }

        switch (token.type) {
            case TokenType::NUMBER:
                ops.push_back(Op(OpType::PUSH_NUMBER, token.value));
                pushStack();
                advance();
                break;

            case TokenType::IDENTIFIER: {
                std::string name = token.value;

                if (peekToken().type == TokenType::ASSIGN) {
                    advance();
                    break;
                }

                if (symbols.find(name) == symbols.end()) {
                    std::cerr << "[PARSER ERROR] Undefined variable '" << name << "'\n";
                    hadError = true;
                }

                ops.push_back(Op(OpType::LOAD_VARIABLE, name));
                pushStack();
                advance();
                break;
            }

            case TokenType::ASSIGN: {
                if (pos == 0 || tokens[pos - 1].type != TokenType::IDENTIFIER) {
                    std::cerr << "[PARSER ERROR] Assignment must follow an identifier\n";
                    hadError = true;
                    advance();
                    break;
                }

                std::string name = tokens[pos - 1].value;

                if (requireStack(1, "assignment")) {
                    ops.push_back(Op(OpType::STORE_VARIABLE, name));
                    symbols.insert(name);
                    popStack();
                }

                advance();
                break;
            }

            case TokenType::PLUS:
                if (requireStack(2, "addition")) {
                    ops.push_back(Op(OpType::ADD));
                    replaceBinaryOp();
                }
                advance();
                break;

            case TokenType::MINUS:
                if (requireStack(2, "subtraction")) {
                    ops.push_back(Op(OpType::SUB));
                    replaceBinaryOp();
                }
                advance();
                break;

            case TokenType::STAR:
                if (requireStack(2, "multiplication")) {
                    ops.push_back(Op(OpType::MUL));
                    replaceBinaryOp();
                }
                advance();
                break;

            case TokenType::SLASH:
                if (requireStack(2, "division")) {
                    ops.push_back(Op(OpType::DIV));
                    replaceBinaryOp();
                }
                advance();
                break;

            case TokenType::GREATER:
                if (requireStack(2, "greater-than comparison")) {
                    ops.push_back(Op(OpType::GREATER));
                    replaceBinaryOp();
                }
                advance();
                break;

            case TokenType::LESS:
                if (requireStack(2, "less-than comparison")) {
                    ops.push_back(Op(OpType::LESS));
                    replaceBinaryOp();
                }
                advance();
                break;

            case TokenType::GREATER_EQUAL:
                if (requireStack(2, "greater-equal comparison")) {
                    ops.push_back(Op(OpType::GREATER_EQUAL));
                    replaceBinaryOp();
                }
                advance();
                break;

            case TokenType::LESS_EQUAL:
                if (requireStack(2, "less-equal comparison")) {
                    ops.push_back(Op(OpType::LESS_EQUAL));
                    replaceBinaryOp();
                }
                advance();
                break;

            case TokenType::EQUAL_EQUAL:
                if (requireStack(2, "equality comparison")) {
                    ops.push_back(Op(OpType::EQUAL));
                    replaceBinaryOp();
                }
                advance();
                break;

            case TokenType::NOT_EQUAL:
                if (requireStack(2, "not-equal comparison")) {
                    ops.push_back(Op(OpType::NOT_EQUAL));
                    replaceBinaryOp();
                }
                advance();
                break;

            case TokenType::PRINT:
                if (requireStack(1, "print")) {
                    ops.push_back(Op(OpType::PRINT));
                    popStack();
                }
                advance();
                break;

            case TokenType::IF: {
                if (!requireStack(1, "if condition")) {
                    advance();
                    break;
                }

                popStack();
                advance();

                int stackBeforeThen = stackDepth;

                std::vector<Op> thenOps = parseBlock(true);
                int stackAfterThen = stackDepth;

                std::vector<Op> elseOps;
                int stackAfterElse = stackAfterThen;

                if (currentToken().type == TokenType::ELSE) {
                    advance();

                    stackDepth = stackBeforeThen;
                    elseOps = parseBlock(true);
                    stackAfterElse = stackDepth;
                }

                if (currentToken().type != TokenType::ENDIF) {
                    std::cerr << "[PARSER ERROR] Missing endif for if statement\n";
                    hadError = true;
                } else {
                    advance();
                }

                if (stackAfterThen != stackAfterElse) {
                    std::cerr << "[PARSER WARNING] Then and else branches leave different stack depths\n";
                }

                stackDepth = stackAfterThen;
                ops.push_back(Op(thenOps, elseOps));
                break;
            }

            case TokenType::ELSE:
            case TokenType::ENDIF:
                if (!stopAtElseOrEndif) {
                    std::cerr << "[PARSER ERROR] Unexpected "
                              << tokenTypeToString(token.type) << "\n";
                    hadError = true;
                }
                return ops;

            case TokenType::UNKNOWN:
                std::cerr << "[PARSER ERROR] Unknown token '" << token.value << "'\n";
                hadError = true;
                advance();
                break;

            default:
                advance();
                break;
        }
    }

    return ops;
}

void Parser::printInstructions(const std::vector<Op>& ops) {
    std::cout << "\n========================================\n";
    std::cout << " STACK INSTRUCTION LIST\n";
    std::cout << "========================================\n";
    printInstructionList(ops);
}

void Parser::printInstructionList(const std::vector<Op>& ops, int indent) {
    for (const Op& op : ops) {
        printOp(op, indent);
    }
}

void Parser::printOp(const Op& op, int indent) {
    std::string space(indent, ' ');

    if (op.type == OpType::IF_ELSE) {
        std::cout << space << "IF_ELSE\n";
        std::cout << space << "  THEN:\n";
        printInstructionList(op.thenOps, indent + 4);
        std::cout << space << "  ELSE:\n";
        printInstructionList(op.elseOps, indent + 4);
        return;
    }

    std::cout << space << std::left << std::setw(18) << opTypeToString(op.type);

    if (!op.value.empty()) {
        std::cout << op.value;
    }

    std::cout << "\n";
}
