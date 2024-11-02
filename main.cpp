#include <iostream>

#include "nfa_factory.hpp"
#include "nfa.hpp"
#include "dfa_factory.hpp"
#include "dfa.hpp"
#include "regex_preprocessor.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "cfgrammar.hpp"
#include "nonterminal.hpp"


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
    std::cout << lexer << '\n';

    std::string input = "16 17 + 45 * -54 +1223 afgsg -+34 return";
    auto tokens = lexer.tokenize(input, std::cout, std::cerr);
    for (auto c : tokens)
    {
        std::cout << c << '\n';
    }

    std::cout << "\n\n";

    std::vector<Nonterminal> nonterminals;
    std::vector<TokenType> cfgr_terminals;

    cfgr_terminals.emplace_back(TokenType::ADD);
    cfgr_terminals.emplace_back(TokenType::MULTIPLY);
    cfgr_terminals.emplace_back(TokenType::SUBTRACT);

    std::vector<Transition> T_transitions;
    T_transitions.emplace_back(Transition{"R"});
    T_transitions.emplace_back(Transition{Token::verbose_token_types.at(TokenType::ADD), "T", Token::verbose_token_types.at(TokenType::SUBTRACT)});
    Nonterminal T = Nonterminal("T", T_transitions);

    std::vector<Transition> R_transitions;
    R_transitions.emplace_back(Transition{Token::verbose_token_types.at(TokenType::_EMPTY)});
    R_transitions.emplace_back(Transition{"R", Token::verbose_token_types.at(TokenType::MULTIPLY), "R"});
    Nonterminal R = Nonterminal("R", R_transitions);

    nonterminals.emplace_back(T);
    nonterminals.emplace_back(R);

    CFGrammar grammar = CFGrammar(cfgr_terminals, nonterminals);

    auto alphabet = grammar.get_alphabet();
    for (auto c : alphabet)
    {
        std::cout << grammar.get_label_from_id(c) << '\n';
    }

    std::cout << "\nNullable " << grammar.get_nullable() << '\n';

    std::cout << "\nFIRST sets" << '\n';
    auto FIRST = grammar.get_FIRST();
    for (auto pair : FIRST)
    {
        size_t id = pair.first;

        std::string label = grammar.get_label_from_id(id);
        std::cout << label << '\n';
        std::cout << std::string(label.size(), '-') << '|' << '\n';
        for (auto i : pair.second)
        {
            std::cout << std::string(label.size(), ' ') << "|- " << grammar.get_label_from_id(i) << '\n';
        }
        std::cout << '\n';
    }

    std::cout << "\nFOLLOW sets" << '\n';
    auto FOLLOW = grammar.get_FOLLOW();
    for (auto pair : FOLLOW)
    {
        size_t id = pair.first;

        std::string label = grammar.get_label_from_id(id);
        std::cout << label << '\n';
        std::cout << std::string(label.size(), '-') << '|' << '\n';
        for (auto i : pair.second)
        {
            std::cout << std::string(label.size(), ' ') << "|- " << grammar.get_label_from_id(i) << '\n';
        }
        std::cout << std::endl;
    }
}