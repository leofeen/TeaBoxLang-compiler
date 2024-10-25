#include <iostream>

#include "nfa_factory.hpp"
#include "nfa.hpp"


int main()
{
    NFA nfa = NFAFactory::from_regexp("a*", false);
    std::cout << nfa << std::endl;
}