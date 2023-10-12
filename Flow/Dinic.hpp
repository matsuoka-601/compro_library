#pragma once
#include <queue>
#include <vector>
#include <limits>
#include <assert.h>

// this implementation is based on
// https://megalodon.jp/2022-0108-1135-33/https://kopricky.github.io:443/code/NetworkFlow/dinic.html

// verify:
// https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=6190842

template<class Cap> class Dinic {
    int n;
    std::vector<int> level, iter;
    struct _Edge {
        int to, rev;
        Cap cap;
    };
    std::vector< std::vector<_Edge> > g;
    std::vector< std::pair<int, int> > pos;

    void bfs(int s) {
        fill(level.begin(), level.end(), -1);
        std::queue<int> que;
        level[s] = 0;
        que.push(s);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (auto& e : g[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    Cap dfs(int v, int t, Cap f) {
        if (v == t)
            return f;
        for (int& i = iter[v]; i < (int)g[v].size(); i++) {
            _Edge& e = g[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                Cap d = dfs(e.to, t, std::min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    g[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

public:

    Dinic(const int node_size) : 
        n(node_size), level(n), iter(n), g(n) {}

    void add_edge(int from, int to, Cap cap) {
        assert(from != to);
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);
        assert(0 <= cap);

        int from_id = int(g[from].size());
        int to_id = int(g[to].size());

        pos.push_back({from, from_id});

        g[from].push_back(_Edge{to, to_id, cap});
        g[to].push_back(_Edge{from, from_id, 0});
    }

    Cap solve(int s, int t) {
        Cap flow = 0;
        for (;;) {
            bfs(s);
            if (level[t] < 0) return flow;
            Cap f;
            fill(iter.begin(), iter.end(), 0);
            while ((f = dfs(s, t, std::numeric_limits<Cap>::max())) > 0)
                flow += f;
        }
    }

    struct Edge {
        int from, to;
        Cap cap, flow;
    };

    Edge get_edge(int edge_id) const {
        int sz = pos.size();
        assert(0 <= edge_id && edge_id < sz);
        auto e = g[pos[edge_id].first][pos[edge_id].second];
        auto re = g[e.to][e.rev];
        return Edge{pos[edge_id].first, e.to, e.cap + re.cap, re.cap};
    }

    std::vector<Edge> get_edges() const {
        std::vector<Edge> edges;
        int sz = pos.size();
        for (int i = 0; i < sz; i++)
            edges.push_back(get_edge(i));
        return edges;
    }


    std::vector<bool> cut(int s) const {
        std::vector<bool> used(n, false);
        std::vector<bool> ans(n, false);
        std::queue<int> que;

        used[s] = true;
        que.push(s);

        while (!que.empty()) {
            int v = que.front();
            que.pop();            
            ans[v] = true;
            for (auto &e: g[v]) {
                if (!used[e.to] && e.cap > 0) {
                    que.push(e.to);
                    used[e.to] = true;
                }
            }
        }

        return ans;
    }
};