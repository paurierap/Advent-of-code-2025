#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

struct Range 
{
    long long start;
    long long end;
};

std::vector<Range> getRanges(std::ifstream& input)
{
    std::string line;
    std::vector<Range> ranges;
    while (std::getline(input, line))
    {
        if (line.empty()) break;

        std::stringstream ss(line);
        Range range;
        char c;

        ss >> range.start >> c >> range.end;
        ranges.push_back(range);
    }

    std::sort(ranges.begin(), ranges.end(), [](const Range& a, const Range& b)
    {
        if (a.start == b.start) return a.end < b.end;
        return a.start < b.start;
    });

    return ranges;
}

// Optionally, we could implement binary search over the sorted vector, but the input size is relatively low, so probably not worth the effort. 
int getAvailableFreshIDs(std::ifstream& input, const std::vector<Range>& ranges)
{
    int fresh = 0;
    std::string line;
    while (input >> line)
    {
        long long ID = std::stoll(line);

        for (const auto& range : ranges)
        {
            if (ID < range.start) break;
            if (ID >= range.start && ID <= range.end)
            {
                fresh++;
                break;
            }
        }
    }

    return fresh;
}

// Merging interval ranges:
long long getFreshIDs(std::vector<Range>& ranges) 
{ 
    long long fresh = 0; 
    long long currEnd = -1; 
    
    for (const auto& range : ranges) 
    { 
        if (range.start > currEnd) fresh += range.end - range.start + 1; 
        else if (range.start == currEnd) fresh += range.end - range.start; 
        else 
        {  
            if (currEnd < range.end) fresh += range.end - currEnd; 
        } 
    
        currEnd = std::max(currEnd, range.end); 
    } 
    
    return fresh; 
}


int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);
    
    // Get fresh ingredient ranges:
    std::vector<Range> ranges = getRanges(input);

    std::cout << "The number of available fresh ingredients (Part I) is " << getAvailableFreshIDs(input, ranges) << "\n";
    std::cout << "The number of fresh ingredients (Part II) is " << getFreshIDs(ranges) << "\n";

    return EXIT_SUCCESS;
}
