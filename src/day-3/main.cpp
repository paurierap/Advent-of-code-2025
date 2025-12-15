#include <fstream>
#include <iostream>
#include <string>

/*
We are given a several "banks" (strings of digits) and we must select `n` digits from it,
preserving order, to form the largest possible number.

Constraints:
- We must pick exactly `n` digits.
- Digits must be chosen left-to-right (no reordering).
- After picking a digit at position `i`, the next digit must come from positions > i.
- To ensure it's possible to pick the remaining digits, the choice for the k-th digit
  can only range up to index: bank.length() - (n - k).

Algorithm:
A greedy approach that selects the largest possible digit that still leaves enough characters to pick the remaining digits. For each digit:
    - Scan the allowed range [start_index, max_allowed_index]
    - Choose the largest digit in that range, preferring earlier occurrences
      (e.g. leftmost '9').
    - Record its position so the next digit search starts to the right of it.
    - Append that chosen digit to the result.

Minor optimization:
    If the digit '9' appears in the allowed range, we can stop early since no
    larger digit exists.

Example:
    bank = "582471", n = 3

    - First digit can be chosen from indices [0 .. 3] -> candidates: 5,8,2,4 → pick '8' at idx=1
    - Second digit can be chosen from indices [2 .. 4] -> candidates: 2,4,7 → pick '7' at idx=4
    - Third digit can be chosen from indices [5 .. 5] -> pick '1'
    Result: "871"

This function applies the above logic to each bank in the input and sums the
resulting chosen numbers as long long.
*/

long long calculateJoltage(std::ifstream& input, int n)
{
    input.clear();
    input.seekg(0);

    long long joltage = 0;
    std::string bank;
    while (input >> bank)
    {
        std::string digits;
        int digit_num = 0, idx = -1;

        // Go digit by digit:
        while (digit_num < n)
        {
            char largest = '0';

            // Start at the next element from previous max digit until the end of the permitted range:
            for (int i = idx + 1; i < bank.length() - (n - 1) + digit_num; ++i)
            {
                if (bank[i] > largest)
                {
                    largest = bank[i];
                    idx = i;
                }
                
                // Early exit, take the leftmost 9:
                if (largest == '9') break;
            }

            digits += largest;
            digit_num++;
        }

        joltage += std::stoll(digits);
    }
    
    return joltage;
}

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);
    int batteriesPartI = 2, batteriesPartII = 12;

    std::cout << "Total output joltage for Part I is " << calculateJoltage(input, batteriesPartI) << "\n";
    std::cout << "Total output joltage for Part II is " << calculateJoltage(input, batteriesPartII) << "\n";

    return EXIT_SUCCESS;
}
