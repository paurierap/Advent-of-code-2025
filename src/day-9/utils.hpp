#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// function to check if point q lies on line segment 'pr'
bool onSegment(const std::vector<int>& q, const std::vector<int>& p, const std::vector<int>& r) {
    return (q[0] <= std::max(p[0], r[0]) && 
            q[0] >= std::min(p[0], r[0]) &&
            q[1] <= std::max(p[1], r[1]) && 
            q[1] >= std::min(p[1], r[1]));
}

bool isPointEnclosed(const std::vector<int>& p, const std::vector<std::vector<int>>& polygon)
{
    int n = polygon.size();
    int isEnclosed = false;
    int px = p[0], py = p[1];

    for (int i = 0; i < n; ++i)
    {
        int j = (i + 1) % n;
        int x1 = polygon[i][0], y1 = polygon[i][1];
        int x2 = polygon[j][0], y2 = polygon[j][1];
        
        if (onSegment(p, polygon[i], polygon[j])) return true;

        if ((y1 > py) != (y2 > py))
        {
            int slope = 1LL * (px - x1) * (y2 - y1) - 1LL * (x2 - x1) * (py - y1);

            if (slope == 0) return true;
            if ((slope < 0) != (y2 < y1)) isEnclosed = !isEnclosed;
        }
    }

    return isEnclosed;
}

bool isRectangleEnclosed(const std::vector<int>& p1, const std::vector<int>& p3, const std::vector<std::vector<int>>& polygon)
{
    // The potential remaining corners:
    std::vector<int> p2{p1[0], p3[1]}, p4{p3[0], p1[1]};

    // First condition: ensure the other corners are contained within the polygon
    if (!isPointEnclosed(p2, polygon) || !isPointEnclosed(p4, polygon)) return false;

    std::vector<std::vector<int>> rect{p1,p2,p3,p4};

    // Second condition: ensure all cells of the edges of the rectangle are contained within the polygon
    for (int i = 0; i < 4; ++i)
    {
        int j = (i + 1) % 4;
        if (rect[i][1] == rect[j][1])
        {
            int xdir = (rect[i][0] < rect[j][0]) ? 1 : -1;
            for (int k = rect[i][0] + xdir; k != rect[j][0]; k += xdir)
            {
                if (!isPointEnclosed({k, rect[i][1]},polygon)) return false;
            }
        }
        else
        {
            int ydir = (rect[i][1] < rect[j][1]) ? 1 : -1;
            for (int k = rect[i][1] + ydir; k != rect[j][1]; k += ydir)
            {
                if (!isPointEnclosed({rect[i][0], k},polygon)) return false;
            }
        }
    }

    return true;
}

long long calculateArea(const std::vector<int>& a, const std::vector<int>& b)
{
    return 1LL * (abs(a[0] - b[0]) + 1) * (abs(a[1]- b[1]) + 1);
}

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