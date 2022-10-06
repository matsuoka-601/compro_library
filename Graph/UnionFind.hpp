#pragma once
#include <vector>

class UnionFind {
    std::vector<int> par, rank, siz;
public:
    int connected_components_cnt;

    UnionFind(int n) : par(n, -1), rank(n, 0), siz(n, 1), connected_components_cnt(n) {}
    
    int root(int x) {
        if (par[x] == -1)
            return x;
        else
            return root(par[x]);
    }

    bool issame(int x, int y) {
        return root(x) == root(y);
    }

    bool unite(int x, int y) {
        int rx = root(x), ry = root(y);
        if (rx == ry)
            return false;
        if (siz[rx] < siz[ry]) 
            std::swap(rx, ry);
        par[ry] = rx;
        siz[rx] += siz[ry];
        connected_components_cnt--;
        return true;
    }

    int size(int x) {
        return siz[root(x)];
    }
};