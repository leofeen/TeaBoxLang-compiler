#pragma once

#include <unordered_set>

#include "dfa.hpp"
#include "nfa.hpp"


class DFAFactory
{
public:
    static DFA from_nfa(const NFA& nfa, std::string (*label_presedence)(std::string, std::string));

private:
    static void check_and_apply_final_label(std::unordered_set<size_t> reached_ids, const NFA& nfa,
                                                                DetermenisticNode& new_node, std::string (*label_presedence)(std::string, std::string));
};