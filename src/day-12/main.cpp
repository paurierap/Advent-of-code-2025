#include "utils.hpp"

#include <bitset>
#include <iostream>
#include <fstream>
#include <vector>

/*
Day 12 — Packing Problem

Approach:
- Generate all rotations/reflections of patterns
- Backtracking search to place patterns without overlap
- Use pruning on pattern order and area constraints
- Return existence (not count)

This works within AoC puzzle input bounds.
*/


int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);
    std::vector<std::bitset<9>> patterns = parsePatterns(input);
    std::vector<Problem> problems = parseProblems(input);
    std::vector<std::unordered_set<std::bitset<9>>> combinations = patternCombinations(patterns);

    int regionsPartI = partI(problems, combinations);
    std::cout << "The number of regions that can fit all the presents listed is " << regionsPartI << "\n";

    return EXIT_SUCCESS;
}