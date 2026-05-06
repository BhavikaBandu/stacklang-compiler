#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class TokenType {
    NUMBER,
    IDENTIFIER,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,
    EQUAL_EQUAL,
    NOT_EQUAL,

    ASSIGN,

    IF,
    ELSE,
    ENDIF,
    PRINT,

    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
private:
    std::string source;
    size_t pos;

    char currentChar();
    char peekChar();
    void advance();
    void skipWhitespace();

    Token number();
    Token identifier();

public:
    Lexer(const std::string& input);
    std::vector<Token> tokenize();
};

std::string tokenTypeToString(TokenType type);

#endif
