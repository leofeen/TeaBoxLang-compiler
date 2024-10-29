#include <iostream>

#include "nfa_factory.hpp"
#include "nfa.hpp"
#include "dfa_factory.hpp"
#include "dfa.hpp"
#include "regex_preprocessor.hpp"
#include "token.hpp"
#include "lexer.hpp"


int main()
{
    std::vector<TokenDefenition> tokens_defenition {
        TokenDefenition(TokenType::SPACE, "\\s", "SPACE"),
        TokenDefenition(TokenType::RETURN, "return", "RETURN"),
        TokenDefenition(TokenType::INT_LITERAL, "[\\+-]?[0-9]+", "INT_LITERAL"),
        TokenDefenition(TokenType::ADD, "\\+", "ADD"),
        TokenDefenition(TokenType::SUBTRACT, "-", "SUBTRACT"),
        TokenDefenition(TokenType::MULTIPLY, "\\*", "MULTIPLY"),
        TokenDefenition(TokenType::DIVIDE, "/", "DIVIDE"),
        TokenDefenition(TokenType::VARIABLE, "[a-zA-Z_]+[a-zA-Z_0-9]*", "VARIABLE"),
    };

    Lexer lexer = Lexer(tokens_defenition);
    std::cout << lexer << std::endl;

    std::string input = "16 17 + 45 * -54 +1223 afgsg -+34 return";
    auto tokens = lexer.tokenize(input, std::cout, std::cerr);
    for (auto c : tokens)
    {
        std::cout << c << std::endl;
    }
}