#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <span>
#include <unordered_set>

#include "automaton_node.hpp"


class DFA  // Determenisic Finite Atomata
{
public:
    std::string alphabet;

    DFA() = default;

    size_t size() const noexcept { return this->nodes.size(); };
    void resize(const size_t size);
    
    AutomatonNode& operator[](const size_t index);
    const AutomatonNode& operator[](const size_t index) const;
    const AutomatonNode& get_by_id(const size_t id) const;

    void add_node(const AutomatonNode node);

private:
    std::vector<AutomatonNode> nodes;
};

std::ostream& operator<<(std::ostream& os, const DFA& dfa);