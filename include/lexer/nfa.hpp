#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <span>
#include <unordered_set>

#include "automaton_node.hpp"


class NFA  // Nondetermenisic Finite Atomata
{
public:
    char out_transition_trigger = ' ';
    int out_node_index = 0;
    std::string alphabet;

    NFA() = default;

    size_t size() const noexcept { return this->nodes.size(); };
    void resize(const size_t size);
    
    AutomatonNode& operator[](const size_t index);
    const AutomatonNode& operator[](const size_t index) const;
    const AutomatonNode& get_by_id(const size_t id) const;

    NFA star();
    // NFA plus();
    NFA question();

    void add_node(const AutomatonNode& node);

    std::unordered_set<size_t> find_closure(std::span<size_t> start_set) const;
    std::unordered_set<size_t> find_closure(std::unordered_set<size_t> start_set) const;

    void add_final_label(std::string label);

    void insert(NFA& nfa);

private:
    std::vector<AutomatonNode> nodes;
};

NFA& operator+=(NFA& first, const NFA& second);
NFA operator+(NFA& first, NFA& second);

NFA operator|(NFA& first, NFA& second);

std::ostream& operator<<(std::ostream& os, const NFA& nfa);