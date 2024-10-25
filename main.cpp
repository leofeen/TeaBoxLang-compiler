#include <iostream>

#include "nfa_factory.hpp"
#include "nfa.hpp"
#include "regexp_preprocessor.hpp"


int main()
{
    NFA nfa = NFAFactory::from_regexp("[a-c]", false);
    std::cout << nfa << std::endl;
}