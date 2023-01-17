#pragma once
#include <vector>
#include <assert.h>

template<class Cap> class FordFulkerson {
    int n;
    struct _Edge {
        int to, rev;
        Cap cap;
    };
    std::vector< std::vector<_Edge> > g;
    std::vector<bool> used;
    std::vector< std::pair<int, int> > pos;

    Cap dfs(int v, int t, Cap f) {
        if (v == t)
            return f; // minimum capacity along the augmenting path
        used[v] = true;
        for (auto &e: g[v]) {
            if (!used[e.to] && e.cap > 0) {
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

    FordFulkerson(const int node_size) : 
        n(node_size), g(n), used(n) {}

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
            std::fill(used.begin(), used.end(), false);
            Cap f = dfs(s, t, std::numeric_limits<Cap>::max());
            if (f == 0) 
                return flow;
            flow += f;
        }
    }

    struct Edge {
        int from, to;
        Cap cap, flow;
    };

    Edge get_edge(int edge_id) {
        int sz = pos.size();
        assert(0 <= edge_id && edge_id < sz);
        auto e = g[pos[edge_id].first][pos[edge_id].second];
        auto re = g[e.to][e.rev];
        return Edge{pos[edge_id].first, e.to, e.cap + re.cap, re.cap};
    }

    std::vector<Edge> get_edges() {
        std::vector<Edge> edges;
        int sz = pos.size();
        for (int i = 0; i < sz; i++)
            edges.push_back(get_edge(i));
        return edges;
    }
};