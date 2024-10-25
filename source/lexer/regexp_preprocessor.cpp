#include "regexp_preprocessor.hpp"

std::string RegexpPreprocessor::clean_and_expand(const std::string& regexp)
{
    std::string first_pass;

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
            first_pass += c;
            break;

        case ']':
            is_in_list = false;
            if (range_start_value != -1)
            {
                first_pass += static_cast<char>(range_start_value);
                range_start_value = -1;
            }
            if (range_ready)
            {
                range_ready = false;
                first_pass += '-';
            }
            first_pass += c;
            break;
            
        default:
            if (!is_in_list) 
            { 
                first_pass += c;
                break;
            }

            if (range_start_value == -1 && c != '-')
            {
                range_start_value = c;
                break;
            }

            if (range_start_value == -1 && c == '-')
            {
                first_pass += c;
                break;
            }

            if (c == '-')
            {
                range_ready = true;
                break;
            }

            if (!range_ready)
            {
                first_pass += range_start_value;
                range_start_value = c;
                break;
            }

            for (char range_char = range_start_value; range_char <= c; range_char++)
            {
                first_pass += range_char;
            }

            range_ready = false;
            range_start_value = -1;
            break;
        }
    }

    // std::string result;

    // is_in_list = false;

    // for (int i = 0; i < first_pass.size(); i++)
    // {
    //     char c = first_pass[i];
    //     switch (c)
    //     {
    //     case '[':
    //         if (is_in_list) { continue; }
    //         is_in_list = true;
    //         result += '(';
    //         break;

    //     case ']':
    //         if (!is_in_list) { continue; }
    //         is_in_list = false;
    //         result += ')';
    //         break;
            
    //     default:
    //         result += c;
    //         if (!is_in_list) { break; } 
    //         if (first_pass[i+1] == ']') { break; }

    //         result += '|';
    //         break;
    //     }
    // }

    // return result;

    return first_pass;
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
                start_of_group =  i+1;           
            }
            break;

        default:
            break;
        }
    }

    return result;
}
