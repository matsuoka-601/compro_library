#pragma once
#include <vector>
#include <algorithm>
#include <queue>

// verify:
// https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=6470370#1

template <class T> class Dijkstra {
    struct Edge {
        int to;
        T cost;
    };
    using Graph = std::vector<std::vector<Edge> >;
    using P = std::pair<T, int>;
    using PQ = std::priority_queue<P, std::vector<P>, std::greater<P> >;

    Graph g;
    std::vector<T> dist;
public:
    Dijkstra(int n): g(n), dist(n) {}

    void add_edge(int from, int to, T cost) { g[from].push_back({to, cost}); }

    std::vector<T> calc_shortest_path(int s) {
        PQ que;
        std::fill(dist.begin(), dist.end(), -1); // -1 => unreachable

        dist[s] = T(0);
        que.push(std::make_pair(T(0), s));
        while(!que.empty()) {
            P p = que.top(); que.pop();
            int u = p.second;
            if (dist[u] < p.first) continue;
            for (auto &e: g[u]) {
                T tmp_cost = dist[u] + e.cost;
                int v = e.to;
                if (dist[v] == -1 || tmp_cost < dist[v]) {
                    dist[v] = tmp_cost;
                    que.push(std::make_pair(tmp_cost, v));
                }
            }
        }
        return dist;
    }
};

