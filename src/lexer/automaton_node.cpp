#include <automaton_node.hpp>


AutomatonNode::AutomatonNode(std::map<char, AutomatonNode> transitions, std::string label, bool is_final) :
                             transitions(transitions), label(label), is_final(is_final) {}

AutomatonNode& AutomatonNode::emplace_transition(const char trigger, const AutomatonNode& target)
{
    this->transitions.emplace(trigger, target);
    return *this;
}

AutomatonNode& AutomatonNode::add_transition(const char trigger, const AutomatonNode &target) const
{
    AutomatonNode result = *this;
    result.transitions.emplace(trigger, target);
    return result;
}
