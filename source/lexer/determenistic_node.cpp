#include "determenistic_node.hpp"


size_t DetermenisticNode::id_counter = 0;

DetermenisticNode::DetermenisticNode(std::map<char, size_t> transitions, std::string label, bool is_final) :
                             transitions(transitions), label(label), is_final(is_final), id(id_counter++) {}

DetermenisticNode::DetermenisticNode(const AutomatonNode& node) : label(label), is_final(is_final), id(id_counter++)
{
    for (auto transition : node.transitions)
    {
        this->transitions.emplace(transition.first, transition.second);
    }
}

DetermenisticNode& DetermenisticNode::emplace_transition(const char trigger, const DetermenisticNode& target)
{
    this->transitions.emplace(trigger, target.get_id());
    return *this;
}

std::ostream& operator<<(std::ostream& os, const DetermenisticNode& node)
{
    os << "DNode: " << (node.label.size() == 0 ? "null" : node.label) << ". Id: " << node.get_id() << ". Is final: " << node.is_final << '\n';
    os << "Transitions: " << node.transitions.size() << '\n';

    for (auto pair : node.transitions)
    {
        char trigger = pair.first == '\0' ? '~' : pair.first;
        os << trigger << " to id: " << pair.second << '\n';
    }

    return os;
}
