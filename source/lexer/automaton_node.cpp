#include "automaton_node.hpp"


size_t AutomatonNode::id_counter = 0;
std::map<char, std::string> AutomatonNode::verbose_trigger_name = {
    std::pair<char, std::string>('\0', "Empty"),
    std::pair<char, std::string>(' ', "Space"),
    std::pair<char, std::string>('\t', "Tab"),
    std::pair<char, std::string>('\n', "New line"),
    std::pair<char, std::string>('\r', "Carret reset"),
};

AutomatonNode::AutomatonNode(std::string label, bool is_final) : label(label), is_final(is_final), id(id_counter++) {}

NondetermenisticNode::NondetermenisticNode(TransitionMap transitions, std::string label, bool is_final) :
                             transitions(transitions), AutomatonNode(label, is_final) {}

NondetermenisticNode& NondetermenisticNode::emplace_transition(const char trigger, const AutomatonNode& target)
{
    this->transitions.emplace_back(trigger, target.get_id());
    return *this;
}

DetermenisticNode& DetermenisticNode::emplace_transition(const char trigger, const AutomatonNode& target)
{
    this->transitions.emplace(trigger, target.get_id());
    return *this;
}

std::ostream& operator<<(std::ostream& os, const NondetermenisticNode& node)
{
    os << "NNode: " << (node.label.size() == 0 ? "null" : node.label) << ". Id: " << node.get_id() << ". Is final: " << node.is_final << '\n';
    os << "Transitions: " << node.transitions.size() << '\n';

    for (auto pair : node.transitions)
    {
        std::string trigger = AutomatonNode::verbose_trigger_name.contains(pair.first) ? AutomatonNode::verbose_trigger_name.at(pair.first) : std::string(1, pair.first);
        os << trigger << " to id: " << pair.second << '\n';
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const DetermenisticNode& node)
{
    os << "DNode: " << (node.label.size() == 0 ? "null" : node.label) << ". Id: " << node.get_id() << ". Is final: " << node.is_final << '\n';
    os << "Transitions: " << node.transitions.size() << '\n';

    for (auto pair : node.transitions)
    {
        std::string trigger = AutomatonNode::verbose_trigger_name.contains(pair.first) ? AutomatonNode::verbose_trigger_name.at(pair.first) : std::string(1, pair.first);
        os << trigger << " to id: " << pair.second << '\n';
    }

    return os;
}


DetermenisticNode::DetermenisticNode(std::map<char, size_t> transitions, std::string label, bool is_final) :
                             transitions(transitions), AutomatonNode(label, is_final) {}

DetermenisticNode::DetermenisticNode(const NondetermenisticNode& node) : AutomatonNode(label, is_final)
{
    for (auto transition : node.transitions)
    {
        this->transitions.emplace(transition.first, transition.second);
    }
}