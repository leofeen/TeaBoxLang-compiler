#include "nfa.hpp"


void NFA::resize(const size_t size)
{
    if (size < nodes.size()) {
        std::cerr << "Can't discard nodes to size " << size << std::endl;
        exit(1);
    }

    this->nodes.resize(size);
}

NondetermenisticNode& NFA::operator[](const size_t index)
{
    return this->nodes.at(index);
}

const NondetermenisticNode& NFA::operator[](const size_t index) const
{
    return this->nodes.at(index);
}

const NondetermenisticNode& NFA::get_by_id(const size_t id) const
{
    for (size_t i = 0; i < this->size(); i++)
    {
        if ((*this)[i].get_id() == id)
        {
            return (*this)[i];
        }
    }

    std::cerr << "No element with id " << id << " in" << std::endl;
    std::cerr << *this << std::endl;
    std::exit(1);
}

NFA NFA::star()
{
    NFA result;
    size_t this_size = this->size();

    result.resize(this_size + 1);

    NondetermenisticNode start_node;

    result[0] = start_node;
    result.out_node_index = 0;
    result.out_transition_trigger = '\0';

    for (size_t i = 0; i < this_size; i++)
    {
        result[i + 1] = i != this->out_node_index ? (*this)[i] : (*this)[i].emplace_transition(this->out_transition_trigger, start_node);
    }

    result[0].emplace_transition('\0', result[1]);

    return result;
}

NFA NFA::question()
{
    NFA result;
    size_t this_size = this->size();

    result.resize(this_size + 2);

    NondetermenisticNode start_node;
    NondetermenisticNode finish_node;

    result.out_transition_trigger = '\0';

    result[0] = start_node;

    result[this_size + 1] = finish_node;
    result.out_node_index = this_size + 1;

    for (size_t i = 0; i < this_size ; i++)
    {
        result[i + 1] = i != this->out_node_index ? (*this)[i] : (*this)[i].emplace_transition(this->out_transition_trigger, finish_node);
    }

    result[0].emplace_transition('\0', result[1]).emplace_transition('\0', finish_node);

    return result;
}

void NFA::add_node(const NondetermenisticNode& node)
{
    this->nodes.push_back(node);
}

std::unordered_set<size_t> NFA::find_closure(std::unordered_set<size_t> start_set) const
{
    std::unordered_set<size_t> result;  
    std::unordered_set<size_t> unmarked = std::unordered_set<size_t>(start_set.begin(), start_set.end());

    while (unmarked.size() != 0)
    {
        auto next_unmarked_handle = unmarked.extract(unmarked.begin());
        const NondetermenisticNode& next_unmarked = this->get_by_id(next_unmarked_handle.value());

        result.insert(std::move(next_unmarked_handle));

        for (auto transition_pair : next_unmarked.transitions)
        {
            if (transition_pair.first == '\0' && !result.contains(transition_pair.second))
            {
                unmarked.insert(transition_pair.second);
            }
        }
    }

    return result;
}

void NFA::add_final_label(std::string label)
{
    AutomatonNode& final_node = (*this)[this->size() - 1];
    if (!final_node.is_final)
    {
        for (size_t i = 0; i < this->size(); i++)
        {
            if ((*this)[i].is_final)
            {
                final_node = (*this)[i];
                break;
            }
        }
    }

    final_node.label = label;
}

void NFA::insert(NFA &nfa)
{
    for (size_t i = 0; i < nfa.size(); i++)
    {
        this->add_node(nfa[i]);
    }
}

std::unordered_set<size_t> NFA::find_closure(std::span<size_t> start_set) const
{
    return this->find_closure(std::unordered_set<size_t>(start_set.begin(), start_set.end()));
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

    for (size_t i = 0; i < second_size; i++)
    {
        first[first_size + i] = second[i];
    }

    first[first.out_node_index].emplace_transition(first.out_transition_trigger, first[first_size]);

    first.out_transition_trigger = second.out_transition_trigger;
    first.out_node_index = first_size + second.out_node_index;

    return first;
}

NFA operator+(NFA& first, NFA& second)
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
        result[i] = i != first.out_node_index ? first[i] : first[i].emplace_transition(first.out_transition_trigger, second[0]);
    }

    for (size_t i = 0; i < second_size; i++)
    {
        result[first_size + i] = second[i];
    }

    result.out_transition_trigger = second.out_transition_trigger;
    result.out_node_index = first_size + second.out_node_index;

    return result;
}

NFA operator|(NFA &first, NFA &second)
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

    NondetermenisticNode start_node;
    NondetermenisticNode finish_node;

    result.out_transition_trigger = '\0';

    result[0] = start_node;

    result[first_size + second_size + 1] = finish_node;
    result.out_node_index = first_size + second_size + 1;

    for (size_t i = 0; i < first_size ; i++)
    {
        result[i + 1] = i != first.out_node_index ? first[i] : first[i].emplace_transition(first.out_transition_trigger, finish_node);
    }

    for (size_t i = 0; i < second_size; i++)
    {
        result[i + 1 + first_size] = i != second.out_node_index ? second[i] : second[i].emplace_transition(second.out_transition_trigger, finish_node);
    }

    result[0].emplace_transition('\0', result[1]).emplace_transition('\0', result[1 + first_size]);

    return result;
}

std::ostream& operator<<(std::ostream& os, const NFA& nfa)
{
    os << "NFA with " << nfa.size() << " nodes:" << '\n';
    os << "Out: " << nfa.out_node_index << " at transition " << nfa.out_transition_trigger << '\n';
    for (size_t i = 0; i < nfa.size(); i++)
    {
        os << nfa[i] << '\n';
    }
    return os;
}