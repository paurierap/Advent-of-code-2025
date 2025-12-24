#include "utils.hpp"

#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);
    std::vector<Box> boxes = getBoxes(input);
    std::pair<long long, long long> answer = connectBoxes(boxes, 1000);

    std::cout << "Multiplying together the sizes of the three largest circuits gives " << answer.first << "\n";

    std::cout << "Multiplying together the X coordinates of the last two junction boxes needed to be connected gives " << answer.second << "\n";

    return EXIT_SUCCESS;
}
