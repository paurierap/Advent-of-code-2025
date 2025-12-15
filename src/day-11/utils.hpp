#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, std::vector<std::string>> getMap(std::ifstream& input)
{
    std::string line;
    std::unordered_map<std::string, std::vector<std::string>> mp;

    while (std::getline(input, line))
    {
        std::stringstream ss(line);
        std::string key;
        std::vector<std::string> output;

        std::getline(ss, key, ':');
        std::string value;
        while (ss >> value) mp[key].push_back(value);
    }

    return mp;
}

// DFS for part I using memoisation with dp
long long dfsPartI(const std::unordered_map<std::string, std::vector<std::string>>& mp,
              std::unordered_map<std::string, long long>& dp,
              const std::string& curr)
{
    if (curr == "out") return 1;
    if (dp.contains(curr)) return dp[curr];

    long long paths = 0;
    for (const auto& s : mp.at(curr))
    {
        paths += dfsPartI(mp, dp, s);
    }

    dp[curr] = paths;
    return paths;
}

long long partI(const std::unordered_map<std::string, std::vector<std::string>>& mp)
{
    if (!mp.contains("you")) return 0;
    std::unordered_map<std::string, long long> dp;
    return dfsPartI(mp, dp, "you");
}

// DFS for part II using memoisation with dp, including the 4 states in which we can reach a string: 0 = 0b00 -> no dac nor fft, 1 = 0b01 -> dac but no fft, 2 = 0b10 -> fft but no dac, 3 = 0b11 -> dac and fft
long long dfsPartII(const std::unordered_map<std::string, std::vector<std::string>>& mp,
                    const std::string& curr, 
                    int mask, // bit 0: dac, bit 1: fft
                    std::unordered_map<std::string, std::array<long long, 4>>& dp)
{
    if (curr == "out") return mask == 0b11;

    if (!dp.contains(curr)) dp[curr].fill(-1);

    if (dp[curr][mask] != -1) return dp[curr][mask];

    long long paths = 0;
    for (const auto& s : mp.at(curr)) 
    {
        int next_mask = mask;
        if (s == "dac") next_mask |= 0b01;
        if (s == "fft") next_mask |= 0b10;

        paths += dfsPartII(mp, s, next_mask, dp);
    }

    dp[curr][mask] = paths;
    return paths;
}

long long partII(const std::unordered_map<std::string, std::vector<std::string>>& mp)
{
    std::unordered_map<std::string, std::array<long long, 4>> dp;
    return dfsPartII(mp, "svr", 0, dp);
}