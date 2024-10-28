#pragma once

#include <string>


enum class TokenType
{
    _NULL = 0,
    SPACE,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    INT_LITERAL,
};


struct Token
{
    TokenType token_type;
    std::string value;

    Token() = default;
    Token(TokenType token_type, std::string value) : token_type(token_type), value(value) {}
};