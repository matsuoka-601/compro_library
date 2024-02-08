#pragma once
#include <assert.h>
#include <queue>
#include <iostream>
#include <limits>
#include <vector>

// this implementation is based on
// https://megalodon.jp/2022-0321-1006-53/https://ei1333.hateblo.jp:443/entry/2019/12/15/094229
// verify:
// https://atcoder.jp/contests/practice2/submissions/30316518
// https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=6331152

template<class CapType, class CostType> class MinCostFlow {
    struct Edge {
        int to, rev;
        CapType cap;
        CostType cost;
    };

    struct EdgeInfo {
        int v, offset;
    };

    using P = std::pair<CostType, int>;

    int n;
    const CostType inf;
    std::vector<CostType> h, dist;
    std::vector<int> prevv, preve;
    std::vector<CapType> b;
    std::vector<EdgeInfo> edge_infos;
    CostType res = 0;

public:
    std::vector< std::vector<Edge> > g;

    MinCostFlow(int node_size) : 
        n(node_size), inf(std::numeric_limits<CostType>::max()),
        g(n), h(n, 0), dist(n), prevv(n), preve(n), b(n) {}

    CostType solve(int s, int t, CapType f) {
        while(f > 0) {
            std::priority_queue<P, std::vector<P>, std::greater<P> > que;
            std::fill(dist.begin(), dist.end(), inf);
            dist[s] = 0;
            que.push({0, s});
            while(!que.empty()) {
                P p = que.top(); que.pop();
                int v = p.second;
                if (dist[v] < p.first) continue;
                for (int i = 0; i < (int)g[v].size(); i++) {
                    Edge &e = g[v][i];
                    CostType diff = e.cost + h[v] - h[e.to];
                    if (e.cap > 0 && dist[e.to] > dist[v] + diff) {
                        dist[e.to] = dist[v] + diff;
                        prevv[e.to] = v, preve[e.to] = i;
                        que.push({dist[e.to], e.to});
                    }
                }
            }

            if (dist[t] == inf) {
                std::cout << "-1" << std::endl;
                exit(0);
            }

            for (int i = 0; i < n; i++) h[i] += dist[i];

            CapType d = f;
            for (int v = t; v != s; v = prevv[v]) {
                d = std::min(d, g[prevv[v]][preve[v]].cap);
            }

            f -= d;
            res += h[t] * d;
            for (int v = t; v != s; v = prevv[v]) {
                Edge& e = g[prevv[v]][preve[v]];
                e.cap -= d;
                g[v][e.rev].cap += d;
            }
        }

        return res;
    }

    void set_potential(std::vector<CostType> &potential) {
        h = potential;
    }

    void add_edge(int from, int to, CapType cap, CostType cost) {
        assert(from != to); 
        g[from].push_back({to, (int)g[to].size(), cap, cost});
        g[to].push_back({from, (int)g[from].size() - 1, 0, -cost});
        edge_infos.push_back({to, (int)g[to].size() - 1});
    }

    CapType get_edge(int i) {
        EdgeInfo &ei = edge_infos[i];
        Edge &e = g[ei.v][ei.offset];
        CapType ret = e.cap;
        return ret;
    }
};