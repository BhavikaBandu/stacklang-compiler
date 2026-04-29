#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& input) {
    source = input;
    pos = 0;
}

char Lexer::currentChar() {
    if (pos >= source.length()) {
        return '\0';
    }
    return source[pos];
}

char Lexer::peekChar() {
    if (pos + 1 >= source.length()) {
        return '\0';
    }
    return source[pos + 1];
}

void Lexer::advance() {
    if (pos < source.length()) {
        pos++;
    }
}

void Lexer::skipWhitespace() {
    while (std::isspace(currentChar())) {
        advance();
    }
}

Token Lexer::number() {
    std::string result;

    while (std::isdigit(currentChar())) {
        result += currentChar();
        advance();
    }

    return {TokenType::NUMBER, result};
}

Token Lexer::identifier() {
    std::string result;

    while (std::isalnum(currentChar()) || currentChar() == '_') {
        result += currentChar();
        advance();
    }

    if (result == "if") {
        return {TokenType::IF, result};
    }
    if (result == "else") {
        return {TokenType::ELSE, result};
    }
    if (result == "endif") {
        return {TokenType::ENDIF, result};
    }
    if (result == "print") {
        return {TokenType::PRINT, result};
    }

    return {TokenType::IDENTIFIER, result};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (currentChar() != '\0') {
        skipWhitespace();

        char ch = currentChar();

        if (ch == '\0') {
            break;
        }

        if (std::isdigit(ch)) {
            tokens.push_back(number());
            continue;
        }

        if (std::isalpha(ch) || ch == '_') {
            tokens.push_back(identifier());
            continue;
        }

        switch (ch) {
            case '+':
                tokens.push_back({TokenType::PLUS, "+"});
                advance();
                break;

            case '-':
                tokens.push_back({TokenType::MINUS, "-"});
                advance();
                break;

            case '*':
                tokens.push_back({TokenType::STAR, "*"});
                advance();
                break;

            case '/':
                tokens.push_back({TokenType::SLASH, "/"});
                advance();
                break;

            case '>':
                tokens.push_back({TokenType::GREATER, ">"});
                advance();
                break;

            case '<':
                tokens.push_back({TokenType::LESS, "<"});
                advance();
                break;

            case '=':
                if (peekChar() == '=') {
                    tokens.push_back({TokenType::EQUAL_EQUAL, "=="});
                    advance();
                    advance();
                } else {
                    tokens.push_back({TokenType::ASSIGN, "="});
                    advance();
                }
                break;

            default:
                tokens.push_back({TokenType::UNKNOWN, std::string(1, ch)});
                advance();
                break;
        }
    }

    tokens.push_back({TokenType::END_OF_FILE, ""});
    return tokens;
}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::IDENTIFIER: return "IDENTIFIER";

        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";

        case TokenType::GREATER: return "GREATER";
        case TokenType::LESS: return "LESS";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";

        case TokenType::ASSIGN: return "ASSIGN";

        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::ENDIF: return "ENDIF";
        case TokenType::PRINT: return "PRINT";

        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::UNKNOWN: return "UNKNOWN";
    }

    return "UNKNOWN";
}
