#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int countNeighbors(int i, int j, const std::vector<std::string>& grid)
{
    int count = 0;
    int m = grid.size();
    int n = grid[0].size();

    for (int di = -1; di <= 1; ++di)
    {
        for (int dj = -1; dj <= 1; ++dj)
        {
            if (di == 0 && dj == 0) continue;

            int ni = i + di;
            int nj = j + dj;

            if (ni >= 0 && ni < m && nj >= 0 && nj < n) count += (grid[ni][nj] == '@');
        }
    }
    
    return count;
}

int partI(const std::vector<std::string>& grid)
{
    int rolls = 0;

    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[0].size(); j++)
        {
            if (grid[i][j] == '@' && countNeighbors(i, j, grid) < 4) rolls++;
        }
    }

    return rolls;
}

/*
    We update the grid in-place because the process is strictly monotonic: cells only change from '@' → '.', never the reverse. Therefore the sequence of states always moves toward the same fixed point,
    and the final steady-state does not depend on the order of updates.
*/

int partII(std::vector<std::string>& grid)
{
    int rolls = -1;
    int new_rolls = 0;

    while (rolls != new_rolls)
    {
        rolls = new_rolls;

        for (int i = 0; i < grid.size(); ++i)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {
                if (grid[i][j] == '@' && countNeighbors(i, j, grid) < 4) 
                {
                    grid[i][j] = '.';
                    new_rolls++;
                }
            }
        }
    }
    
    return rolls;
}

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);

    std::vector<std::string> grid;

    // Create grid
    std::string row;
    while (input >> row) grid.push_back(row);

    std::cout << "The number of rolls of paper that can be accessed by a forklift in Part I is " << partI(grid) << "\n";
    std::cout << "The number of rolls of paper that can be accessed by a forklift in Part I is " << partII(grid) << "\n";

    return EXIT_SUCCESS;
}
