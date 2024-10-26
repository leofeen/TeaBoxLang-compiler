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
    std::string regexp_clean = not_first_pass ? regexp : RegexpPreprocessor::clean_and_expand(regexp);

    size_t open_group_pos = regexp_clean.find('(');
    size_t open_range_pos = regexp_clean.find('[');
    if (open_group_pos != regexp_clean.npos || open_range_pos != regexp_clean.npos)
    {
        auto split = RegexpPreprocessor::split_upper_level_groups(regexp_clean);

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

    size_t union_pos = regexp_clean.find('|');
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
    for (auto c : regexp_clean)
    {
        if (c == '*')
        {
            if (previous.size() == 0)
            {
                previous = NFAFactory::from_literal(c);
                continue;
            }

            result += previous.star();
            previous = NFA();
        }
        // else if (c == '+')
        // {
        //     if (previous.size() == 0)
        //     {
        //         previous = NFAFactory::from_literal(c);
        //         continue;
        //     }

        //     result += previous.plus();
        //     previous = NFA();
        // }
        else if (c == '?')
        {
            if (previous.size() == 0)
            {
                previous = NFAFactory::from_literal(c);
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

    result.resize(2);
    result.out_transition_trigger = '\0';
    result.out_node_index = 1;

    AutomatonNode start_node;
    AutomatonNode final_node;

    for (size_t i = 1; i < range.size() - 1; i++)
    {
        start_node.emplace_transition(range[i], final_node);
    }

    result[0] = start_node;
    result[1] = final_node;

    return result;
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

    std::unordered_set<char> result_alphabet_set;

    for (size_t i = 0; i < result.size(); i++)
    {
        for (size_t j = 0; j < result[i].transitions.size(); j++)
        {
            char trigger = result[i].transitions[j].first;
            if (trigger == '\0') { continue; }

            result_alphabet_set.insert(trigger);
        }
    }

    result.alphabet = std::string(result_alphabet_set.begin(), result_alphabet_set.end());

    return result;
}
