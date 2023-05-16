#pragma once
#include <vector>
#include <functional>

// verified at:
// https://atcoder.jp/contests/abc294/submissions/41451620
// https://judge.yosupo.jp/submission/139707
// https://judge.yosupo.jp/submission/139704
class HLD {
    using Graph = std::vector< std::vector<int> >;
    int n;
    Graph g;
    int root;

    std::vector<int> parent, sz, in, out, head;

    void size_dfs(int v, int p) {
        sz[v] = 1, parent[v] = p;
        for (auto &to: g[v]) {
            if (to == p) {
                if (to == g[v].back())
                    break;
                else 
                    std::swap(to, g[v].back());
            }
            size_dfs(to, v);
            sz[v] += sz[to];
            if (sz[to] > sz[g[v][0]]) 
                std::swap(to, g[v][0]);
        }
    }

    void build_dfs(int v, int p, int &tm) {
        in[v] = tm++;
        for (auto to: g[v]) {
            if (to == p)
                continue;
            head[to] = (to == g[v][0]) ? head[v] : to;
            build_dfs(to, v, tm);
        }
        out[v] = tm;
    }
public:
    HLD(int n_) : n(n_), g(n_), parent(n_), sz(n_), in(n_), out(n_), head(n_) {}

    void add_edge(int a, int b) {
        g[a].push_back(b);
        g[b].push_back(a);
    }

    void build(int _root = 0) {
        root = _root;
        int tm = 0;
        size_dfs(0, -1);
        head[root] = root;
        build_dfs(0, -1, tm);
    }

    void subtree_query(int a, const std::function< void(int, int) > &func) {
        func(in[a], out[a]);
    }

    // returns lca
    int path_query(int a, int b, const std::function< void(int, int) > &func) {
        int heada = head[a], headb = head[b];
        while (head[a] != head[b]) {
            if (in[heada] > in[headb]) {
                func(in[heada], in[a] + 1);
                a = parent[heada], heada = head[a];
            } else {
                func(in[headb], in[b] + 1);
                b = parent[headb], headb = head[b];
            }
        }
        if (in[a] > in[b])
            std::swap(a, b);
        func(in[a], in[b] + 1);
        return a;
    }

    int lca(int a, int b) {
        int heada = head[a], headb = head[b];
        while (head[a] != head[b]) {
            if (in[heada] > in[headb]) {
                a = parent[heada], heada = head[a];
            } else {
                b = parent[headb], headb = head[b];
            }
        }
        if (in[a] > in[b]) 
            std::swap(a, b);
        return a;
    }

    inline int get_idx(int v) {
        return in[v];
    }
};