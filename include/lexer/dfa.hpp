#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>
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

    // std::pair<bool, std::string> test_string(std::string input_string);
    std::tuple<bool, std::string, std::string> test_string(std::string input_string);

    void recalculate_id_lookup();

private:
    std::vector<DetermenisticNode> nodes;
    std::map<size_t, size_t> id_lookup_table;
};

std::ostream& operator<<(std::ostream& os, const DFA& dfa);