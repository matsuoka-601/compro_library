#pragma once
#include <assert.h>
#include <algorithm>
#include <vector>

// verify: 
// https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=6424837

template<class T> class Point {
public:
    int id;
    T x, y;
    Point() {}
    Point(int id, T x, T y): id(id), x(x), y(y) {}
};

template<class T> class KDTree {
    class Node {
    public:
        Node *l, *r;
        int location;
        Node() {}
    };
    

    // [l, r)
    Node* build(int l, int r, int depth) {
        if (l >= r) return nullptr;
        int mid = (l + r) / 2;

        if (depth % 2 == 0) std::sort(points.begin() + l, points.begin() + r, 
            [](auto const &p1, auto const &p2) { return p1.x < p2.x; });
        else std::sort(points.begin() + l, points.begin() + r, 
            [](auto const &p1, auto const &p2) { return p1.y < p2.y; });

        Node* node = (Node *) malloc(sizeof(Node));
        node->location = mid;
        node->l = build(l, mid, depth + 1);
        node->r = build(mid + 1, r, depth + 1);
        return node;
    }

    void find(Node* v, int sx, int tx, int sy, int ty, int depth, std::vector< Point<T> > &result) {
        int loc = v->location;
        T x = points[loc].x;
        T y = points[loc].y;

        if (sx <= x && x <= tx && sy <= y && y <= ty) result.push_back(points[loc]);

        if (depth % 2 == 0) {
            if (v->l != nullptr && sx <= x) find(v->l, sx, tx, sy, ty, depth + 1, result); 
            if (v->r != nullptr && x <= tx) find(v->r, sx, tx, sy, ty, depth + 1, result); 
        } else {
            if (v->l != nullptr && sy <= y) find(v->l, sx, tx, sy, ty, depth + 1, result);
            if (v->r != nullptr && y <= ty) find(v->r, sx, tx, sy, ty, depth + 1, result);
        }
    }

    std::vector< Point<T> > points;
    int n;
    Node* root;

public:
    KDTree(std::vector< Point<T> > &points): points(points), n((int)points.size()), root(nullptr) {
        root = build(0, n, 0);
    }

    // find the closest point to q and return the id of the point
    Point<T> find(Point<T> &q) {
        return NULL; // to implement
    }

    // find points in [sx, tx] x [sy, ty] and append these points to result
    void find(int sx, int tx, int sy, int ty, std::vector< Point<T> > &result) {
        assert(sx <= tx);
        assert(sy <= ty);
        assert(root);
        find(root, sx, tx, sy, ty, 0, result);
    }
};