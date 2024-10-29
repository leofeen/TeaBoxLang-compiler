#include "token.hpp"


std::ostream& operator<<(std::ostream& os, const Token& token)
{
    switch (token.token_type)
    {
    case TokenType::_NULL : os << "NULL "; break;
    case TokenType::ADD : os << "ADD "; break;
    case TokenType::SPACE : os << "SPACE "; break;
    case TokenType::SUBTRACT : os << "SUBTRACT "; break;
    case TokenType::MULTIPLY : os << "MULTIPLY "; break;
    case TokenType::DIVIDE : os << "DIVIDE "; break;
    case TokenType::INT_LITERAL : os << "INT_LITERAL "; break;
    case TokenType::RETURN : os << "RETURN "; break;
    case TokenType::VARIABLE : os << "VARIABLE "; break;
    default: os << "DEFAULT_TOKEN_LABEL "; break;
    }

    os << token.value;

    return os;
}