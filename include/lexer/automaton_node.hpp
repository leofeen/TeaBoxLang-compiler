#pragma once

#include <string>
#include <map>


struct AutomatonNode
{
    std::map<char, AutomatonNode> transitions;
    std::string label;
    bool is_final;

    AutomatonNode() = default;
    AutomatonNode(std::map<char, AutomatonNode> transitions, std::string label, bool is_final);

    AutomatonNode& emplace_transition(const char trigger, const AutomatonNode& target);
    AutomatonNode& add_transition(const char trigger, const AutomatonNode& target) const;
};

typedef std::map<char, AutomatonNode> TransitionMap;