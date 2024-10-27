#include "lexer.hpp"

Lexer::Lexer(std::vector<TokenDefenition> tokens_defenitions)
{
    std::vector<NFA> token_nfas = std::vector<NFA>(tokens_defenitions.size());
    std::map<std::string, size_t> label_presedence_map;

    TokenType token_type;
    std::string regexp;
    std::string label;
    auto unpacking_tuple = std::tie(token_type, regexp, label);

    for (size_t i = 0; i < tokens_defenitions.size(); i++)
    {
        unpacking_tuple = tokens_defenitions[i];
        if (this->implemented_types.contains(token_type))
        {
            std::cerr << "Redefenition of token with label " << label << std::endl;
            std::exit(1);
        }

        this->implemented_types.insert(token_type);

        token_nfas[i] = NFAFactory::from_regexp(regexp, false);
        token_nfas[i].add_final_label(label);
        label_presedence_map.emplace(label, i);
    }

    NFA combined_nfa = NFAFactory::from_literal('\0');
    for (size_t i = 0; i < token_nfas.size(); i++)
    {
        combined_nfa[0].emplace_transition('\0', token_nfas[i][0]);
        combined_nfa.insert(token_nfas[i]);
    }

    combined_nfa.out_node_index = -1;
    combined_nfa.out_transition_trigger = ' ';

    auto combined_alphabet = NFAFactory::construct_nfa_alphabet_set(combined_nfa);
    combined_nfa.alphabet = std::string(combined_alphabet.begin(), combined_alphabet.end());

    std::function<std::string(std::string, std::string)> label_presedence = [&label_presedence_map](std::string a, std::string b){ return label_presedence_map.at(a) < label_presedence_map.at(b) ? a : b; };

    this->tokens_dfa = DFAFactory::from_nfa(combined_nfa, label_presedence);
}

std::ostream& operator<<(std::ostream& os, const Lexer& lexer)
{
    os << "Lexer\n";
    os << "DFA with " << lexer.tokens_dfa.size() << " nodes:" << '\n';
    for (size_t i = 0; i < lexer.tokens_dfa.size(); i++)
    {
        os << lexer.tokens_dfa[i] << '\n';
    }
    return os;
}