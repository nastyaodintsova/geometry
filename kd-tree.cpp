#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct Point {
    int x;
    int y;
};

struct Node {
    Point p;
    Node* left;
    Node* right;
};

long long dist2(const Point& a, const Point& b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return dx * dx + dy * dy;
}

Node* build(std::vector<Point>& pts, int l, int r, int depth) {
    if (l >= r) {
        return nullptr;
    }

    int axis = depth % 2;
    int m = (l + r - 1) / 2;

    std::nth_element(pts.begin() + l, pts.begin() + m, pts.begin() + r,
                     [axis](const Point& a, const Point& b) {
                         if (axis == 0) {
                             return a.x < b.x;
                         } else {
                             return a.y < b.y;
                         }
                     });

    Node* node = new Node();
    node->p = pts[m];

    node->left = build(pts, l, m, depth + 1);
    node->right = build(pts, m + 1, r, depth + 1);

    return node;
}

struct Best {
    Point p;
    long long d;
};

void relax(Best best[2], const Point& p, long long d) {
    if (d < best[0].d) {
        best[1] = best[0];
        best[0] = {p, d};
    } else if (d < best[1].d) {
        best[1] = {p, d};
    }
}

void query(Node* node, const Point& q, int depth, Best best[2]) {
    if (node == nullptr) {
        return;
    }

    long long d = dist2(node->p, q);
    relax(best, node->p, d);

    int axis = depth % 2;

    Node* first;
    Node* second;

    long long delta;

    if (axis == 0) {
        if (q.x < node->p.x) {
            first = node->left;
            second = node->right;
        } else {
            first = node->right;
            second = node->left;
        }
        delta = (long long)(q.x - node->p.x) * (q.x - node->p.x);
    } else {
        if (q.y < node->p.y) {
            first = node->left;
            second = node->right;
        } else {
            first = node->right;
            second = node->left;
        }
        delta = (long long)(q.y - node->p.y) * (q.y - node->p.y);
    }

    query(first, q, depth + 1, best);

    if (delta < best[1].d) {
        query(second, q, depth + 1, best);
    }
}

int main() {
    int n;
    std::cin >> n;

    std::vector<Point> pts(n);
    for (int i = 0; i < n; i++) {
        std::cin >> pts[i].x >> pts[i].y;
    }

    Node* root = build(pts, 0, n, 0);

    int q;
    std::cin >> q;

    while (q--) {
        Point queryPoint;
        std::cin >> queryPoint.x >> queryPoint.y;

        Best best[2];
        best[0].d = best[1].d = (long long)4e18;

        query(root, queryPoint, 0, best);

        std::cout << best[0].p.x << " " << best[0].p.y << " " << best[1].p.x << " " << best[1].p.y << "\n";
    }
}