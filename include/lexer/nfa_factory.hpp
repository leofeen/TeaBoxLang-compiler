#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "nfa.hpp"
#include "regexp_preprocessor.hpp"


class NFAFactory
{
public:
    static NFA from_literal(const char literal);
    static NFA from_regexp(const std::string& regexp, const bool not_first_pass);

private:
    static NFA& add_final_node(NFA& nfa);
};