#include "dfa_factory.hpp"

DFA DFAFactory::from_nfa(const NFA &nfa, std::string (*label_presedence)(std::string, std::string))
{
    DFA result;
    result.alphabet = nfa.alphabet;

    std::vector<DetermenisticNode> result_nodes;

    typedef std::pair<std::unordered_set<size_t>, size_t> NodeAsSubset;
    auto result_subset_nodes = std::vector<NodeAsSubset>();
    auto starting_node_id = std::vector<size_t>();
    starting_node_id.push_back(nfa[0].get_id());

    auto starting_set = nfa.find_closure(starting_node_id);

    result_nodes.emplace_back();
    result_subset_nodes.push_back(NodeAsSubset(starting_set, result_nodes[0].get_id()));
    size_t current_unchecked_node_index = 0;

    DFAFactory::check_and_apply_final_label(starting_set, nfa, result_nodes[0], label_presedence);

    std::unordered_set<size_t> reached_ids = std::unordered_set<size_t>();
    while (current_unchecked_node_index != result_subset_nodes.size())
    {
        size_t current_dfa_node_id = result_subset_nodes[current_unchecked_node_index].second;

        for (auto c : nfa.alphabet)
        {
            reached_ids.clear();

            for (auto id : result_subset_nodes[current_unchecked_node_index].first)
            {
                const AutomatonNode& current_node_from_subset = nfa.get_by_id(id);

                for (auto transition : current_node_from_subset.transitions)
                {
                    if (transition.first == c)
                    {
                        reached_ids.insert(transition.second);
                    }
                }
            }

            reached_ids = nfa.find_closure(reached_ids);
            size_t transition_target_id = 0;

            bool new_subset = reached_ids.size() > 0 ? true : false;
            for (auto already_found : result_subset_nodes)
            {
                if (already_found.first == reached_ids)
                {
                    new_subset = false;
                    transition_target_id = already_found.second;
                    break;
                }
            }

            if (new_subset)
            {
                result_nodes.emplace_back();
                DetermenisticNode& new_node = result_nodes[result_nodes.size() - 1];
                result_subset_nodes.push_back(NodeAsSubset(reached_ids, new_node.get_id()));
                transition_target_id = new_node.get_id();

                DFAFactory::check_and_apply_final_label(reached_ids, nfa, new_node, label_presedence);
            }

            if (result_nodes[current_unchecked_node_index].get_id() == current_dfa_node_id && reached_ids.size() > 0)
            {
                size_t transition_target_index = result_nodes.size() - 1;
                for (size_t i = result_nodes.size() - 1; i >= 0; i--)
                {
                    if (result_nodes[i].get_id() == transition_target_id)
                    {
                        transition_target_index = i;
                        break;
                    }
                }

                result_nodes[current_unchecked_node_index].emplace_transition(c, result_nodes[transition_target_index]);
            }
        }

        current_unchecked_node_index++;
    }

    for (size_t i = 0; i < result_nodes.size(); i++)
    {
        result.add_node(result_nodes[i]);
    }

    return result;
}

void DFAFactory::check_and_apply_final_label(std::unordered_set<size_t> reached_ids, const NFA& nfa,
                                                                DetermenisticNode& new_node, std::string (*label_presedence)(std::string, std::string))
{
    std::vector<std::string> final_nodes_labels;
    for (auto id : reached_ids)
    {
        const AutomatonNode& candidate = nfa.get_by_id(id);
        if (candidate.is_final)
        {
            final_nodes_labels.push_back(candidate.label);
        }
    }

    if (final_nodes_labels.size() > 0)
    {
        new_node.is_final = true;
        std::string new_label = final_nodes_labels[0];
        for (size_t i = 1; i < final_nodes_labels.size(); i++)
        {
            new_label = label_presedence(new_label, final_nodes_labels[i]);
        }

        new_node.label = new_label;
    }
}
