#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

long long isInvalidPartI(const std::string& s)
{
    int n = s.size();
    if (n & 1) return 0;

    for (int i = 0; i < n / 2; ++i)
    {
        if (s[i] != s[i + n / 2]) return 0;
    }
    
    return std::stoll(s);
}

long long isInvalidPartII(const std::string& s)
{
    int n = s.size();

    for (int len = 1; len <= n / 2; ++len)
    {
        if (n % len != 0) continue;

        bool ok = true;
        for (int i = len; i < n; ++i)
        {
            if (s[i] != s[i % len])
            {
                ok = false;
                break;
            }
        }

        if (ok) return std::stoll(s);
    }

    return 0;
}

long long examineRanges(std::ifstream& input, long long (*isInvalid)(const std::string&))
{
    input.clear();
    input.seekg(0);

    long long password = 0;
    std::string range;

    while (std::getline(input, range, ','))
    {
        std::stringstream ss(range);
        long long start, end;
        char dash;

        ss >> start >> dash >> end;

        for (long long x = start; x <= end; ++x)
        {
            std::string s = std::to_string(x);
            password += isInvalid(s);
        }
    }

    return password;
}

int main()
{
    std::ifstream input("input.txt");

    std::cout << "Password for part I is " << examineRanges(input, isInvalidPartI) << "\n";
    std::cout << "Password for part II is " << examineRanges(input, isInvalidPartII) << "\n";

    return EXIT_SUCCESS;
}
