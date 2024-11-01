#pragma once

#include <vector>
#include <string>


struct Nonterminal
{
    std::string label;
    std::vector<std::vector<std::string>> transitions; // Labels for Nonterminal and verbose names for TokenType

    Nonterminal() = default;
    Nonterminal(std::string label, std::vector<std::vector<std::string>> transitions) : label(label), transitions(transitions) {}
};