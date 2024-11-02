#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <unordered_set>

#include "token.hpp"
#include "nonterminal.hpp"


class CFGrammar
{
public:
    CFGrammar() = default;
    CFGrammar(std::vector<TokenType> terminals, std::vector<Nonterminal> nonterminals);

    std::string get_alphabet();
    std::vector<Nonterminal> get_nonterminals();
    std::vector<TokenType> get_terminals();

    std::string get_nullable();
    std::unordered_map<char, std::unordered_set<size_t>> get_FIRST();
    std::unordered_map<char, std::unordered_set<size_t>> get_FOLLOW();

    std::string get_label_from_id(size_t id);

private:
    std::string alphabet;  // All terminals
    std::vector<Nonterminal> nonterminals;
    std::vector<TokenType> terminals;
    std::unordered_map<std::string, size_t> id_lookup;

    std::unordered_map<char, std::unordered_set<size_t>> FIRST;
    std::unordered_map<char, std::unordered_set<size_t>> FOLLOW;  // Only for nonterminals, as terminals can't be nullable
    std::string nullable;

    // std::unordered_map<char, std::vector<std::string>> compact_transitions;

    void calculate_nullable();
    void calculate_FIRST();
    void calculate_FOLLOW();

    inline Nonterminal get_nonterminal_from_id(const size_t id) const;
    inline TokenType get_terminal_from_id(const size_t id) const; 

    inline bool is_terminal_label(const std::string& label) const;
};