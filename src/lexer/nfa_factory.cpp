#include <nfa_factory.hpp>


NFA NFAFactory::from_literal(const char literal)
{
    NFA result;
    result.add_node(AutomatonNode());
    result.out_transition_trigger = literal;

    return result;
}

NFA NFAFactory::from_regexp(const std::string& regexp, const bool not_first_pass = false)
{
    std::string regexp_rangeless = not_first_pass ? regexp : RegexpPreprocessor::clean_ranges(regexp);

    size_t open_group_pos = regexp.find('(');
    if (open_group_pos != regexp.npos)
    {
        auto split = RegexpPreprocessor::split_upper_level_groups(regexp_rangeless);

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

            current = NFAFactory::from_regexp(regexp_fragment, true);
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

        return result;
    }

    size_t union_pos = regexp_rangeless.find('|');
    if (union_pos != regexp_rangeless.npos)
    {
        return NFAFactory::from_regexp(regexp_rangeless.substr(0, union_pos), true) | NFAFactory::from_regexp(regexp_rangeless.substr(union_pos+1), true);
    }


    NFA result;
    NFA previous;
    for (auto c : regexp_rangeless)
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
        else
        {
            result += previous;
            previous = NFAFactory::from_literal(c);
        }
    }

    result += previous;

    if (!not_first_pass)
    {
        AutomatonNode final_node = AutomatonNode();
        final_node.is_final = true;

        result.add_node(final_node);
        result[result.out_node_index].add_transition(result.out_transition_trigger, result[result.size() - 1]);
        result.out_transition_trigger = ' ';
        result.out_node_index = -1;
    }

    return result;
}
