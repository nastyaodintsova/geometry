#include <iostream>
#include <vector>
#include <algorithm>

struct Point {
    double x, y;
    Point(double a, double b) : x(a), y(b) {}
};

Point operator+(Point a, Point b) {
    return Point(a.x + b.x, a.y + b.y);
}

Point operator-(Point a, Point b) {
    return Point(a.x - b.x, a.y - b.y);
}

double cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

int First(std::vector<Point>& p) {
    int id = 0;
    for (int i = 1; i < (int)p.size(); ++i) {
        if (p[id].x > p[i].x || (p[id].x == p[i].x && p[id].y > p[i].y)) {
            id = i;
        }
    }
    return id;
}

void Normalize(std::vector<Point>& p) {
    std::reverse(p.begin(), p.end());
    int f = First(p);
    std::rotate(p.begin(), p.begin() + f, p.end());
    p.push_back(p[0]);
    p.push_back(p[1]);
}

std::vector<Point> Min_sum(std::vector<Point>& p1, std::vector<Point>& p2) {
    Normalize(p1);
    Normalize(p2);
    int i = 0, j = 0;
    std::vector<Point> sum;
    while (i < (int)p1.size() - 2 || j < (int)p2.size() - 2) {
        sum.push_back(p1[i] + p2[j]);

        Point v1 = p1[i+1] - p1[i];
        Point v2 = p2[j+1] - p2[j];

        double cr = cross(v1, v2);

        if (cr >= 0 && i < (int)p1.size() - 2) i++;
        if (cr <= 0 && j < (int)p2.size() - 2) j++;
    }
    return sum;
}

bool is_inside(std::vector<Point>& p, Point a) {
    for (int i = 0; i < (int)p.size(); i++) {
        Point b = p[i];
        Point c = p[(i + 1) % p.size()];
        if (cross(c - b, a - b) < -1e-9) {
            return false;
        }
    }
    return true;
}

int main() {
    int n, m;
    std::vector<Point> p1, p2;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        double a, b;
        std::cin >> a >> b;
        p1.push_back(Point(a, b));
    }
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        double a, b;
        std::cin >> a >> b;
        p2.push_back(Point(-a, -b));
    }


    std::vector<Point> sum = Min_sum(p1, p2);

    if (is_inside(sum, Point(0, 0))) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
}