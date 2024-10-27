#include "dfa.hpp"


void DFA::resize(const size_t size)
{
    if (size < nodes.size()) {
        std::cerr << "Can't discard nodes to size " << size << std::endl;
        exit(1);
    }

    this->nodes.resize(size);
}

DetermenisticNode& DFA::operator[](const size_t index)
{
    return this->nodes.at(index);
}

const DetermenisticNode& DFA::operator[](const size_t index) const
{
    return this->nodes.at(index);
}

const DetermenisticNode& DFA::get_by_id(const size_t id) const
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

void DFA::add_node(const DetermenisticNode node)
{
    this->nodes.push_back(node);
}

std::ostream& operator<<(std::ostream& os, const DFA& dfa)
{
    os << "DFA with " << dfa.size() << " nodes:" << '\n';
    for (size_t i = 0; i < dfa.size(); i++)
    {
        os << dfa[i] << '\n';
    }
    return os;
}