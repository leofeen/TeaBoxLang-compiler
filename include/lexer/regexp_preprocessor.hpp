#pragma once

#include <string>
#include <vector>


class RegexpPreprocessor
{
public:
    static std::string clean_ranges(const std::string& regexp);
    static std::vector<std::string> split_upper_level_groups(const std::string& regexp);
};