#include <iostream>
#include <vector>

using namespace std;
using G = vector<vector<int>>;


class HeavyLightDecomposition {
    G &g;
    vector<int> head, subtree_size;
    using P = pair<int, int>;
    template <class T> using V = vector<T>; 

    void build(int v, int cur_head){
        head[v] = cur_head;

        int max_subtree_size = 0;
        int heavy_v = -1;
        for (auto &to : g[v]) {
            if (subtree_size[to] > max_subtree_size) {
                max_subtree_size = subtree_size[to];
                heavy_v = to;
            }
        }

        build(heavy_v, cur_head);

        for (auto &to : g[v]) {
            if (to != heavy_v) build(to, to);
        }
    }

    void calc_subtree_size(int v, int par) {
        subtree_size[v] = 1;
        for (auto &to : g[v]) {
            if (to == par) continue;
            calc_subtree_size(to, v);
            subtree_size[v] += subtree_size[to];
        }
    }

    V<P> query(int u, int v) {
        
    }

public:
    explicit HeavyLightDecomposition(G &g) : g(g), head(g.size()), subtree_size(g.size()) {
        calc_subtree_size(0, -1);
        build(0, 0);
    }
};