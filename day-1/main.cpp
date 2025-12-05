#include <fstream>
#include <iostream>
#include <string>

static const int MOD = 100;

int wrap(int x) 
{
    return (x % MOD + MOD) % MOD;
}

int zeroes_after_rotation(std::ifstream& input)
{
    input.clear();
    input.seekg(0);

    int dial = 50;
    int password = 0;
    char dir;
    int rot;

    while (input >> dir >> rot) 
    {
        rot %= MOD;
        dial = wrap(dir == 'L' ? dial - rot : dial + rot);

        if (dial == 0) password++;
    }

    return password;
}

int zeroes_during_rotation(std::ifstream& input)
{
    input.clear();
    input.seekg(0);

    int dial = 50;
    int password = 0;
    char dir;
    int rot;

    while (input >> dir >> rot) 
    {
        password += rot / MOD;
        rot %= MOD;

        if (dir == 'L') 
        {
            if (dial < rot) password++;
            dial = wrap(dial - rot);
        } 
        else 
        {
            if (dial + rot >= MOD) password++;
            dial = wrap(dial + rot);
        }
    }

    return password;
}

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);

    int pass1 = zeroes_after_rotation(input);
    std::cout << "Password for task 1 is " << pass1 << "\n";

    int pass2 = zeroes_during_rotation(input);
    std::cout << "Password for task 2 is " << pass2 << "\n";

    return EXIT_SUCCESS;
}
