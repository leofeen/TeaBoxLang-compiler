#include "nfa_factory.hpp"


NFA NFAFactory::from_literal(const char literal)
{
    NFA result;
    result.add_node(NondetermenisticNode());
    result.out_transition_trigger = literal;

    return result;
}

NFA NFAFactory::from_regexp(const std::string& regexp, const bool not_first_pass = false)
{
    std::string regex_clean = not_first_pass ? regexp : RegexPreprocessor::clean_and_expand(regexp);

    size_t open_group_pos = regex_clean.find('(');
    size_t open_range_pos = regex_clean.find('[');
    if (open_group_pos != regex_clean.npos || open_range_pos != regex_clean.npos)
    {
        auto split = RegexPreprocessor::split_upper_level_groups(regex_clean);

        NFA result;
        NFA current;
        char operation = '.';

        for (auto regexp_fragment : split)
        {
            if (regexp_fragment == "*")
            {
                current = current.star();
                continue;
            }

            if (regexp_fragment == "?")
            {
                current = current.question();
                continue;
            }
            
            if (regexp_fragment == "|")
            {
                switch (operation)
                {
                case '.':
                    result += current;
                    current = NFA();
                    operation = '|';
                    break;

                case '|':
                    result = result | current;
                    current = NFA();
                    break;

                default:
                    break;
                }
                continue;
            }

            if (current.size() != 0)
            {
                switch (operation)
                {
                case '.':
                    result += current;
                    current = NFA();
                    break;

                case '|':
                    result = result | current;
                    current = NFA();
                    operation = '.';
                    break;

                default:
                    break;
                }
            }

            if (regexp_fragment[0] == '[' && regexp_fragment[regexp_fragment.size() - 1] == ']')
            {
                current = NFAFactory::from_range(regexp_fragment);
            }
            else
            {
                current = NFAFactory::from_regexp(regexp_fragment, true);
            }
        }

        switch (operation)
        {
        case '.':
            result += current;
            break;

        case '|':
            result = result | current;
            break;

        default:
            break;
        }

        if (!not_first_pass)
        {
            NFAFactory::finalize(result);
        }

        return result;
    }

    RegexPreprocessor::encode_escaped_symbols(regex_clean);
    size_t union_pos = regex_clean.find('|');
    RegexPreprocessor::decode_escaped_symbols(regex_clean);

    if (union_pos != regex_clean.npos)
    {
        NFA left_part = NFAFactory::from_regexp(regex_clean.substr(0, union_pos), true);
        NFA right_part = NFAFactory::from_regexp(regex_clean.substr(union_pos+1), true);
        NFA result = left_part | right_part;
        
        if (!not_first_pass)
        {
            NFAFactory::finalize(result);
        }
        
        return result;
    }


    NFA result;
    NFA previous;

    std::map<char, char> change_map = {
        std::pair<char, char>('*', '\a'),
        std::pair<char, char>('?', '\f'),
    };

    std::map<char, char> change_back_map;
    for (auto pair : change_map)
    {
        change_back_map.emplace(pair.second, pair.first);
    }

    regex_clean = RegexPreprocessor::remove_escaping_slashes_and_change_operations(regex_clean, change_map);

    for (auto c : regex_clean)
    {
        if (previous.size() == 0)
        {
            char literal = change_back_map.contains(c) ? change_back_map.at(c) : c;
            previous = NFAFactory::from_literal(literal);
            continue;
        }

        if (c == change_map.at('*'))
        {
            result += previous.star();
            previous = NFA();
        }
        else if (c == change_map.at('?'))
        {
            result += previous.question();
            previous = NFA();
        }
        else
        {
            result += previous;
            previous = NFAFactory::from_literal(c);
        }
    }

    result += previous;


    if (!not_first_pass)
    {
        NFAFactory::finalize(result);
    }

    return result;
}

NFA NFAFactory::from_range(const std::string& range)
{
    NFA result;
    std::string range_clean = RegexPreprocessor::remove_escaping_slashes_and_change_operations(range, std::map<char, char>());

    result.resize(2);
    result.out_transition_trigger = '\0';
    result.out_node_index = 1;

    NondetermenisticNode start_node;
    NondetermenisticNode final_node;

    for (size_t i = 1; i < range_clean.size() - 1; i++)
    {
        start_node.emplace_transition(range_clean[i], final_node);
    }

    result[0] = start_node;
    result[1] = final_node;

    return result;
}

std::unordered_set<char> NFAFactory::construct_nfa_alphabet_set(const NFA& nfa)
{
    std::unordered_set<char> result_alphabet_set;

    for (size_t i = 0; i < nfa.size(); i++)
    {
        for (size_t j = 0; j < nfa[i].transitions.size(); j++)
        {
            char trigger = nfa[i].transitions[j].first;
            if (trigger == '\0') { continue; }

            result_alphabet_set.insert(trigger);
        }
    }
    
    return result_alphabet_set;
}

NFA& NFAFactory::finalize(NFA& nfa)
{
    NFA& result = nfa;

    NondetermenisticNode final_node;
    final_node.is_final = true;

    result.add_node(final_node);
    result[result.out_node_index].emplace_transition(result.out_transition_trigger, result[result.size() - 1]);
    result.out_transition_trigger = ' ';
    result.out_node_index = -1;

    std::unordered_set<char> result_alphabet_set = NFAFactory::construct_nfa_alphabet_set(result);

    result.alphabet = std::string(result_alphabet_set.begin(), result_alphabet_set.end());

    return result;
}
