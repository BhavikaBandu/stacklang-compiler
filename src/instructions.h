#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <string>
#include <vector>

enum class OpType {
    PUSH_NUMBER,
    LOAD_VARIABLE,
    STORE_VARIABLE,

    ADD,
    SUB,
    MUL,
    DIV,

    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,
    EQUAL,
    NOT_EQUAL,

    PRINT,
    IF_ELSE
};

struct Op {
    OpType type;
    std::string value;
    std::vector<Op> thenOps;
    std::vector<Op> elseOps;

    Op(OpType type) : type(type) {}

    Op(OpType type, const std::string& value)
        : type(type), value(value) {}

    Op(const std::vector<Op>& thenOps, const std::vector<Op>& elseOps)
        : type(OpType::IF_ELSE), thenOps(thenOps), elseOps(elseOps) {}
};

std::string opTypeToString(OpType type);

#endif
