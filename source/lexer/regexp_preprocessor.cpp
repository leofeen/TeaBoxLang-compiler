#include "regexp_preprocessor.hpp"

std::string RegexpPreprocessor::clean_ranges(const std::string& regexp)
{
    std::string result;

    bool is_in_list = false;
    int range_start_value = -1;
    bool range_ready = false;

    for (int i = 0; i < regexp.size(); i++)
    {
        char c = regexp[i];
        switch (c)
        {
        case '[':
            is_in_list = true;
            break;

        case ']':
            is_in_list = false;
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

    return result;
}

std::vector<std::string> RegexpPreprocessor::split_upper_level_groups(const std::string& regexp)
{
    std::vector<std::string> result;

    int open_groups = 0;
    size_t start_of_group = 0;

    for (size_t i = 0; i < regexp.size(); i++)
    {
        char c = regexp[i];
        switch (c)
        {
        case '(':
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
                    
                default:
                    break;
                }
            }
            break;

        case '|':
            if (open_groups == 0)
            {
                result.push_back(regexp.substr(start_of_group, i - start_of_group));
                result.push_back("|");
                start_of_group =  i+1;           
            }
            break;

        case '*':
            if (open_groups == 0)
            {
                result.push_back(regexp.substr(start_of_group, i - start_of_group));
                result.push_back("*");
                start_of_group =  i+1;           
            }
            break;

        default:
            break;
        }
    }

    return result;
}
