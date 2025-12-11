#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

static std::unordered_map<int, std::vector<std::pair<int,int>>> horizontalInside;
static std::unordered_map<int, std::vector<std::pair<int,int>>> verticalInside;

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

std::vector<std::pair<int,int>> computeHorizontalInside(int y, const std::vector<std::vector<int>>& poly)
{
    std::vector<int> xs; 
    int n = poly.size();

    // collect all x where the horizontal ray intersects polygon edges
    for (int i = 0; i < n; ++i)
    {
        int j = (i + 1) % n;
        int y1 = poly[i][1], y2 = poly[j][1];
        int x1 = poly[i][0], x2 = poly[j][0];

        // horizontal edges don't contribute to crossing count 
        if (y1 == y2) continue;

        // check if horizontal ray at y intersects segment (y between y1 and y2)
        bool intersects = ((y1 > y) != (y2 > y));
        if (!intersects) continue;

        // find x of intersection
        int x_intersect;

        if (x1 == x2) x_intersect = x1;
        else
        {
            long dy = y2 - y1;
            long dx = x2 - x1;
            long t_num = (long)(y - y1);

            // integer-safe because dy = ±1 in orthogonal pixel boundary
            x_intersect = x1 + (dx * t_num) / dy;
        }

        xs.push_back(x_intersect);
    }

    // sort intersection x-coordinates
    std::sort(xs.begin(), xs.end());

    // pair them into interior intervals: (x0,x1), (x2,x3), ...
    std::vector<std::pair<int,int>> intervals;
    for (int i = 0; i + 1 < (int)xs.size(); i += 2) intervals.emplace_back(xs[i], xs[i+1]);

    return intervals;
}

std::vector<std::pair<int,int>> computeVerticalInside(int x, const std::vector<std::vector<int>>& poly)
{
    std::vector<int> ys; 
    int n = poly.size();

    for (int i = 0; i < n; ++i)
    {
        int j = (i + 1) % n;
        int x1 = poly[i][0], x2 = poly[j][0];
        int y1 = poly[i][1], y2 = poly[j][1];

        if (x1 == x2) continue;

        bool intersects = ((x1 > x) != (x2 > x));
        if (!intersects) continue;

        int y_intersect;

        if (y1 == y2) y_intersect = y1;
        else
        {
            long dx = x2 - x1;
            long dy = y2 - y1;
            long t_num = (long)(x - x1);
            y_intersect = y1 + (dy * t_num) / dx;
        }

        ys.push_back(y_intersect);
    }

    std::sort(ys.begin(), ys.end());

    std::vector<std::pair<int,int>> intervals;
    for (int i = 0; i + 1 < (int)ys.size(); i += 2) intervals.emplace_back(ys[i], ys[i+1]);

    return intervals;
}

bool isFullyInside(const std::vector<std::pair<int,int>>& iv, int a, int b)
{
    if (a > b) std::swap(a,b);

    for (auto& p : iv)
    {
        if (p.first <= a && p.second >= b) return true;
    }

    return false;
}

const std::vector<std::pair<int,int>>& getHorizontal(int y, const std::vector<std::vector<int>>& poly)
{
    auto it = horizontalInside.find(y);
    if (it != horizontalInside.end()) return it->second;

    auto iv = computeHorizontalInside(y, poly);
    horizontalInside[y] = std::move(iv);
    return horizontalInside[y];
}

const std::vector<std::pair<int,int>>& getVertical(int x, const std::vector<std::vector<int>>& poly)
{
    auto it = verticalInside.find(x);
    if (it != verticalInside.end()) return it->second;

    auto iv = computeVerticalInside(x, poly);
    verticalInside[x] = std::move(iv);
    return verticalInside[x];
}

bool edgeInside(const std::vector<int>& p1,
                const std::vector<int>& p2,
                const std::vector<std::vector<int>>& polygon)
{
    if (p1[1] == p2[1])
    {
        // horizontal edge
        int y = p1[1];
        auto& iv = getHorizontal(y, polygon);
        return isFullyInside(iv, p1[0], p2[0]);
    }
    else
    {
        // vertical edge
        int x = p1[0];
        auto& iv = getVertical(x, polygon);
        return isFullyInside(iv, p1[1], p2[1]);
    }
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
        
        /*
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
        */
        if (!edgeInside(rect[i], rect[j], polygon)) return false;

    }

    return true;
}

long long calculateArea(const std::vector<int>& a, const std::vector<int>& b)
{
    return 1LL * (abs(a[0] - b[0]) + 1) * (abs(a[1]- b[1]) + 1);
}