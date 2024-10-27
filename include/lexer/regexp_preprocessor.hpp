#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>


class RegexpPreprocessor
{
public:
    static std::string clean_and_expand(std::string regexp);
    static std::vector<std::string> split_upper_level_groups(std::string regexp);
    static std::string remove_escaping_slashes_and_change_operations(std::string regexp, std::map<char, std::string> change_map);

    static std::map<std::string, std::string> encode_map;
    static void encode_escaped_symbols(std::string& regexp);
    static void decode_escaped_symbols(std::string& regexp);
};