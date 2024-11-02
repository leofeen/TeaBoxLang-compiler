#include "cfgrammar.hpp"


CFGrammar::CFGrammar(std::vector<TokenType> terminals, std::vector<Nonterminal> nonterminals) : nonterminals(nonterminals), terminals(terminals)
{
    this->terminals.insert(this->terminals.begin(), TokenType::_EMPTY);
    this->terminals.emplace_back(TokenType::_EOF);

    Nonterminal EOF_detector;
    EOF_detector.label = "EOF_DETECTOR";
    std::vector<std::vector<std::string>> EOF_transition_wrapper;
    std::vector<std::string> EOF_transition;
    EOF_transition.reserve(2);
    EOF_transition.emplace_back(nonterminals[0].label);
    EOF_transition.emplace_back(Token::verbose_token_types.at(TokenType::_EOF));
    EOF_transition_wrapper.emplace_back(EOF_transition);
    EOF_detector.transitions = EOF_transition_wrapper;
    this->nonterminals.insert(this->nonterminals.begin(), EOF_detector);

    size_t number_of_terminals = this->terminals.size();
    size_t number_of_nonterminals = this->nonterminals.size();

    for (size_t i = 0; i < number_of_terminals; i++)
    {
        this->id_lookup.emplace(Token::verbose_token_types.at(this->terminals[i]), i);
        this->alphabet += static_cast<char>(i);
    }

    for (size_t i = 0; i < number_of_nonterminals; i++)
    {
        this->id_lookup.emplace(this->nonterminals[i].label, i + number_of_terminals);
        this->alphabet += static_cast<char>(i + number_of_terminals);
    }

    // Generate compact_transitions - rewrite transitions in symbols from alphabet
    // for (size_t i = 0; i < number_of_nonterminals; i++)
    // {
    //     Nonterminal current_nonterminal = nonterminals[i];
    //     char current_symbol = this->alphabet[i + number_of_terminals];
    //     auto all_current_transitions_compact = std::vector<std::string>();
    //     all_current_transitions_compact.reserve(current_nonterminal.transitions.size());

    //     for (auto transition : current_nonterminal.transitions)
    //     {
    //         std::string current_transition_compact;
    //         for (auto label : transition)
    //         {
    //             current_transition_compact += this->alphabet[this->id_lookup.at(label)];
    //         }
    //         all_current_transitions_compact.emplace_back(current_transition_compact);
    //     }

    //     this->compact_transitions.emplace(current_symbol, all_current_transitions_compact);
    // }

    this->calculate_nullable();
    this->calculate_FIRST();
    this->calculate_FOLLOW();
}

std::string CFGrammar::get_alphabet()
{
    return this->alphabet;
}

std::vector<Nonterminal> CFGrammar::get_nonterminals()
{
    return this->nonterminals;
}

std::vector<TokenType> CFGrammar::get_terminals()
{
    return this->terminals;
}

std::string CFGrammar::get_nullable()
{
    return this->nullable;
}

std::unordered_map<char, std::unordered_set<size_t>> CFGrammar::get_FIRST()
{
    return this->FIRST;
}

std::unordered_map<char, std::unordered_set<size_t>> CFGrammar::get_FOLLOW()
{
    return this->FOLLOW;
}

std::string CFGrammar::get_label_from_id(size_t id)
{
    if (id < this->terminals.size())
    {
        return Token::verbose_token_types.at(this->terminals[id]);
    }

    return this->nonterminals[id - this->terminals.size()].label;
}

void CFGrammar::calculate_nullable()
{
    size_t number_of_terminals = this->terminals.size();
    size_t number_of_nonterminals = this->nonterminals.size();

    std::string nullable_next_iteration = std::string(number_of_terminals + number_of_nonterminals, '0');
    nullable_next_iteration[0] = '1';

    while (this->nullable != nullable_next_iteration)
    {
        this->nullable = nullable_next_iteration;

        nullable_next_iteration = std::string(number_of_terminals, '0');
        nullable_next_iteration[0] = '1';
        for (size_t i = 0; i < number_of_nonterminals; i++)
        {
            bool new_nullable_value = this->nullable[number_of_terminals + i] == '1';
            for (auto transition : nonterminals[i].transitions)
            {
                if (new_nullable_value) { break; }

                bool current_transition_nullable_value = this->nullable[this->id_lookup.at(transition[0])] == '1';
                for (size_t j = 1; j < transition.size(); j++)
                {
                    if (!current_transition_nullable_value) { break; }

                    current_transition_nullable_value = current_transition_nullable_value && (this->nullable[this->id_lookup.at(transition[j])] == '1');
                }

                new_nullable_value = new_nullable_value || current_transition_nullable_value;
            }

            nullable_next_iteration += new_nullable_value ? '1' : '0';
        }
    }
}

