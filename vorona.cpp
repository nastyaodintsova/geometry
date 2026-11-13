#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

const long double EPS = 1e-12L;

struct Point {
    long double x, y;
    Point(long double x = 0, long double y = 0) : x(x), y(y) {}
    Point operator + (const Point& o) const { return {x + o.x, y + o.y}; }
    Point operator - (const Point& o) const { return {x - o.x, y - o.y}; }
    Point operator * (long double k) const { return {x * k, y * k}; }
};

long double orient(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

long double dist2(const Point& a, const Point& b) {
    long double dx = a.x - b.x;
    long double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

long double area(const std::vector<Point>& poly) {
    int m = poly.size();
    if (m < 3) {
        return 0.0L;
    }
    long double s = 0;
    for (int i = 0; i < m; ++i) {
        int j = (i + 1) % m;
        s += poly[i].x * poly[j].y - poly[i].y * poly[j].x;
    }
    return fabsl(s) / 2.0L;
}

bool inside(const Point& p, const Point& a, const Point& b) {
    return dist2(p, a) <= dist2(p, b) + EPS;
}

Point intersect(const Point& s, const Point& e, const Point& a, const Point& b) {
    long double fs = dist2(s, a) - dist2(s, b);
    long double fe = dist2(e, a) - dist2(e, b);
    long double t = fs / (fs - fe);
    return s + (e - s) * t;
}

std::vector<Point> clip(const std::vector<Point>& poly, const Point& a, const Point& b) {
    std::vector<Point> res;
    int m = (int)poly.size();
    if (m == 0) {
        return res;
    }

    for (int i = 0; i < m; ++i) {
        Point cur = poly[i];
        Point nxt = poly[(i + 1) % m];

        bool in_cur = inside(cur, a, b);
        bool in_nxt = inside(nxt, a, b);

        if (in_cur && in_nxt) {
            res.push_back(nxt);
        } else if (in_cur && !in_nxt) {
            res.push_back(intersect(cur, nxt, a, b));
        } else if (!in_cur && in_nxt) {
            res.push_back(intersect(cur, nxt, a, b));
            res.push_back(nxt);
        }
    }

    return res;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    long double w, h;
    std::cin >> n >> w >> h;

    std::vector<Point> p(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> p[i].x >> p[i].y;
    }

    std::cout << std::fixed << std::setprecision(10);

    for (int i = 0; i < n; ++i) {
        std::vector<Point> poly = {{0, 0}, {w, 0}, {w, h}, {0, h}};
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                continue;
            }
            poly = clip(poly, p[i], p[j]);
            if (poly.empty()) {
                break;
            }
        }
        std::cout <<std::fixed << std::setprecision(6) << area(poly) << '\n';
    }
}
