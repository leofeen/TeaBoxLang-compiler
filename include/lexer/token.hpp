#pragma once

#include <string>


enum class TokenType
{
    _NULL = 0,
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
};