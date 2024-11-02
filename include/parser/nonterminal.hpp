#pragma once

#include <vector>
#include <string>


typedef std::vector<std::string> Transition;

struct Nonterminal
{
    std::string label;
    std::vector<std::vector<std::string>> transitions; // Labels for Nonterminal and verbose names for TokenType

    Nonterminal() = default;
    Nonterminal(std::string label, std::vector<Transition> transitions) : label(label), transitions(transitions) {}
};
