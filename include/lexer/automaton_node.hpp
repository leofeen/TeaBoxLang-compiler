#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>


typedef std::vector<std::pair<char, size_t>> TransitionMap;

class AutomatonNode
{
public:
    TransitionMap transitions;
    std::string label;
    bool is_final = false;

    AutomatonNode() {id = id_counter++;};
    AutomatonNode(TransitionMap transitions, std::string label, bool is_final);

    AutomatonNode emplace_transition(const char trigger, const AutomatonNode& target);
    AutomatonNode add_transition(const char trigger, const AutomatonNode& target) const;

    constexpr size_t get_id() const { return this->id; }

    static size_t id_counter;
private:
    size_t id = 0;
};

std::ostream& operator<<(std::ostream& os, const AutomatonNode& nfa);