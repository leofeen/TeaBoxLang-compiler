#include <iostream>

#include "nfa_factory.hpp"
#include "nfa.hpp"
#include "dfa_factory.hpp"
#include "dfa.hpp"
#include "regexp_preprocessor.hpp"


int main()
{
    NFA nfa = NFAFactory::from_regexp("a*(a|b)aa", false);
    nfa.add_final_label("TEST");
    std::cout << nfa << std::endl;

    DFA dfa = DFAFactory::from_nfa(nfa, [](std::string a, std::string b){return a;});
    std::cout << dfa << std::endl;
}