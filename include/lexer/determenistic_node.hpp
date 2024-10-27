#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>

#include "automaton_node.hpp"


class DetermenisticNode
{
public:
    std::map<char, size_t> transitions;
    std::string label;
    bool is_final = false;

    DetermenisticNode() {id = id_counter++;};
    DetermenisticNode(std::map<char, size_t> transitions, std::string label, bool is_final);
    DetermenisticNode(const AutomatonNode& node);

    DetermenisticNode& emplace_transition(const char trigger, const DetermenisticNode& target);

    size_t get_id() const { return this->id; }

    static size_t id_counter;
private:
    size_t id = 0;
};

std::ostream& operator<<(std::ostream& os, const DetermenisticNode& nfa);