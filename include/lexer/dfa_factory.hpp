#pragma once

#include <unordered_set>
#include <functional>

#include "dfa.hpp"
#include "nfa.hpp"


class DFAFactory
{
public:
    static DFA from_nfa(const NFA& nfa, std::function<std::string(std::string, std::string)> label_presedence);
    // static DFA minimize_dfa(const DFA& dfa);  // TODO
private:
    static void check_and_apply_final_label(std::unordered_set<size_t> reached_ids, const NFA& nfa,
                                            DetermenisticNode& new_node, std::function<std::string(std::string, std::string)> label_presedence);
};