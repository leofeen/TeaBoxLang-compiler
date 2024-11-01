#pragma once

#include <string>
#include <iostream>
#include <unordered_map>


enum class TokenType
{
    _EMPTY = 0,
    SPACE,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    INT_LITERAL,
    RETURN,
    VARIABLE,
    _EOF,
};


struct Token
{
    TokenType token_type;
    std::string value;

    Token() = default;
    Token(TokenType token_type, std::string value) : token_type(token_type), value(value) {}

    static std::unordered_map<TokenType, std::string> verbose_token_types;
};

std::ostream& operator<<(std::ostream& os, const Token& token);