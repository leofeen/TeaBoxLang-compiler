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


/* 
Tests input_string and returns tuple <is_success, token_label, token_value, input_suffix>
If token is found, returns is_success = true.
If no token found, returns is_succes = false, input_suffix = input_string.
*/
std::tuple<bool, std::string, std::string, std::string> DFA::test_string(std::string input_string) 
{
    if (this->id_lookup_table.empty()) { this->recalculate_id_lookup(); }

    size_t current_node_id = (*this)[0].get_id();
    bool is_passed_final = false;
    std::pair<size_t, size_t> passed_final;
    
    for (size_t i = 0; i < input_string.size(); i++)
    {
        char c = input_string[i];

        if ((*this)[this->id_lookup_table.at(current_node_id)].transitions.contains(c))
        {
            current_node_id = (*this)[this->id_lookup_table.at(current_node_id)].transitions.at(c);
            if ((*this)[this->id_lookup_table.at(current_node_id)].is_final)
            { 
                is_passed_final = true;
                passed_final.first = i;
                passed_final.second = current_node_id;
            }
        }
        else
        {
            if (is_passed_final)
            {
                std::string label = (*this)[this->id_lookup_table.at(passed_final.second)].label;
                return std::tuple<bool, std::string, std::string, std::string>(true, label, input_string.substr(0, passed_final.first+1), input_string.substr(passed_final.first+1));
            }

            return std::tuple<bool, std::string, std::string, std::string>(false, "", "", input_string);
        }
    }

    if ((*this)[this->id_lookup_table.at(current_node_id)].is_final)
    {
        return std::tuple<bool, std::string, std::string, std::string>(true, (*this)[this->id_lookup_table.at(current_node_id)].label, input_string, "");
    }
    else if (is_passed_final)
    {
        std::string label = (*this)[this->id_lookup_table.at(passed_final.second)].label;
        return std::tuple<bool, std::string, std::string, std::string>(true, label, input_string.substr(0, passed_final.first+1), input_string.substr(passed_final.first+1));
    }

    return std::tuple<bool, std::string, std::string, std::string>(false, "", "", input_string);
}

void DFA::recalculate_id_lookup()
{
    this->id_lookup_table.clear();
    for (size_t i = 0; i < this->size(); i++)
    {
        this->id_lookup_table.emplace((*this)[i].get_id(), i);
    }
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