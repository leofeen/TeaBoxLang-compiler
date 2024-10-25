#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>

#include "automaton_node.hpp"


class NFA
{
public:
    char out_transition_trigger = ' ';
    int out_node_index = 0;

    NFA() = default;

    size_t size() const noexcept { return this->nodes.size(); };
    void resize(const size_t size);
    
    AutomatonNode& operator[](const size_t index);
    const AutomatonNode& operator[](const size_t index) const;

    NFA star() const;
    NFA plus() const;
    NFA question() const;

    void add_node(const AutomatonNode node);

private:
    std::vector<AutomatonNode> nodes;
};

NFA& operator+=(NFA& first, const NFA& second);
NFA operator+(const NFA& first, const NFA& second);

NFA operator|(const NFA& first, const NFA& second);

std::ostream& operator<<(std::ostream& os, const NFA& nfa);