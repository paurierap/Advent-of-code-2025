#include <iostream>
#include <fstream>
#include <vector>

void parseManual(std::ifstream&, 
                 std::vector<std::vector<bool>>&, 
                 std::vector<std::vector<std::vector<int>>>&, 
                 std::vector<std::vector<int>>&);
long partI(const std::vector<std::vector<bool>>&, 
           const std::vector<std::vector<std::vector<int>>>&);
long partII(const std::vector<std::vector<std::vector<int>>>&,
            const std::vector<std::vector<int>>&);

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);
    std::vector<std::vector<bool>> indicators;
    std::vector<std::vector<std::vector<int>>> wiring;
    std::vector<std::vector<int>> jrequirements;
    
    parseManual(input, indicators, wiring, jrequirements);

    long min_presses_partI = partI(indicators, wiring);
    std::cout << "The fewest button presses required to correctly configure the indicator lights on all of the machines is " << min_presses_partI << "\n";

    long long min_presses_partII = partII(wiring, jrequirements);
    std::cout << "The fewest button presses required to correctly configure the joltage level counters on all of the machines is " << min_presses_partII << "\n";

    return EXIT_SUCCESS;
}