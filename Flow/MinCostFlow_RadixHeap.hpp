#pragma once
#include <assert.h>
#include <queue>
#include <iostream>
#include <limits>
#include <vector>
#include "DataStructure/RadixHeap.hpp"

// this implementation is based on
// https://megalodon.jp/2022-0321-1006-53/https://ei1333.hateblo.jp:443/entry/2019/12/15/094229
// verify:
// https://atcoder.jp/contests/practice2/submissions/30697263


template <class CapType, class CostType> class MinCostFlow_RadixHeap {
    struct Edge {
        int to, rev;
        CapType cap;
        CostType cost;
    };

    struct EdgeInfo {
        int v, offset;
        CapType cap_min, cap_max;
        bool is_negative;
    };

    using P = std::pair<CostType, int>;

    int n_tot, n, s, t;
    const CostType inf;
    std::vector< std::vector<Edge> > g;
    std::vector<CostType> h, dist;
    std::vector<int> prevv, preve;
    std::vector<CapType> b;
    std::vector<EdgeInfo> edge_infos;
    CostType res = 0;

public:
    MinCostFlow_RadixHeap(int node_size) : 
        n_tot(node_size + 2), n(node_size), s(n_tot - 2), t(n_tot - 1), 
        inf(std::numeric_limits<CostType>::max()),
        g(n_tot), h(n_tot, 0), dist(n_tot), prevv(n_tot), preve(n_tot), b(n) {}

    CostType solve(int s, int t, CapType f) {
        assert(f > 0);
        b[s] = f; b[t] = -f;
        return solve();
    }

    CostType solve() {
        CapType f = 0;

        for (int i = 0; i < n; i++) {
            if (b[i] > 0) {
                add_edge(s, i, 0, b[i], 0);
                f += b[i];
            } else if (b[i] < 0) {
                add_edge(i, t, 0, -b[i], 0);
                f += -b[i];
            }
        }

        f /= 2;

        while(f > 0) {
            RadixHeap<CostType, int> que;
            std::fill(dist.begin(), dist.end(), inf);
            dist[s] = 0;
            que.push(0, s);
            while(!que.empty()) {
                P p = que.pop();
                int v = p.second;
                if (dist[v] < p.first) continue;
                for (int i = 0; i < (int)g[v].size(); i++) {
                    Edge &e = g[v][i];
                    CostType diff = e.cost + h[v] - h[e.to];
                    if (e.cap > 0 && dist[e.to] > dist[v] + diff) {
                        dist[e.to] = dist[v] + diff;
                        prevv[e.to] = v, preve[e.to] = i;
                        que.push(dist[e.to], e.to);
                    }
                }
            }

            if (dist[t] == inf) {
                std::cout << "-1" << std::endl;
                exit(0);
            }

            for (int i = 0; i < n_tot; i++) h[i] += dist[i];

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

    
    void add_edge(int from, int to, 
        CapType cap_min, CapType cap_max, CostType cost) {
        assert(cap_max >= cap_min);
        assert(cap_min >= 0);
        assert(from != to);

        if (cap_min != 0) {
            b[from] -= cap_min;
            b[to] += cap_min;
            res += cap_min * cost;
        }

        CapType cap_diff = cap_max - cap_min;

        if(cost < 0) {
            res += cap_diff * cost;
            b[from] -= cap_diff;
            b[to] += cap_diff;
            g[to].push_back({from, (int)g[from].size(), cap_diff, -cost});
            g[from].push_back({to, (int)g[to].size() - 1, 0, cost});
            edge_infos.push_back({from, (int)g[from].size() - 1, cap_min, cap_max, true});
        } else {
            g[from].push_back({to, (int)g[to].size(), cap_diff, cost});
            g[to].push_back({from, (int)g[from].size() - 1, 0, -cost});
            edge_infos.push_back({to, (int)g[to].size() - 1, cap_min, cap_max, false});
        }
    }

    void add_edge(int from, int to, CapType cap, CostType cost){
        add_edge(from, to, 0, cap, cost);
    }

    void set_b(std::vector<CapType> &b_) {
        assert(n == (int)b_.size());
        CapType sum = 0;
        for (int i = 0; i < n; i++) sum += b_[i];
        assert(sum == 0);
        std::copy(b_.begin(), b_.end(), b.begin());
    }

    CapType get_edge(int i) {
        EdgeInfo &ei = edge_infos[i];
        Edge &e = g[ei.v][ei.offset];
        CapType ret = e.cap;

        if (ei.is_negative) ret = (ei.cap_max - ei.cap_min) - ret;
        ret += ei.cap_min;
        return ret;
    }
};
