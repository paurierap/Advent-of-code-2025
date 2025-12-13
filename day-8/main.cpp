#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

struct Node3D
{
    int x;
    int y; 
    int z;
};

class KDTree
{
    private:
        std::vector<Node3D> v;

    public:
        KDTree(int size) 
        { 
            v.resize(size);
        }
        ~KDTree() = default;

        void insert(Node3D node)
        {
            if (v.empty()) 
            {
                v.push_back(node);
                return;
            }

            return;  
        };
};

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);
    std::string box;
    std::vector<std::vector<int>> positions(1000);

    while (input >> box)
    {
        std::stringstream ss(box);
        std::vector<int> position(3);
        char c;

        ss >> position[0] >> c >> position[1] >> c >> position[2];
        positions.push_back(position);
    }

    return EXIT_SUCCESS;
}
