#include <iostream>
#include <vector>
#include <algorithm>

struct Rect {
    int x1;
    int y1;
    int x2;
    int y2;
    int id;
};

struct Node {
    int x1;
    int y1;
    int x2;
    int y2;

    bool leaf;

    std::vector<Node*> children;
    std::vector<int> rects;

    Node() {
        x1 = 1000000000;
        y1 = 1000000000;
        x2 = -1000000000;
        y2 = -1000000000;
        leaf = false;
    }
};

std::vector<Rect> rectangles;

bool pointInRect(int x, int y, int x1, int y1, int x2, int y2) {
    return x >= x1 && x <= x2 && y >= y1 && y <= y2;
}

Node* build(std::vector<int>& ids) {
    Node* node = new Node();

    if (ids.size() <= 4) {
        node->leaf = true;

        for (int id : ids) {
            node->rects.push_back(id);

            const Rect& r = rectangles[id];

            node->x1 = std::min(node->x1, r.x1);
            node->y1 = std::min(node->y1, r.y1);
            node->x2 = std::max(node->x2, r.x2);
            node->y2 = std::max(node->y2, r.y2);
        }

        return node;
    }

    std::sort(ids.begin(),ids.end(),[](int a, int b) {
                  long long ca =(long long)rectangles[a].x1 +(long long)rectangles[a].x2;
                  long long cb =(long long)rectangles[b].x1 +(long long)rectangles[b].x2;

                  return ca < cb;
              }
    );

    int n = ids.size();

    for (int part = 0; part < 4; part++) {
        int l = (long long)part * n / 4;
        int r = (long long)(part + 1) * n / 4;

        if (l >= r) {
            continue;
        }

        std::vector<int> sub(ids.begin() + l, ids.begin() + r);

        Node* child = build(sub);

        node->children.push_back(child);

        node->x1 = std::min(node->x1, child->x1);
        node->y1 = std::min(node->y1, child->y1);
        node->x2 = std::max(node->x2, child->x2);
        node->y2 = std::max(node->y2, child->y2);
    }

    return node;
}

void query(
        Node* node,
        int x,
        int y,
        std::vector<int>& answer
) {
    if (!pointInRect(x, y, node->x1, node->y1, node->x2, node->y2)) {
        return;
    }

    if (node->leaf) {
        for (int id : node->rects) {
            const Rect& r = rectangles[id];

            if (pointInRect(x, y, r.x1, r.y1, r.x2, r.y2)) {
                answer.push_back(r.id);
            }
        }

        return;
    }

    for (Node* child : node->children) {
        query(child, x, y, answer);
    }
}

int main() {
    int n;
    std::cin >> n;

    rectangles.resize(n);

    for (int i = 0; i < n; i++) {
        std::cin >> rectangles[i].x1 >> rectangles[i].y1 >> rectangles[i].x2 >> rectangles[i].y2;
        rectangles[i].id = i + 1;
    }

    std::vector<int> ids(n);

    for (int i = 0; i < n; i++) {
        ids[i] = i;
    }

    Node* root = build(ids);

    int q;
    std::cin >> q;

    while (q--) {
        int x;
        int y;

        std::cin >> x >> y;
        std::vector<int> answer;
        query(root, x, y, answer);
        std::sort(answer.begin(), answer.end());
        std::cout << answer.size();
        for (int v : answer) {
            std::cout << ' ' << v;
        }

        std::cout << '\n';
    }
}