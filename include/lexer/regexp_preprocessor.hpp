#pragma once

#include <string>
#include <vector>
#include <iostream>


class RegexpPreprocessor
{
public:
    static std::string clean_and_expand(const std::string& regexp);
    static std::vector<std::string> split_upper_level_groups(const std::string& regexp);
};