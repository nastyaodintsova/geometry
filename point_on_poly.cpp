#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <climits>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <iomanip>

struct Point {
    double x_;
    double y_;
    Point(double x, double y) : x_(x), y_(y) {}
};

double prod(Point a1, Point a2) {
    return (a1.x_ * a2.y_ - a2.x_ * a1.y_);
}

bool point_on_segm(Point a1, Point a2, Point k) {
    return (k.x_ >= std::min(a1.x_, a2.x_) - 1e-9) &&
           (k.x_ <= std::max(a1.x_, a2.x_) + 1e-9) &&
           (k.y_ >= std::min(a1.y_, a2.y_) - 1e-9) &&
           (k.y_ <= std::max(a1.y_, a2.y_) + 1e-9);
}

bool point_in_polygon(const std::vector<Point>& new_points) {
    int n = new_points.size();
    Point a(0.0, 0.0);

    for (int i = 0; i < n; ++i) {
        if (prod(new_points[i], new_points[(i + 1) % n]) == 0.0 &&
            point_on_segm(new_points[i], new_points[(i + 1) % n], a)) {
            return true;
        }
    }

    bool flag = false;
    for (int i = 0; i < n; ++i) {
        double y1 = new_points[i].y_;
        double y2 = new_points[(i + 1) % n].y_;

        if ((y1 > 0.0) != (y2 > 0.0)) {
            if (new_points[i].x_ +
                (new_points[(i + 1) % n].x_ - new_points[i].x_) *
                (-y1) / (y2 - y1) > 0.0) {
                flag = !flag;
            }
        }
    }

    return flag;
}

int main() {
    int n;
    double x, y;
    std::cin >> n >> x >> y;
    std::vector<Point> new_points;
    for (int i = 0; i < n; ++i) {
        double k, t;
        std::cin >> k >> t;
        new_points.push_back(Point(k - x, t - y));
    }

    if (point_in_polygon(new_points)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
}