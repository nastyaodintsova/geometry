#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

const double EPS = 1e-9;

struct Point {
    int64_t x, y;
};

Point operator-(const Point& a, const Point& b) {
    return Point(b.x - a.x, b.y - a.y);
}

int64_t Cross(const Point& a, const Point& b, const Point& c) {
    Point ba = b - a;
    Point ca = c - a;
    return ba.x * ca.y - ba.y * ca.x;
}

int64_t SquareDist(const Point& a, const Point& b) {
    Point ba = b - a;
    return ba.x * ba.x + ba.y * ba.y;
}

std::vector<Point> ConvexHull(std::vector<Point>& points) {
    std::sort(points.begin(), points.end(), [&](const Point& a, const Point& b) {
        if (a.x != b.x) {
            return a.x < b.x;
        }
        return a.y < b.y;
    });

    points.erase(std::unique(points.begin(), points.end(),
                             [](const Point& a, const Point& b) {
                                 return a.x == b.x && a.y == b.y;
                             }),points.end());
    int min_ind = 0;
    for (int i = 1; i < points.size(); ++i) {
        if (points[i].y < points[min_ind].y || (points[i].y == points[min_ind].y && points[i].x < points[min_ind].x)) {
            min_ind = i;
        }
    }

    std::swap(points[0], points[min_ind]);
    Point start = points[0];
    std::sort(points.begin() + 1, points.end(),
              [start](const Point& a, const Point& b) {
                  int64_t cross = Cross(start, a, b);
                  if (cross == 0) {
                      return SquareDist(start, a) <
                             SquareDist(start, b);
                  }
                  return cross > 0;
              });
    std::vector<Point> hull;

    for (const Point& point : points) {
        while (hull.size() >= 2) {
            const Point& last = hull[hull.size() - 1];
            const Point& second_last = hull[hull.size() - 2];
            if (Cross(second_last, last, point) <= 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.push_back(point);
    }

    return hull;
}

double CountPerimeter(const std::vector<Point>& hull) {
    double perimeter = 0.0;
    int n = hull.size();

    for (int i = 0; i < n; ++i) {
        const Point& curr = hull[i];
        const Point& next = hull[(i + 1) % n];
        perimeter += std::sqrt(SquareDist(next, curr));
    }

    return perimeter;
}

int64_t Diameter(const std::vector<Point>& hull) {
    int n = hull.size();
    if (n == 1) {
        return 0;
    }
    if (n == 2) {
        return SquareDist(hull[0], hull[1]);
    }
    int64_t ans = 0;
    int j = 1;
    for (int i = 0; i < n; ++i) {
        int i1 = (i + 1) % n;
        while (true) {
            int j2 = (j + 1) % n;
            int64_t a1 = std::abs(Cross(hull[i], hull[i1], hull[j2]));
            int64_t a2 = std::abs(Cross(hull[i], hull[i1], hull[j]));

            if (a1 > a2) {
                j = j2;
            }
            else {
                break;
            }
        }
        ans = std::max(ans, SquareDist(hull[i], hull[j]));
    }
    return ans;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<Point> points(n);
    for (int i = 0; i < n; ++i) {
        double x,y;
        std::cin >> x >> y;
        points[i] = Point(x, y);
    }

    std::vector<Point> hull = ConvexHull(points);
    std::cout << Diameter(hull);
}
