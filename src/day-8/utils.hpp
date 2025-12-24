#pragma once

#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Box
{
    int id;
    std::array<int,3> coordinates;
};

struct Connection
{
    long long dist;
    std::pair<int, int> boxes;
};

class DisjointSet 
{
private:
    std::vector<int> parent;
    std::unordered_map<int, int> counts;

public:
    DisjointSet(int size) 
    {
        parent.resize(size);
    
        for (int i = 0; i < size; i++) 
        {
            parent[i] = i;
            counts[i] = 1;
        }
    }

    int find(int i)
    {
        if (parent[i] != i) parent[i] = find(parent[i]);
        return parent[i];
    }


    void unite(int i, int j)
    {
        int irep = find(i); 
        int jrep = find(j);
        
        if (irep == jrep) return;

        parent[irep] = jrep;
        counts[jrep] += counts[irep];
        counts.erase(irep);
    }

    int size() const
    {
        return counts.size();
    }

    std::vector<int> component_sizes() const
    {
        std::vector<int> sizes;
        sizes.reserve(counts.size());

        for (const auto& [_, c] : counts) sizes.push_back(c);

        return sizes;
    }

};

std::vector<Box> getBoxes(std::ifstream& input)
{
    std::string box;
    std::vector<Box> boxes;
    int i = 0;

    while (input >> box)
    {
        std::stringstream ss(box);
        Box b;
        char c;

        b.id = i++;
        ss >> b.coordinates[0] >> c >> b.coordinates[1] >> c >> b.coordinates[2];
        boxes.push_back(b);
    }

    return boxes;
}

long long squareDistance(const Box& b1, const Box& b2)
{
    return 1LL * (b1.coordinates[0] - b2.coordinates[0]) * (b1.coordinates[0] - b2.coordinates[0]) + 
           1LL * (b1.coordinates[1] - b2.coordinates[1]) * (b1.coordinates[1] - b2.coordinates[1]) +
           1LL * (b1.coordinates[2] - b2.coordinates[2]) * (b1.coordinates[2] - b2.coordinates[2]);
}

std::pair<long long, long long> connectBoxes(const std::vector<Box>& boxes, int n)
{
    std::vector<Connection> edges;
    edges.reserve(boxes.size() * (boxes.size() - 1) / 2);
    
    for (int i = 0; i < boxes.size(); ++i)
    {
        for (int j = i + 1; j < boxes.size(); ++j) edges.push_back({squareDistance(boxes[i], boxes[j]), {i,j}});
    }

    std::sort(edges.begin(), edges.end(), [](const Connection& lhs, const Connection& rhs){return lhs.dist < rhs.dist;}); 

    DisjointSet DS(boxes.size());

    int unions = 0;
    long long partI = 0;
    long long partII = 0;

    for (const auto& e : edges)
    {
        int u = e.boxes.first;
        int v = e.boxes.second;

        DS.unite(u, v);
        ++unions;

        // Part I: 
        if (unions == n)
        {
            auto sizes = DS.component_sizes();
            sort(sizes.rbegin(), sizes.rend());
            partI = 1;
            for (int i = 0; i < 3 && i < sizes.size(); ++i)
                partI *= sizes[i];
        }

        // Part II:
        if (DS.size() == 1)
        {
            partII = boxes[u].coordinates[0] * boxes[v].coordinates[0];
            break;  
        }
    }

    return {partI, partII};
}