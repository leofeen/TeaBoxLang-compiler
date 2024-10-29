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
        this->label_to_token_type_map.emplace(label, token_type);

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

std::vector<Token> Lexer::tokenize(std::string input, std::ostream& output_stream, std::ostream& error_stream)
{
    output_stream << "Started tokenizing.\nIn case of Lexical Error symbols up to the next space or linefeed will be skipped." << std::endl;

    std::vector<Token> result;

    bool is_success = false;
    std::string next_label;
    std::string token_value;
    std::string input_suffix;

    auto tied_returns = std::tie(is_success, next_label, token_value, input_suffix);

    while (input.size() > 0)
    {
        tied_returns = this->tokens_dfa.test_string(input);

        if (is_success)
        {
            result.emplace_back(this->label_to_token_type_map.at(next_label), token_value);
            input = input_suffix;
        }
        else
        {
            size_t space_index = input_suffix.find(' ');
            size_t tab_index = input_suffix.find('\t');
            size_t new_line_index = input_suffix.find('\n');

            size_t cutoff_index = input_suffix.size() - 1;

            if (space_index != input_suffix.npos && space_index < tab_index && space_index < new_line_index)
            {
                cutoff_index = space_index;
            }
            else if (tab_index != input_suffix.npos && tab_index < space_index && tab_index < new_line_index)
            {
                cutoff_index = tab_index;
            }
            else if (new_line_index != input_suffix.npos && new_line_index < space_index && new_line_index < tab_index)
            {
                cutoff_index = new_line_index;
            }

            error_stream << "Lexical Error: " << input_suffix.substr(0, cutoff_index + 1) << " is an invalid token" << std::endl;

            input = input_suffix.substr(cutoff_index + 1);
        }
    }

    output_stream << "Tokenization complete. Found " << result.size() << " tokens." << std::endl;

    return result;
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