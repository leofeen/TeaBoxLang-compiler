#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>

#include "nfa.hpp"
#include "regexp_preprocessor.hpp"


class NFAFactory
{
public:
    static NFA from_literal(const char literal);
    static NFA from_regexp(const std::string& regexp, const bool not_first_pass);
    static NFA from_range(const std::string& range);
    static std::unordered_set<char> construct_nfa_alphabet_set(const NFA& nfa);
private:
    static NFA& finalize(NFA& nfa);
};