#pragma once

#include <string>
#include <unordered_set>
#include <map>
#include <functional>

#include "nfa_factory.hpp"
#include "nfa.hpp"
#include "dfa_factory.hpp"
#include "dfa.hpp"
#include "regexp_preprocessor.hpp"
#include "token.hpp"


typedef std::tuple<TokenType, std::string, std::string> TokenDefenition;

class Lexer
{
public:
    Lexer() = default;
    Lexer(std::vector<TokenDefenition> tokens_defenitions);

    DFA tokens_dfa;
    std::unordered_set<TokenType> implemented_types;
};

std::ostream& operator<<(std::ostream& os, const Lexer& lexer);