#pragma once

#include <string>
#include <unordered_set>
#include <map>
#include <functional>
#include <iostream>

#include "nfa_factory.hpp"
#include "nfa.hpp"
#include "dfa_factory.hpp"
#include "dfa.hpp"
#include "regex_preprocessor.hpp"
#include "token.hpp"


typedef std::tuple<TokenType, std::string, std::string> TokenDefenition;

class Lexer
{
public:
    DFA tokens_dfa;
    std::unordered_set<TokenType> implemented_types;
    std::map<std::string, TokenType> label_to_token_type_map;

    Lexer() = default;
    Lexer(std::vector<TokenDefenition> tokens_defenitions);

    std::vector<Token> tokenize(std::string input, std::ostream& output_stream, std::ostream& error_stream);
};

std::ostream& operator<<(std::ostream& os, const Lexer& lexer);