#pragma once

#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct Problem 
{
    int rows;
    int cols;
    std::vector<int> counts;
};

std::vector<std::bitset<9>> parsePatterns(std::istream& input)
{
    std::vector<std::bitset<9>> patterns;
    std::string line;

    while (true)
    {
        std::streampos pos = input.tellg();   

        if (!std::getline(input, line))  break;

        if (line.find('x') != std::string::npos)
        {
            input.seekg(pos);
            break;
        }

        // stop when reaching first problem
        if (line.find('x') != std::string::npos) break;

        std::bitset<9> pattern;
        for (int row = 2; row >= 0; --row)
        {
            std::getline(input, line);
            for (int col = 0; col < 3; ++col)
            {
                if (line[col] == '#') pattern.set(3 * row + col);
            }
        }

        patterns.push_back(pattern);
        std::getline(input, line);
    }

    return patterns;
}

std::vector<Problem> parseProblems(std::ifstream& input)
{
    std::vector<Problem> problems;
    std::string line;

    while (std::getline(input, line))
    {
        std::stringstream ss(line);
        Problem p;
        char x, colon;  

        ss >> p.rows >> x >> p.cols >> colon;

        int value;
        while (ss >> value) p.counts.push_back(value);

        problems.push_back(p);
    }

    return problems;
}

std::bitset<9> rotate90(const std::bitset<9>& p)
{
    std::bitset<9> r;

    r[6] = p[8];
    r[3] = p[7];
    r[0] = p[6];

    r[7] = p[5];
    r[4] = p[4];
    r[1] = p[3];

    r[8] = p[2];
    r[5] = p[1];
    r[2] = p[0];

    return r;
}

std::bitset<9> rotate180(const std::bitset<9>& p)
{
    return rotate90(rotate90(p));
}

std::bitset<9> rotate270(const std::bitset<9>& p)
{
    return rotate90(rotate180(p));
}

std::bitset<9> flip_horizontal(const std::bitset<9>& p)
{
    std::bitset<9> r;

    r[6] = p[8];
    r[7] = p[7];
    r[8] = p[6];

    r[3] = p[5];
    r[4] = p[4];
    r[5] = p[3];

    r[0] = p[2];
    r[1] = p[1];
    r[2] = p[0];

    return r;
}

std::vector<std::unordered_set<std::bitset<9>>> patternCombinations(const std::vector<std::bitset<9>>& patterns)
{
    std::vector<std::unordered_set<std::bitset<9>>> combinations;
    combinations.reserve(patterns.size());

    for (const auto& pattern : patterns)
    {
        std::unordered_set<std::bitset<9>> curr;

        // original pattern
        curr.insert(pattern);

        // rotate 90 deg
        curr.insert(rotate90(pattern));

        // rotate 180 deg
        curr.insert(rotate180(pattern));

        // rotate 90 deg
        curr.insert(rotate270(pattern));
        
        // flip and rotations
        auto f0 = flip_horizontal(pattern);
        curr.insert(f0);
        curr.insert(rotate90(f0));
        curr.insert(rotate180(f0));
        curr.insert(rotate270(f0));

        combinations.push_back(curr);
    }

    return combinations;
}

bool can_place(const std::vector<std::bitset<100>>& grid,
               const std::bitset<9>& pat,
               int R, int C)
{
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (!pat.test(3 * row + col)) continue;
            if (grid[R + row].test(C + col)) return false;
        } 
    }

    return true;
}

void place(std::vector<std::bitset<100>>& grid,
           const std::bitset<9>& pat,
           int R, int C)
{
    for (int row = 0; row < 3; ++row)
    {
        std::bitset<100> row_mask;
        for (int col = 0; col < 3; ++col)
        {
            if (pat.test(3 * row + col)) row_mask.set(C + col);
        }
        grid[R + row] |= row_mask;
    }
}

void unplace(std::vector<std::bitset<100>>& grid,
             const std::bitset<9>& pat,
             int R, int C)
{
    for (int row = 0; row < 3; ++row)
    {
        std::bitset<100> row_mask;
        for (int col = 0; col < 3; ++col)
        {
            if (pat.test(3 * row + col)) row_mask.set(C + col);
        }
        grid[R + row] ^= row_mask;
    }
}

int backtrack(std::vector<int>& presents, 
              const std::vector<std::unordered_set<std::bitset<9>>>& combinations,
              std::vector<std::bitset<100>>& grid,
              int n)
{
    if (std::all_of(presents.begin(), presents.end(), [](int i) {return i==0;})) return 1;

    int i = 0;
    while (i < presents.size() && presents[i] == 0) ++i;

    for (const auto& pat : combinations[i])
    {
        for (int R = 0; R + 3 <= grid.size(); ++R)
        {
            for (int C = 0; C + 3 <= n; ++C)
            {
                if (!can_place(grid, pat, R, C)) continue;

                place(grid, pat, R, C);
                presents[i]--;

                if (backtrack(presents, combinations, grid, n)) return 1;

                presents[i]++;
                unplace(grid, pat, R, C);
            }
        }
    }

    return 0;
}

int partI(const std::vector<Problem>& problems,
          const std::vector<std::unordered_set<std::bitset<9>>>& combinations)
{
    int canFit = 0;
    //int i = 0;
    for (const auto& problem : problems)
    {
        //std::cout << "Running problem number " << i++ << " of " << problems.size() <<"\n"; 
        std::vector<std::bitset<100>> grid(problem.rows);
        std::vector<int> presents = problem.counts;

        int totalArea = 0;
        for (int i = 0; i < presents.size(); ++i) totalArea += presents[i] * combinations[i].begin()->count();

        if (totalArea > problem.cols * problem.rows) continue;

        canFit += backtrack(presents, combinations, grid, problem.cols);
    }

    return canFit;
}