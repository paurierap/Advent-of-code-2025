#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);
    std::unordered_map<std::string, std::vector<std::string>> mp = getMap(input);

    long long pathsPartI = partI(mp);
    std::cout << "The number of differents paths leading from 'you' to 'out' is " << pathsPartI << "\n";

    long long pathsPartII = partII(mp);
    std::cout << "The number of differents paths leading from 'svr' to 'out' visiting both 'dac' and 'fft' is " << pathsPartII << "\n";

    return EXIT_SUCCESS;
}