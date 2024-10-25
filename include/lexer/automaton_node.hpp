#pragma once

#include <string>
#include <vector>
#include <iostream>


struct AutomatonNode
{
    std::vector<std::pair<char, AutomatonNode>> transitions;
    std::string label;
    bool is_final;

    AutomatonNode() = default;
    AutomatonNode(std::vector<std::pair<char, AutomatonNode>> transitions, std::string label, bool is_final);

    AutomatonNode emplace_transition(const char trigger, const AutomatonNode& target);
    AutomatonNode add_transition(const char trigger, const AutomatonNode& target) const;
};

typedef std::vector<std::pair<char, AutomatonNode>> TransitionMap;

std::ostream& operator<<(std::ostream& os, const AutomatonNode& nfa);