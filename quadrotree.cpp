#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <algorithm>

struct Creep {
    long double x;
    long double y;
    long double hp;
};

struct Node {
    long double lx;
    long double rx;
    long double ly;
    long double ry;

    int depth;

    Node* child[4];

    std::vector<int> ids[2];

    Node(long double _lx,
         long double _rx,
         long double _ly,
         long double _ry,
         int _depth)
            : lx(_lx),
              rx(_rx),
              ly(_ly),
              ry(_ry),
              depth(_depth) {

        for (int i = 0; i < 4; ++i) {
            child[i] = nullptr;
        }
    }

    bool isLeaf() const {
        return depth == 9;
    }
};

std::vector<Creep> creeps[2];

int getChild(Node* v, long double x, long double y) {
    long double mx = (v->lx + v->rx) / 2.0L;
    long double my = (v->ly + v->ry) / 2.0L;

    int id = 0;

    if (x >= mx) {
        id += 1;
    }

    if (y >= my) {
        id += 2;
    }

    return id;
}

Node* build(long double lx,
            long double rx,
            long double ly,
            long double ry,
            int depth) {

    Node* v = new Node(lx, rx, ly, ry, depth);

    if (depth == 9) {
        return v;
    }

    long double mx = (lx + rx) / 2.0L;
    long double my = (ly + ry) / 2.0L;

    v->child[0] = build(lx, mx, ly, my, depth + 1);
    v->child[1] = build(mx, rx, ly, my, depth + 1);
    v->child[2] = build(lx, mx, my, ry, depth + 1);
    v->child[3] = build(mx, rx, my, ry, depth + 1);

    return v;
}

Node* getLeaf(Node* root, long double x, long double y) {
    Node* v = root;

    while (!v->isLeaf()) {
        int to = getChild(v, x, y);
        v = v->child[to];
    }

    return v;
}

void insertCreep(Node* root, int side, int id) {
    Node* leaf = getLeaf(root,
                         creeps[side][id].x,
                         creeps[side][id].y);

    leaf->ids[side].push_back(id);
}

void eraseCreep(Node* root, int side, int id) {
    Node* leaf = getLeaf(root,
                         creeps[side][id].x,
                         creeps[side][id].y);

    std::vector<int>& vec = leaf->ids[side];

    for (std::size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == id) {
            vec[i] = vec.back();
            vec.pop_back();
            return;
        }
    }
}

void moveCreep(Node* root,
               int side,
               int id,
               long double vx,
               long double vy) {

    eraseCreep(root, side, id);

    creeps[side][id].x += vx;
    creeps[side][id].y += vy;

    insertCreep(root, side, id);
}

bool intersects(Node* v,
                long double x,
                long double y,
                long double r) {

    long double dx = 0.0L;

    if (x < v->lx) {
        dx = v->lx - x;
    } else if (x > v->rx) {
        dx = x - v->rx;
    }

    long double dy = 0.0L;

    if (y < v->ly) {
        dy = v->ly - y;
    } else if (y > v->ry) {
        dy = y - v->ry;
    }

    return dx * dx + dy * dy < r * r;
}

void collect(Node* v,
             int enemy,
             long double x,
             long double y,
             std::vector<int>& result) {

    if (!intersects(v, x, y, 2.0L)) {
        return;
    }

    if (v->isLeaf()) {
        for (int id : v->ids[enemy]) {
            result.push_back(id);
        }
        return;
    }

    for (int i = 0; i < 4; ++i) {
        collect(v->child[i], enemy, x, y, result);
    }
}

long double attack(Node* root, int side, int id) {
    int enemy = side ^ 1;

    Creep& a = creeps[side][id];

    if (a.hp <= 0.0L) {
        return 0.0L;
    }

    std::vector<int> candidates;

    collect(root,
            enemy,
            a.x,
            a.y,
            candidates);

    long double total = 0.0L;

    for (int eid : candidates) {
        Creep& b = creeps[enemy][eid];

        if (b.hp <= 0.0L) {
            continue;
        }

        long double dx = a.x - b.x;
        long double dy = a.y - b.y;

        long double d2 = dx * dx + dy * dy;

        if (d2 >= 4.0L) {
            continue;
        }

        long double d = std::sqrt(d2);

        long double coef = std::min(1.0L, 2.0L - d);

        long double dmg =
                std::min(b.hp,
                         a.hp * coef / 10.0L);

        b.hp -= dmg;

        if (b.hp < 0.0L) {
            b.hp = 0.0L;
        }

        total += dmg;
    }

    return total;
}

int main() {
    Node* root = build(0.0L,
                       1024.0L,
                       0.0L,
                       1024.0L,
                       0);

    int n;
    std::cin >> n;

    creeps[0].resize(n);
    creeps[1].resize(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> creeps[0][i].x
                 >> creeps[0][i].y;

        creeps[0][i].hp = 10.0L;

        insertCreep(root, 0, i);
    }

    for (int i = 0; i < n; ++i) {
        std::cin >> creeps[1][i].x
                 >> creeps[1][i].y;

        creeps[1][i].hp = 10.0L;

        insertCreep(root, 1, i);
    }

    int m;
    std::cin >> m;

    std::cout << std::fixed
              << std::setprecision(10);

    for (int t = 0; t < m; ++t) {
        long double lightDamage = 0.0L;
        long double darkDamage = 0.0L;

        for (int i = 0; i < n; ++i) {
            std::string type;
            std::cin >> type;

            if (type == "m") {
                long double vx;
                long double vy;

                std::cin >> vx >> vy;

                moveCreep(root,
                          0,
                          i,
                          vx,
                          vy);
            } else {
                lightDamage += attack(root,
                                      0,
                                      i);
            }
        }

        for (int i = 0; i < n; ++i) {
            std::string type;
            std::cin >> type;

            if (type == "m") {
                long double vx;
                long double vy;

                std::cin >> vx >> vy;

                moveCreep(root,
                          1,
                          i,
                          vx,
                          vy);
            } else {
                darkDamage += attack(root,
                                     1,
                                     i);
            }
        }

        std::cout << lightDamage << '\n';
        std::cout << darkDamage << '\n';
    }
}
