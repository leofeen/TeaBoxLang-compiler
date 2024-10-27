#include <iostream>

#include "nfa_factory.hpp"
#include "nfa.hpp"
#include "dfa_factory.hpp"
#include "dfa.hpp"
#include "regexp_preprocessor.hpp"
#include "token.hpp"
#include "lexer.hpp"


int main()
{
    // std::cout << RegexpPreprocessor::clean_and_expand("[0-9]+") << std::endl;
    // NFA nfa = NFAFactory::from_regexp("a*(a|b)aa", false);
    // nfa.add_final_label("TEST");
    // std::cout << nfa << std::endl;

    // DFA dfa = DFAFactory::from_nfa(nfa, [](std::string a, std::string b){return a;});
    // std::cout << dfa << std::endl;

    // auto result = dfa.test_string("aabaab");
    // bool is_accepted;
    // std::string label;
    // std::string string_suffix;
    // std::tie(is_accepted, label, string_suffix) = result;
    // std::cout << "Is accepted? " << std::boolalpha << is_accepted << std::noboolalpha << '\n';
    // std::cout << "Label: " << label << '\n';
    // std::cout << "Suffix: " << string_suffix << std::endl;

    std::vector<TokenDefenition> tokens{
        TokenDefenition(TokenType::INT_LITERAL, R"(0\+)", "INT_LIT"),
    };

    Lexer lexer = Lexer(tokens);
    std::cout << lexer << std::endl;
}