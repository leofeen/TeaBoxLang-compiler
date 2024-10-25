#include "automaton_node.hpp"


AutomatonNode::AutomatonNode(std::vector<std::pair<char, AutomatonNode>> transitions, std::string label, bool is_final) :
                             transitions(transitions), label(label), is_final(is_final) {}

AutomatonNode AutomatonNode::emplace_transition(const char trigger, const AutomatonNode& target)
{
    this->transitions.emplace_back(trigger, target);
    return *this;
}

AutomatonNode AutomatonNode::add_transition(const char trigger, const AutomatonNode &target) const
{
    AutomatonNode result;
    result.transitions = TransitionMap(this->transitions);
    result.transitions.emplace_back(trigger, target);
    return result;
}

std::ostream& operator<<(std::ostream& os, const AutomatonNode& node)
{
    os << "Node: " << node.label << ". Is final: " << node.is_final << '\n';
    os << "Transitions: " << node.transitions.size() << '\n';

    for (auto pair : node.transitions)
    {
        char trigger = pair.first == '\0' ? '~' : pair.first;
        os << trigger << ": " << pair.second.label << '\n';
    }

    return os;
}
