#include "regexp_preprocessor.hpp"

std::string RegexpPreprocessor::clean_and_expand(const std::string& regexp)
{
    std::string result;

    bool is_in_list = false;
    int range_start_value = -1;
    bool range_ready = false;

    for (size_t i = 0; i < regexp.size(); i++)
    {
        char c = regexp[i];
        switch (c)
        {
        case '[':
            is_in_list = true;
            result += c;
            break;

        case ']':
            is_in_list = false;
            if (range_start_value != -1)
            {
                result += static_cast<char>(range_start_value);
                range_start_value = -1;
            }
            if (range_ready)
            {
                range_ready = false;
                result += '-';
            }
            result += c;
            break;
            
        default:
            if (!is_in_list) 
            { 
                result += c;
                break;
            }

            if (range_start_value == -1 && c != '-')
            {
                range_start_value = c;
                break;
            }

            if (range_start_value == -1 && c == '-')
            {
                result += c;
                break;
            }

            if (c == '-')
            {
                range_ready = true;
                break;
            }

            if (!range_ready)
            {
                result += range_start_value;
                range_start_value = c;
                break;
            }

            for (char range_char = range_start_value; range_char <= c; range_char++)
            {
                result += range_char;
            }

            range_ready = false;
            range_start_value = -1;
            break;
        }
    }

    while (result.find('+') != result.npos)
    {
        size_t plus_index = result.find('+');
        char previous_symbol = result[plus_index - 1];

        size_t open_index = -1;
        size_t open_groups = 1;
        switch (previous_symbol)
        {
        case ')':
            for (size_t i = 2; i <= plus_index; i++)
            {
                if (open_index != -1) { break; }
                switch (result[plus_index-i])
                {
                    case ')':
                        open_groups++;
                        break;

                    case '(':
                        open_groups--;
                        if (open_groups == 0)
                        {
                            open_index = plus_index - i;
                            break;
                        }
                        break;

                    default:
                        break;
                }
            }

            result.replace(plus_index, 1, "*");
            result.insert(plus_index - 1, result.substr(open_index, plus_index - open_index));
            break;
        
        case ']':
            for (size_t i = 2; i <= plus_index; i++)
            {
                if (result[plus_index-i] == '[')
                {
                    open_index = plus_index - i;
                    break;
                }
            }

            result.replace(plus_index, 1, "*");
            result.insert(plus_index - 1, result.substr(open_index, plus_index - open_index));
            break;
        
        default:
            result.replace(plus_index, 1, "*");
            result.insert(plus_index - 1, 1, previous_symbol);
            break;
        }
    }

    return result;
}

std::vector<std::string> RegexpPreprocessor::split_upper_level_groups(const std::string& regexp)
{
    std::vector<std::string> result;

    int open_groups = 0;
    size_t start_of_group = 0;
    bool is_in_range = false;

    for (size_t i = 0; i < regexp.size(); i++)
    {
        char c = regexp[i];
        switch (c)
        {
        case '[':
            if (is_in_range) { break; }
            if (open_groups != 0) { break; }
            if (i - start_of_group > 0)
            {
                result.push_back(regexp.substr(start_of_group, i - start_of_group));
            }
            start_of_group = i;

            is_in_range = true;
            break;

        case ']':
            if (!is_in_range) { break; }
            is_in_range = false;
            if (i - start_of_group > 0)
            {
                result.push_back(regexp.substr(start_of_group, i - start_of_group) + ']');
            }
            start_of_group = i+1;
            break;

        case '(':
            if (is_in_range) { break; }
            open_groups++;
            if (open_groups == 1) 
            {
                if (i - start_of_group > 0)
                {
                    result.push_back(regexp.substr(start_of_group, i - start_of_group));
                }
                start_of_group = i;
            }
            break;

        case ')':
            if (open_groups == 0) { break; }
            if (is_in_range) { break; }

            open_groups--;
            if (open_groups == 0)
            {
                result.push_back(regexp.substr(start_of_group+1, i - start_of_group - 1));
                start_of_group = i+1;
                
                char next = regexp[i+1];
                switch (next)
                {
                case '|':
                    result.push_back("|");
                    i++;
                    break;

                case '*':
                    result.push_back("*");
                    i++;
                    if (regexp[i+1] == '|')
                    {
                        result.push_back("|");
                        i++;
                    }
                    break;

                case '+':
                    result.push_back("+");
                    i++;
                    if (regexp[i+1] == '|')
                    {
                        result.push_back("|");
                        i++;
                    }
                    break;

                case '?':
                    result.push_back("?");
                    i++;
                    if (regexp[i+1] == '|')
                    {
                        result.push_back("|");
                        i++;
                    }
                    break;

                default:
                    break;
                }
            }
            break;

        case '|':
            if (is_in_range) { break; }
            if (open_groups == 0)
            {
                result.push_back(regexp.substr(start_of_group, i - start_of_group));
                result.push_back("|");
                start_of_group =  i+1;           
            }
            break;

        case '*':
            if (is_in_range) { break; }
            if (open_groups == 0)
            {
                result.push_back(regexp.substr(start_of_group, i - start_of_group));
                result.push_back("*");
                start_of_group =  i+1;           
            }
            break;
        
        case '+':
            if (is_in_range) { break; }
            if (open_groups == 0)
            {
                result.push_back(regexp.substr(start_of_group, i - start_of_group));
                result.push_back("+");
                start_of_group =  i+1;           
            }
            break;

        case '?':
            if (is_in_range) { break; }
            if (open_groups == 0)
            {
                result.push_back(regexp.substr(start_of_group, i - start_of_group));
                result.push_back("?");
                start_of_group = i+1;           
            }
            break;

        default:
            break;
        }
    }

    if (start_of_group != regexp.size())
    {
        result.push_back(regexp.substr(start_of_group, regexp.size() - start_of_group));
    }

    return result;
}
