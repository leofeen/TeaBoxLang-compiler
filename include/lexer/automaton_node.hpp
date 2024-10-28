#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>


typedef std::vector<std::pair<char, size_t>> TransitionMap;

class AutomatonNode
{
public:
    std::string label;
    bool is_final = false;

    AutomatonNode() {id = id_counter++;}
    AutomatonNode(std::string label, bool is_final);
    virtual ~AutomatonNode() {}

    size_t get_id() const { return this->id; }

    static size_t id_counter;
    static std::map<char, std::string> verbose_trigger_name;
protected:
    size_t id = 0;
};


class NondetermenisticNode : public AutomatonNode
{
public:
    TransitionMap transitions;

    NondetermenisticNode() {id = id_counter++;}
    NondetermenisticNode(TransitionMap transitions, std::string label, bool is_final);
    virtual ~NondetermenisticNode() {}

    virtual NondetermenisticNode& emplace_transition(const char trigger, const AutomatonNode& target);
};


class DetermenisticNode : public AutomatonNode
{
public:
    std::map<char, size_t> transitions;

    DetermenisticNode() {id = id_counter++;}
    DetermenisticNode(std::map<char, size_t> transitions, std::string label, bool is_final);
    DetermenisticNode(const NondetermenisticNode& node);
    virtual ~DetermenisticNode() {}

    virtual DetermenisticNode& emplace_transition(const char trigger, const AutomatonNode& target);
};

std::ostream& operator<<(std::ostream& os, const NondetermenisticNode& node);
std::ostream& operator<<(std::ostream& os, const DetermenisticNode& node);