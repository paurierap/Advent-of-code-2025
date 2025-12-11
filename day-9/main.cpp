#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

<<<<<<< HEAD
=======
// cache[y] = vector of {x_start, x_end} intervals that are INSIDE at that y
static std::unordered_map<int, std::vector<std::pair<int,int>>> horizontalInside;

// cache[x] = vector of {y_start, y_end} intervals that are INSIDE at that x
static std::unordered_map<int, std::vector<std::pair<int,int>>> verticalInside;

>>>>>>> 60801933dc1de94435872eb65241a73e00176f4b
bool isRectangleEnclosed(const std::vector<int>&, const std::vector<int>&, const std::vector<std::vector<int>>&);
long long calculateArea(const std::vector<int>&, const std::vector<int>&);

std::vector<std::vector<int>> getPolygon(std::ifstream& input)
{
    std::vector<std::vector<int>> polygon;
    int x, y;
    char comma;

    while (input >> x >> comma >> y) polygon.push_back({x, y});

    return polygon;
};

long long partI(const std::vector<std::vector<int>>& polygon)
{
    long long largestArea = 0;
    for (int i = 0; i < polygon.size(); ++i)
    {
        for (int j = i + 1; j < polygon.size(); ++j) largestArea = std::max(largestArea, calculateArea(polygon[i], polygon[j]));
    }

    return largestArea;
}

long long partII(const std::vector<std::vector<int>>& polygon)
{
    long long largestArea = 0;
    int n = polygon.size();
    
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j) 
        {
            int area = calculateArea(polygon[i], polygon[j]);

            // Prune by area to avoid innecessary calculations
            if (area > largestArea)
            {
                if (isRectangleEnclosed(polygon[i], polygon[j], polygon)) largestArea = area;
            }
        }
    }

    return largestArea;
}

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);
    std::vector<std::vector<int>> polygon = getPolygon(input);

    std::cout << "The largest area of any rectangle we can make in Part I is " << partI(polygon) << "\n";

    const auto start{std::chrono::steady_clock::now()};
    long long largestRectanglePartII = partII(polygon);
<<<<<<< HEAD
=======
    const auto finish{std::chrono::steady_clock::now()};
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
>>>>>>> 60801933dc1de94435872eb65241a73e00176f4b

    std::cout << "The largest area of any rectangle we can make in Part II is " << largestRectanglePartII << "\n";
    std::cout << "Code execution for Part II took " << time << " ms\n";

    return EXIT_SUCCESS;
}
