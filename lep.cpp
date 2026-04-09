#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <random>

struct Point {
    long double x, y;
};

struct Triangle {
    int a, b, c;
};

long double orient(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

long double incircle(const Point& a, const Point& b, const Point& c, const Point& p) {
    long double ax = a.x - p.x, ay = a.y - p.y;
    long double bx = b.x - p.x, by = b.y - p.y;
    long double cx = c.x - p.x, cy = c.y - p.y;
    long double a2 = ax * ax + ay * ay;
    long double b2 = bx * bx + by * by;
    long double c2 = cx * cx + cy * cy;
    return a2 * (bx * cy - by * cx) - ax * (b2 * cy - by * c2) + ay * (b2 * cx - bx * c2);
}

std::vector<Triangle> triangulation(std::vector<Point>& points, int n) {
    long double M = 1e9;
    points[n] = {-M, -M};
    points[n + 1] = { M, -M};
    points[n + 2] = { 0, M};

    std::vector<int> order(n);
    for (int i = 0; i < n; ++i) {
        order[i] = i;
    }

    std::mt19937 rng(712367);
    std::shuffle(order.begin(), order.end(), rng);

    std::vector<Triangle> tr;
    std::vector<char> alive;

    tr.push_back({n, n + 1, n + 2});
    alive.push_back(1);

    for (int idx : order) {
        std::vector<int> bad;

        for (int t = 0; t < (int)tr.size(); ++t) {
            if (!alive[t]) {
                continue;
            }

            int a = tr[t].a;
            int b = tr[t].b;
            int c = tr[t].c;

            if (orient(points[a], points[b], points[c]) < 0) {
                std::swap(b, c);
            }

            if (incircle(points[a], points[b], points[c], points[idx]) > 0) {
                bad.push_back(t);
            }
        }

        std::map<std::pair<int, int>, int> cnt;
        std::map<std::pair<int, int>, std::pair<int, int>> rep;

        for (int id : bad) {
            alive[id] = 0;
            int a = tr[id].a;
            int b = tr[id].b;
            int c = tr[id].c;

            std::vector<std::pair<int, int>> edges = {{a, b}, {b, c}, {c, a}};

            for (auto e : edges) {
                int u = e.first;
                int v = e.second;
                std::pair<int, int> key = (u < v) ? std::make_pair(u, v) : std::make_pair(v, u);
                cnt[key]++;
                rep[key] = {u, v};
            }
        }

        for (auto& it : cnt) {
            if (it.second != 1) {
                continue;
            }

            int u = rep[it.first].first;
            int v = rep[it.first].second;

            if (orient(points[u], points[v], points[idx]) > 0) {
                tr.push_back({u, v, idx});
            } else {
                tr.push_back({v, u, idx});
            }
            alive.push_back(1);
        }
    }

    std::vector<Triangle> res;
    for (int t = 0; t < (int)tr.size(); ++t) {
        if (alive[t]) {
            res.push_back(tr[t]);
        }
    }
    return res;
}

std::vector<std::pair<int, int>> filter(const std::vector<Point>& points, const std::vector<Triangle>& tr, int n) {
    std::vector<std::pair<int, int>> edges;

    for (const auto& t : tr) {
        int a = t.a;
        int b = t.b;
        int c = t.c;

        if (a < n && b < n) {
            edges.push_back({a, b});
        }
        if (b < n && c < n) {
            edges.push_back({b, c});
        }
        if (c < n && a < n) {
            edges.push_back({c, a});
        }
    }

    for (auto& e : edges) {
        if (e.first > e.second) {
            std::swap(e.first, e.second);
        }
    }

    std::sort(edges.begin(), edges.end());
    edges.erase(std::unique(edges.begin(), edges.end()), edges.end());

    std::vector<std::pair<int, int>> ans;

    for (auto [u, v] : edges) {
        bool ok = true;

        for (int w = 0; w < n; ++w) {
            if (w == u || w == v) {
                continue;
            }

            long double dot = (points[u].x - points[w].x) * (points[v].x - points[w].x) + (points[u].y - points[w].y) * (points[v].y - points[w].y);
            if (dot <= 0) {
                ok = false;
                break;
            }
        }

        if (ok) {
            ans.push_back({u + 1, v + 1});
        }
    }
    return ans;
}

int main() {
    int n;
    std::cin >> n;

    std::vector<Point> points(n + 3);
    for (int i = 0; i < n; ++i) {
        std::cin >> points[i].x >> points[i].y;
    }

    auto tr = triangulation(points, n);
    auto ans = filter(points, tr, n);

    std::cout << ans.size() << '\n';
    for (auto [u, v] : ans) {
        std::cout << u << ' ' << v << '\n';
    }
}