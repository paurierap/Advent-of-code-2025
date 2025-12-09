#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

struct Range
{
    int start;
    int end;
};

long long partI(std::ifstream& input)
{  
    std::vector<int> numbers;
    std::string s;

    // Get numbers
    while (std::getline(input, s))
    {
        std::stringstream ss(s);
        int num;
        
        if (ss >> num) 
        {
            numbers.push_back(num);
            while (ss >> num) numbers.push_back(num);
        }
        else
        {
            // Get the operations to perform
            s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char c){ return std::isspace(c); }), s.end());
            break;
        } 
    }

    // Perform operations
    long long total = 0;
    for (int i = 0; i < s.size(); ++i)
    {
        long long problem = (s[i] == '*') ? 1 : 0;
        if (s[i] == '*')
        {
            for (int j = i; j < numbers.size(); j += s.size()) problem *= numbers[j];
        }
        else if (s[i] == '+')
        {
            for (int j = i; j < numbers.size(); j += s.size()) problem += numbers[j];
        }

        total += problem;
    }

    return total;
}

long long partII(std::ifstream& input)
{ 
    input.clear();
    input.seekg(0);

    std::vector<std::string> lines;
    std::string s;
    while (std::getline(input, s)) lines.push_back(s);

    if (lines.empty()) return 0;

    // s now contains the operations
    s = lines.back();
    lines.pop_back();
    s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char c){ return std::isspace(c); }), s.end());

    // Get the column index ranges for every problem
    std::vector<Range> ranges;
    int start = 0, end = 0;
    while (end < lines[0].size())
    {
        end = start;
        for (const auto& line : lines)
        {
            int currEnd = end;
            while (currEnd < line.length() && line[currEnd] != ' ') currEnd++;

            end = std::max(currEnd, end);
        }
        ranges.push_back({start, end-1});

        // Skip blank space
        start = end + 1;
    }
    
    // Perform operations
    long long total = 0;
    for (int i = 0; i < s.size(); ++i)
    {
        int left = ranges[i].start, right = ranges[i].end;
        long long problem = (s[i] == '*') ? 1 : 0;
        while (left <= right)
        {
            std::string number;
            for (const auto& line : lines) number += line[right];

            if (s[i] == '*') problem *= std::stoi(number);
            else if (s[i] == '+') problem += std::stoi(number);
            right--;
        }

        total += (s[i] == '+') ? problem-1 : problem;
    }

    return total;
}

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);

    std::cout << "The grand total found by adding together all of the answers to the individual problems in Part I is " << partI(input) << "\n";

    std::cout << "The grand total found by adding together all of the answers to the individual problems in Part II is " << partII(input) << "\n";

    return EXIT_SUCCESS;
}
