#pragma once
#include <algorithm>
#include "UnionFind.hpp"

struct Edge {
    long long cost;
    int u, v;
};

// First element of a pair represents whether a spanning tree exists.
std::pair<bool, long long> kruscal(std::vector<Edge> &edges, int n) {
    auto edge_compare = [](const Edge &left, const Edge &right) {
        return left.cost < right.cost;
    };

    std::sort(edges.begin(), edges.end(), edge_compare);
    UnionFind uf(n);
    long long result_cost = 0;
    for (auto &e : edges) {
        if (!uf.issame(e.u, e.v)) {
            uf.unite(e.u, e.v);
            result_cost += e.cost;
        }
    }

    if (uf.connected_components_cnt != 1)
        return std::make_pair(false, 0);

    return std::make_pair(true, result_cost);
}