#include "nfa_factory.hpp"


NFA NFAFactory::from_literal(const char literal)
{
    NFA result;
    result.add_node(AutomatonNode());
    result.out_transition_trigger = literal;

    return result;
}

NFA NFAFactory::from_regexp(const std::string& regexp, const bool not_first_pass = false)
{
    std::string regexp_clean = not_first_pass ? regexp : RegexPreprocessor::clean_and_expand(regexp);

    size_t open_group_pos = regexp_clean.find('(');
    size_t open_range_pos = regexp_clean.find('[');
    if (open_group_pos != regexp_clean.npos || open_range_pos != regexp_clean.npos)
    {
        auto split = RegexPreprocessor::split_upper_level_groups(regexp_clean);

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

            // if (regexp_fragment == "+")
            // {
            //     current = current.plus();
            //     continue;
            // }

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

    RegexPreprocessor::encode_escaped_symbols(regexp_clean);
    size_t union_pos = regexp_clean.find('|');
    RegexPreprocessor::decode_escaped_symbols(regexp_clean);

    if (union_pos != regexp_clean.npos)
    {
        NFA left_part = NFAFactory::from_regexp(regexp_clean.substr(0, union_pos), true);
        NFA right_part = NFAFactory::from_regexp(regexp_clean.substr(union_pos+1), true);
        NFA result = left_part | right_part;
        
        if (!not_first_pass)
        {
            NFAFactory::finalize(result);
        }
        
        return result;
    }


    NFA result;
    NFA previous;

    std::map<char, std::string> change_map = {
        std::pair<char, std::string>('*', "\a"),
        std::pair<char, std::string>('?', "\f"),
    };

    regexp_clean = RegexPreprocessor::remove_escaping_slashes_and_change_operations(regexp_clean, change_map);

    for (auto c : regexp_clean)
    {
        if (c == change_map.at('*')[0])
        {
            if (previous.size() == 0)
            {
                previous = NFAFactory::from_literal(change_map.at('*')[0]);
                continue;
            }

            result += previous.star();
            previous = NFA();
        }
        else if (c == change_map.at('?')[0])
        {
            if (previous.size() == 0)
            {
                previous = NFAFactory::from_literal(change_map.at('?')[0]);
                continue;
            }

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
    std::string range_clean = RegexPreprocessor::remove_escaping_slashes_and_change_operations(range, std::map<char, std::string>());

    result.resize(2);
    result.out_transition_trigger = '\0';
    result.out_node_index = 1;

    AutomatonNode start_node;
    AutomatonNode final_node;

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

    AutomatonNode final_node = AutomatonNode();
    final_node.is_final = true;

    result.add_node(final_node);
    result[result.out_node_index].emplace_transition(result.out_transition_trigger, result[result.size() - 1]);
    result.out_transition_trigger = ' ';
    result.out_node_index = -1;

    std::unordered_set<char> result_alphabet_set = NFAFactory::construct_nfa_alphabet_set(result);

    result.alphabet = std::string(result_alphabet_set.begin(), result_alphabet_set.end());

    return result;
}
