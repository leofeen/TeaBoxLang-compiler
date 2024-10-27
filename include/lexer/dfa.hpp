#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <span>
#include <unordered_set>

#include "determenistic_node.hpp"


class DFA  // Determenisic Finite Atomata
{
public:
    std::string alphabet;

    DFA() = default;

    size_t size() const noexcept { return this->nodes.size(); };
    void resize(const size_t size);
    
    DetermenisticNode& operator[](const size_t index);
    const DetermenisticNode& operator[](const size_t index) const;
    const DetermenisticNode& get_by_id(const size_t id) const;

    void add_node(const DetermenisticNode node);

private:
    std::vector<DetermenisticNode> nodes;
};

std::ostream& operator<<(std::ostream& os, const DFA& dfa);