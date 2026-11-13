#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <array>

struct Point {
    long long x, y, z;
};

struct Face {
    int a, b, c;
    bool alive;
    Face() : a(-1), b(-1), c(-1), alive(true) {}
    Face(int x, int y, int z) : a(x), b(y), c(z), alive(true) {}
};

Point operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Point cross(const Point& a, const Point& b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

long long dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

unsigned long long pack_edge(int u, int v) {
    return ((unsigned long long)(unsigned int)u << 32) | (unsigned int)v;
}

Point face_normal(const std::vector<Point>& v, const std::vector<Face>& faces, int fi) {
    const Face& f = faces[fi];
    return cross(v[f.b] - v[f.a], v[f.c] - v[f.a]);
}

bool is_visible(const std::vector<Point>& v, const std::vector<Face>& faces, int fi, int p) {
    const Face& f = faces[fi];
    Point n = face_normal(v, faces, fi);
    return dot(n, v[p] - v[f.a]) > 0;
}

int add_face(std::vector<Face>& faces, std::unordered_map<unsigned long long, int>& edge2face, int a, int b, int c) {
    int id = faces.size();
    faces.emplace_back(a, b, c);
    edge2face[pack_edge(a, b)] = id;
    edge2face[pack_edge(b, c)] = id;
    edge2face[pack_edge(c, a)] = id;
    return id;
}

int main() {
    int T;
    std::cin >> T;

    while (T--) {
        int n;
        std::cin >> n;

        std::vector<Point> vertices(n);
        for (int i = 0; i < n; i++) {
            std::cin >> vertices[i].x >> vertices[i].y >> vertices[i].z;
        }

        std::vector<Face> faces;
        faces.reserve(n * 2 + 10);

        std::unordered_map<unsigned long long, int> edge2face;
        edge2face.reserve(n * 6 + 10);
        int temp = add_face(faces, edge2face, 0, 1, 2);

        if (is_visible(vertices, faces, temp, 3)) {
            faces[temp].alive = false;
            add_face(faces, edge2face, 0, 2, 1);
            add_face(faces, edge2face, 0, 3, 2);
            add_face(faces, edge2face, 0, 1, 3);
            add_face(faces, edge2face, 1, 2, 3);
        } else {
            faces[temp].alive = false;
            add_face(faces, edge2face, 0, 1, 2);
            add_face(faces, edge2face, 0, 2, 3);
            add_face(faces, edge2face, 0, 3, 1);
            add_face(faces, edge2face, 1, 3, 2);
        }

        for (int p = 4; p < n; p++) {
            std::vector<int> visible;

            for (int i = 0; i < faces.size(); i++) {
                if (faces[i].alive && is_visible(vertices, faces, i, p)) {
                    visible.push_back(i);
                }
            }

            if (visible.empty()) {
                continue;
            }

            std::vector<std::pair<int,int>> horizon;

            for (int fi : visible) {
                Face& f = faces[fi];

                int e[3][2] = {{f.a,f.b},{f.b,f.c},{f.c,f.a}};

                for (int i = 0; i < 3; i++) {
                    int u = e[i][0];
                    int v = e[i][1];
                    auto it = edge2face.find(pack_edge(v,u));
                    if (it == edge2face.end()) {
                        horizon.push_back({u,v});
                    } else {
                        int twin = it->second;
                        if (!faces[twin].alive ||
                            !is_visible(vertices, faces, twin, p)) {
                            horizon.push_back({u,v});
                        }
                    }
                }
            }

            for (int fi : visible) {
                faces[fi].alive = false;
            }
            for (auto& e : horizon) {
                add_face(faces, edge2face, e.first, e.second, p);
            }
        }

        std::vector<std::array<int,3>> ans;
        for (const Face& f : faces) {
            if (!f.alive) {
                continue;
            }
            int a = f.a;
            int b = f.b;
            int c = f.c;
            int mn = std::min({a, b, c});

            if (a == mn) {
                ans.push_back({a, b, c});
            } else if (b == mn) {
                ans.push_back({b, c, a});
            } else {
                ans.push_back({c, a, b});
            }
        }
        std::sort(ans.begin(), ans.end());
        std::cout << ans.size() << "\n";
        for (const auto& t : ans) {
            std::cout << "3 " << t[0] << " " << t[1] << " " << t[2] << "\n";
        }
    }
}