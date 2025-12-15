#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>

/*
For each manifold, a tachyon may undergo a number of beam splits (Part I) and, if it is a quantum tachyon, propagate through multiple timelines (Part II).
*/
struct Manifold
{
    int splits;
    long long timelines;
};

/*
The first row is assumed to contain exactly one 'S', the starting point of the tachyon.
Each subsequent row represents a new manifold layer.

Part I and Part II are solved simultaneously using a top-down dynamic programming approach.

DP definition:
- dp[i] represents the number of distinct ways the tachyon can reach column i
  in the current row.

Transition rule:
- If a splitter '^' is encountered at column i and dp[i] > 0:
    - The beam splits left and right
    - Its contribution is added to columns i-1 and i+1
    - The vertical contribution is removed (dp[i] becomes 0)
    - The split counter is incremented

Important assumptions guaranteed by the problem:
- Splitters never appear at the leftmost or rightmost columns
  (so accessing i-1 and i+1 is always safe).
- dp values grow exponentially and must be accumulated in long long.

Take the example, then using the splitting norm for every row we would have the following grid/matrix: 

0 0 0 0  0 0  0 S  0 0 0 0 0 0 0
0 0 0 0  0 0  0 1  0 0 0 0 0 0 0
0 0 0 0  0 0  1 0  1 0 0 0 0 0 0
0 0 0 0  0 0  1 0  1 0 0 0 0 0 0
0 0 0 0  0 1  0 2  0 1 0 0 0 0 0
0 0 0 0  0 1  0 2  0 1 0 0 0 0 0
0 0 0 0  1 0  3 0  3 0 1 0 0 0 0
0 0 0 0  1 0  3 0  3 0 1 0 0 0 0
0 0 0 1  0 4  0 3  3 1 0 1 0 0 0
0 0 0 1  0 4  0 3  3 1 0 1 0 0 0
0 0 1 0  5 0  4 3  4 0 2 0 1 0 0
0 0 1 0  5 0  4 3  4 0 2 0 1 0 0
0 1 0 1  5 4  0 7  4 0 2 1 0 1 0
0 1 0 1  5 4  0 7  4 0 2 1 0 1 0
1 0 2 0 10 0 11 0 11 0 2 1 1 0 1
1 0 2 0 10 0 11 0 11 0 2 1 1 0 1

At the end, the total number of timelines is just the number of possible paths to reach the last row, so we only need to add all the individual elements: 1+2+10+11+11+2+1+1+1 = 40.
*/

Manifold analyseManifold(std::ifstream& input)
{
    std::string row;
    int splits = 0;

    input >> row;
    std::vector<long long> dp(row.length(), 0);

    // Find starting position S:
    int S = std::find(row.begin(), row.end(), 'S') - row.begin();
    dp[S] = 1LL;

    while (input >> row)
    {
        std::vector<long long> next(dp);

        // We assume no splitters at right or left edge of the manifold:
        for (int i = 1; i < row.size()-1; ++i)
        {
            if (row[i] == '^' && next[i] != 0)
            {
                next[i-1] += dp[i];
                next[i+1] += dp[i];
                next[i] = 0;
                splits++;
            }
        }

        std::swap(next, dp);
    }

    return {splits, std::accumulate(dp.begin(), dp.end(), 0LL)};
};

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);
    
    Manifold M = analyseManifold(input);

    std::cout << "The beam will be split (Part I) " << M.splits << " times\n";
    std::cout << "A single tachuon particle would end up on (Part II) " << M.timelines << " timelines\n";

    return EXIT_SUCCESS;
}
