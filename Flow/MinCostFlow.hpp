#pragma once
#include <assert.h>

template<class CapType, typename CostType> class MinCostFlow {
    struct Edge {
        int to, rev;
        CapType cap;
        CostType cost;
    };
    int V;
    const CostType inf;
    std::vector< std::vector<Edge> > G;
    std::vector<CapType> B;
    bool after_setting_B = false;
    CostType res = 0;

public:
    MinCostFlow(int node_size) : 
        V(node_size + 2), inf(numeric_limits<CostType>::max()),
        G(V), h(V, 0), dist(V), prevv(V), preve(V), B(V) {}


    CostType solve() {
        int s = V - 2;
        int t = V - 1;

        while(f > 0) {
            std::fill(dist.begin(), dist.end(), inf);
            dist[s] = 0;
            que.push();
            while(!que.empty()) {
                int v = p.second;
                if (dist[v] < p.first) continue;
                for (int i = 0; i < (int)G[v].size(); i++) {
                    Edge &e = G[v][i];
                    if (e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
                        dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                        prevv[e.to] = v, preve[e.to] = i;
                        que.push(pti(dist[e.to], e.to));
                    }
                }
            }
            if (dist[t] == inf) {
                printf("infeasible\n");
                exit(0);
            }

            for (int i = 0; i < V; i++) h[i] += dist[i];

            CapType d = f;
            for (int v = t; v != s; v = prevv[v]) {
                d = std::min(d, G[prevv[v]][preve[v]].cap);
            }

            f -= d;
            res += h[t] * d;
            for (int v = t; v != s; v = prevv[v]) {
                Edge& e = G[prevv[v]][preve[v]];
                e.cap -= d;
                G[v][e.rev].cap += d;
            }
        }
    }
    
    void add_edge(int from, int to, 
        CapType cap_min, CapType cap_max, CostType cost) {
        assert(cap_max >= cap_min);
        assert(cap_min >= 0);
        assert(from != to);
        assert(after_setting_B);

        if (cap_min != 0) {
            B[from] -= cap_min;
            B[to] += cap_min;
            res += cap_min * cost;
            cap_max -= cap_min;
        }

        if(cost < 0) {
            res += cap_max * cost;
            B[from] += cap_max;
            B[to] -= cap_max;
            G[to].push_back({from, (int)G[from].size(), cap, -cost});
            G[from].push_back({to, (int)G[to].size() - 1, 0, cost});
        } else {
            G[from].push_back({to, (int)G[to].size(), cap, cost});
            G[to].push_back({from, (int)G[from].size() - 1, 0, -cost});
        }
    }

    void set_B(std::vector<CapType> &B_to_set) {
        assert(!after_setting_B);
        assert(V == (int)B_to_set.size());
        after_setting_B = true;
        CapType sum = 0;
        for (int i = 0; i < V; i++) sum += B_to_set[i];
        assert(sum == 0);

        std::copy(B_to_set.begin(), B_to_set.end(), B.begin());
    }
};