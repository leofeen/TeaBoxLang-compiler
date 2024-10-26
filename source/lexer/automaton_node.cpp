#include "automaton_node.hpp"


size_t AutomatonNode::id_counter = 0;

AutomatonNode::AutomatonNode(TransitionMap transitions, std::string label, bool is_final) :
                             transitions(transitions), label(label), is_final(is_final), id(id_counter++) {}

AutomatonNode& AutomatonNode::emplace_transition(const char trigger, const AutomatonNode& target)
{
    this->transitions.emplace_back(trigger, target.get_id());
    return *this;
}

// AutomatonNode AutomatonNode::add_transition(const char trigger, const AutomatonNode &target) const
// {
//     AutomatonNode result;
//     result.transitions = TransitionMap(this->transitions);
//     result.transitions.emplace_back(trigger, target.get_id());
//     return result;
// }

std::ostream& operator<<(std::ostream& os, const AutomatonNode& node)
{
    os << "Node: " << (node.label.size() == 0 ? "null" : node.label) << ". Id: " << node.get_id() << ". Is final: " << node.is_final << '\n';
    os << "Transitions: " << node.transitions.size() << '\n';

    for (auto pair : node.transitions)
    {
        char trigger = pair.first == '\0' ? '~' : pair.first;
        os << trigger << " to id: " << pair.second << '\n';
    }

    return os;
}
