#pragma once
#include <vector>

class CentroidDecomposition {
    using Graph = std::vector<std::vector<int>>;

    size_t calc_subtree_size(int v, int par = -1) {
        subtree_size[v] = 1;
        for (auto to: g[v]) {
            if (to == par || checked[to]) continue;
            subtree_size[v] += calc_subtree_size(to, v);
        }
        return subtree_size[v];
    }

    int find_centroid(int v, size_t mid_size, int par = -1) {
        for (auto to: g[v]) {
            if (to == par || checked[to]) continue;
            if (subtree_size[to] > mid_size) return find_centroid(to, mid_size, v);
        }
        return v; 
    }

    int build_dfs(int v, Graph &tree) {
        int centroid = find_centroid(v, calc_subtree_size(v, -1) / 2, -1);
        checked[centroid] = true;
        for (auto to: g[centroid]) {
            if (checked[to])
                continue;
            tree[centroid].push_back(build_dfs(to, tree));
        }
        return centroid;
    }

public:
    std::vector<bool> checked;
    std::vector<size_t> subtree_size;
    const Graph& g;
    int N;

    CentroidDecomposition(const Graph& g): g(g) {
        N = g.size();
        checked.resize(N);
        subtree_size.resize(N);
        for (int i = 0; i < N; i++) checked[i] = false;
        for (int i = 0; i < N; i++) subtree_size[i] = 0;
    }

    int build(Graph &tree) {
        return build_dfs(0, tree);
    }
};
