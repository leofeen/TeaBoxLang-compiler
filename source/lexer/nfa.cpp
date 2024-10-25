#include "nfa.hpp"


void NFA::resize(const size_t size)
{
    if (size < nodes.size()) {
        std::cerr << "Can't discard nodes to size " << size << std::endl;
        exit(1);
    }

    this->nodes.resize(size);
}

AutomatonNode& NFA::operator[](const size_t index)
{
    return this->nodes.at(index);
}

const AutomatonNode& NFA::operator[](const size_t index) const
{
    return this->nodes.at(index);
}

NFA NFA::star() const
{
    NFA result;
    size_t this_size = this->size();

    result.resize(this_size + 1);

    AutomatonNode start_node = AutomatonNode(TransitionMap(), "", false);

    result[0] = start_node.emplace_transition('\0', (*this)[0]);
    result.out_node_index = 0;
    result.out_transition_trigger = '\0';

    for (size_t i = 0; i < this_size; i++)
    {
        result[i + 1] = i != this->out_node_index ? (*this)[i] : (*this)[i].add_transition(this->out_transition_trigger, start_node);
    }

    return result;
}

void NFA::add_node(const AutomatonNode node)
{
    this->nodes.push_back(node);
}

NFA& operator+=(NFA& first, const NFA& second)
{
    size_t first_size = first.size();
    size_t second_size = second.size();

    if (first_size == 0)
    {
        first = second;
        return first;
    }

    if (second_size == 0)
    {
        return first;
    }

    first.resize(first_size + second_size);

    first[first.out_node_index].emplace_transition(first.out_transition_trigger, second[0]);

    for (size_t i = 0; i < second_size; i++)
    {
        first[first_size + i] = second[i];
    }

    first.out_transition_trigger = second.out_transition_trigger;
    first.out_node_index = first_size + second.out_node_index;

    return first;
}

NFA operator+(const NFA& first, const NFA& second)
{
    NFA result;
    size_t first_size = first.size();
    size_t second_size = second.size();

    if (first_size == 0)
    {
        return second;
    }

    if (second_size == 0)
    {
        return first;
    }

    result.resize(first_size + second_size);

    for (size_t i = 0; i < first_size; i++)
    {
        result[i] = i != first.out_node_index ? first[i] : first[i].add_transition(first.out_transition_trigger, second[0]);
    }

    for (size_t i = 0; i < second_size; i++)
    {
        result[first_size + i] = second[i];
    }

    result.out_transition_trigger = second.out_transition_trigger;
    result.out_node_index = first_size + second.out_node_index;

    return result;
}

NFA operator|(const NFA &first, const NFA &second)
{
    NFA result;
    size_t first_size = first.size();
    size_t second_size = second.size();

    if (first_size == 0)
    {
        return second;
    }

    if (second_size == 0)
    {
        return first;
    }

    result.resize(first_size + second_size + 2);

    AutomatonNode start_node = AutomatonNode(TransitionMap(), "In", false);
    AutomatonNode finish_node = AutomatonNode(TransitionMap(), "Out", false);

    result.out_transition_trigger = '\0';

    result[0] = start_node.emplace_transition('\0', first[0]).emplace_transition('\0', second[0]);

    result[first_size + second_size + 1] = finish_node;
    result.out_node_index = first_size + second_size + 1;

    for (size_t i = 0; i < first_size ; i++)
    {
        result[i + 1] = i != first.out_node_index ? first[i] : first[i].add_transition(first.out_transition_trigger, finish_node);
    }

    for (size_t i = 0; i < second_size; i++)
    {
        result[i + 1 + first_size] = i != second.out_node_index ? second[i] : second[i].add_transition(second.out_transition_trigger, finish_node);
    }


    return result;
}

std::ostream& operator<<(std::ostream& os, const NFA& nfa)
{
    os << "Out: " << nfa.out_node_index << " at transition " << nfa.out_transition_trigger << '\n';
    for (int i = 0; i < nfa.size(); i++)
    {
        os << nfa[i] << '\n';
    }
    return os;
}