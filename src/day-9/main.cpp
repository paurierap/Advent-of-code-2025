#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);
    std::vector<std::vector<int>> polygon = getPolygon(input);

    long long largestRectanglePartI = partI(polygon);
    std::cout << "The largest area of any rectangle we can make in Part I is " << largestRectanglePartI << "\n";

    long long largestRectanglePartII = partII(polygon);
    std::cout << "The largest area of any rectangle we can make in Part II is " << largestRectanglePartII << "\n";

    return EXIT_SUCCESS;
}