void CFGrammar::calculate_FIRST()
{
    size_t number_of_terminals = this->terminals.size();
    size_t number_of_nonterminals = this->nonterminals.size();

    std::unordered_map<char, std::unordered_set<size_t>> FIRST_next_iteration;

    for (auto c : this->alphabet)
    {
        FIRST_next_iteration.emplace(c, std::unordered_set<size_t>());
    }

    while (this->FIRST != FIRST_next_iteration)
    {
        this->FIRST = FIRST_next_iteration;

        FIRST_next_iteration.clear();

        FIRST_next_iteration.emplace(this->alphabet.at(this->id_lookup.at(Token::verbose_token_types.at(TokenType::_EMPTY))), std::unordered_set<size_t>());
        for (size_t i = 0; i < number_of_terminals; i++)
        {
            std::unordered_set<size_t> terminal_set;
            terminal_set.emplace(i);
            FIRST_next_iteration.emplace(this->alphabet.at(this->id_lookup.at(Token::verbose_token_types.at(terminals[i]))), terminal_set);
        }

        for (size_t i = 0; i < number_of_nonterminals; i++)
        {
            std::unordered_set<size_t> new_nonterminal_set; // = this->FIRST.at(this->alphabet[this->id_lookup.at(nonterminals[i].label)]);

            for (auto transition : nonterminals[i].transitions)
            {
                size_t in_transition_index = 0;
                
                while (true)
                {
                    auto current_FIRST_set = this->FIRST.at(this->alphabet.at(this->id_lookup.at(transition[in_transition_index]))); 
                    new_nonterminal_set.insert(current_FIRST_set.begin(), current_FIRST_set.end());

                    if (this->nullable[this->id_lookup.at(transition[in_transition_index])] == '0') { break; }
                    
                    if (++in_transition_index == transition.size()) { break; }
                }
            }

            FIRST_next_iteration.emplace(this->alphabet.at(this->id_lookup.at(nonterminals[i].label)), new_nonterminal_set);
        }
    }
}

void CFGrammar::calculate_FOLLOW()
{
    size_t number_of_terminals = this->terminals.size();
    size_t number_of_nonterminals = this->nonterminals.size();

    std::unordered_map<std::string, bool> FOLLOW_constraints;  // alphabet[first] + alphabet[second] -> true/false. If true, then FOLLOW(first) <= FOLLOW(second)
    std::unordered_map<std::string, bool> empty_constraints;

    std::unordered_map<char, std::unordered_set<size_t>> FOLLOW_next_iteration;

    // Generate empty constrains and initialize sets for first iteration
    for (size_t i = 0; i < number_of_nonterminals; i++)
    {
        for (size_t j = 0; j < number_of_nonterminals; j++)
        {
            auto from_to = std::string(1, this->alphabet[i + number_of_terminals]) + std::string(1, this->alphabet[j + number_of_terminals]);
            empty_constraints.emplace(from_to, false);
        }

        FOLLOW_next_iteration.emplace(this->alphabet[i + number_of_terminals], std::unordered_set<size_t>());
    }

    // std::cout << this->nonterminals.size() << std::endl;

    // for (auto p : empty_constraints)
    // {
    //     for (auto c : p.first)
    //     {
    //         std::cout << this->get_label_from_id(c) << std::endl;
    //     }
    // }

    // Generating all constraints and fullfill all FIRST in FOLLOW
    for (size_t i = 0; i < number_of_nonterminals; i++)
    {
        char current_nonterminal_symbol = this->alphabet[i + number_of_terminals];
        std::string current_nonterminal_label = this->nonterminals[i].label;

        for (size_t j = 0; j < number_of_nonterminals; j++)
        {
            for (auto transition : this->nonterminals[j].transitions)
            {
                bool adding_constraints = false;

                for (auto label : transition)
                {
                    if (this->is_terminal_label(label))
                    {
                        if (adding_constraints)
                        {
                            FOLLOW_next_iteration.at(current_nonterminal_symbol).emplace(this->id_lookup.at(label));
                        }
                        adding_constraints = false;
                        continue;
                    }

                    if (label == current_nonterminal_label)
                    {
                        adding_constraints = true;
                        continue;
                    }

                    if (!adding_constraints) { continue; }

                    char transition_nonterminal_symbol = this->alphabet[this->id_lookup.at(label)];
                    if (this->FIRST.contains(transition_nonterminal_symbol))
                    {
                        auto transition_nonterminal_FIRST_set = this->FIRST.at(transition_nonterminal_symbol);
                        FOLLOW_next_iteration.at(current_nonterminal_symbol).insert(transition_nonterminal_FIRST_set.begin(), transition_nonterminal_FIRST_set.end());
                    }
                    
                    if (this->nullable[this->id_lookup.at(label)] == '0')
                    {
                        adding_constraints = false;
                    }
                }

                if (adding_constraints)
                {
                    auto from_to = std::string(1, this->alphabet[j + number_of_terminals]) + std::string(1, current_nonterminal_symbol);
                    FOLLOW_constraints.emplace(from_to, true);
                }
            }
        }
    }

    FOLLOW_constraints.merge(empty_constraints);

    // Fixed point iteration for finding FOLLOW;
    while (this->FOLLOW != FOLLOW_next_iteration)
    {
        this->FOLLOW = FOLLOW_next_iteration;
        
        for (auto constraint : FOLLOW_constraints)
        {
            bool is_included = constraint.second;
            if (!is_included) { continue; }

            char first_nonterminal_symbol = constraint.first[0]; // FOLLOW(first) <= FOLLOW(second)
            char second_nonterminal_symbol = constraint.first[1];

            auto first_nonterminal_FOLLOW_set = this->FOLLOW.at(first_nonterminal_symbol);
            FOLLOW_next_iteration.at(second_nonterminal_symbol).insert(first_nonterminal_FOLLOW_set.begin(), first_nonterminal_FOLLOW_set.end());
        }
    }
    
}

inline Nonterminal CFGrammar::get_nonterminal_from_id(const size_t id) const
{
    return this->nonterminals[id - this->terminals.size()];
}

inline TokenType CFGrammar::get_terminal_from_id(const size_t id) const
{
    return this->terminals[id];
}

inline bool CFGrammar::is_terminal_label(const std::string& label) const
{
    return this->id_lookup.at(label) < this->terminals.size() ? true : false;
}
