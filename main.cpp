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
    // auto res = RegexPreprocessor::clean_and_expand("\\s");
    // std::cout << res << std::endl;

    // for (auto c : res)
    // {
    //     std::cout << (AutomatonNode::verbose_trigger_name.contains(c) ? AutomatonNode::verbose_trigger_name.at(c) : std::string(1, c)) << std::endl;
    // }

    // auto split = RegexPreprocessor::split_upper_level_groups(res);
    // for (auto c : split)
    // {
    //     std::cout << c << std::endl;
    // }
    
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

    std::vector<TokenDefenition> tokens_defenition {
        TokenDefenition(TokenType::SPACE, "\\s", "SPACE"),
        TokenDefenition(TokenType::INT_LITERAL, "[\\+-]?[0-9]+", "INT_LITERAL"),
        TokenDefenition(TokenType::ADD, "\\+", "ADD"),
        TokenDefenition(TokenType::SUBTRACT, "-", "SUBTRACT"),
        TokenDefenition(TokenType::MULTIPLY, "\\*", "MULTIPLY"),
        TokenDefenition(TokenType::DIVIDE, "/", "DIVIDE"),
    };

    Lexer lexer = Lexer(tokens_defenition);
    std::cout << lexer << std::endl;

    std::string input = "16 17 + 45 * -54 +1223 -+34";
    auto tokens = lexer.tokenize(input, std::cout, std::cerr);
    for (auto c : tokens)
    {
        std::cout << c.value << std::endl;
    }
}