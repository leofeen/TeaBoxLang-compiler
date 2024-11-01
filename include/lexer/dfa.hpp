#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_set>

#include "automaton_node.hpp"


struct TestResult
{
    bool success = false;
    std::string token_label;
    std::string token_value;
    std::string suffix;

    TestResult() = default;
    TestResult(bool success, std::string token_label, std::string token_value, std::string suffix) : 
                success(success), token_label(token_label), token_value(token_value), suffix(suffix) {}
};

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

    TestResult test_string(std::string input_string);

    void recalculate_id_lookup();

private:
    std::vector<DetermenisticNode> nodes;
    std::map<size_t, size_t> id_lookup_table;
};

std::ostream& operator<<(std::ostream& os, const DFA& dfa);