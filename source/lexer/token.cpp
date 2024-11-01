#include "token.hpp"


std::unordered_map<TokenType, std::string> Token::verbose_token_types = {
    std::pair<TokenType, std::string>(TokenType::_EMPTY, "EMPTY"),
    std::pair<TokenType, std::string>(TokenType::ADD, "ADD"),
    std::pair<TokenType, std::string>(TokenType::SPACE, "SPACE"),
    std::pair<TokenType, std::string>(TokenType::SUBTRACT, "SUBTRACT"),
    std::pair<TokenType, std::string>(TokenType::MULTIPLY, "MULTIPLY"),
    std::pair<TokenType, std::string>(TokenType::DIVIDE, "DIVIDE"),
    std::pair<TokenType, std::string>(TokenType::INT_LITERAL, "INT_LITERAL"),
    std::pair<TokenType, std::string>(TokenType::RETURN, "RETURN"),
    std::pair<TokenType, std::string>(TokenType::VARIABLE, "VARIABLE"),
    std::pair<TokenType, std::string>(TokenType::_EOF, "_EOF"),
};

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    std::string verbose_type = Token::verbose_token_types.contains(token.token_type) ? Token::verbose_token_types.at(token.token_type) : "DEFAULT_TOKEN_LABEL";

    os << verbose_type << " " << token.value;

    return os;
}