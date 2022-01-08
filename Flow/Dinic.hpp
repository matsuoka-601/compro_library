#pragma once
#include <queue>
#include <vector>
#include <limits>

// this implementation is based on
// https://megalodon.jp/2022-0108-1135-33/https://kopricky.github.io:443/code/NetworkFlow/dinic.html

// verify:
// https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=6190842

template<class T> class Dinic {
    int V;
    std::vector<int> level, iter;
    struct edge {
        int to, rev;
        T cap;
    };
    std::vector< std::vector<edge> > G;

    void bfs(int s) {
        fill(level.begin(), level.end(), -1);
        std::queue<int> que;
        level[s] = 0;
        que.push(s);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (auto& e : G[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    T dfs(int v, int t, T f) {
        if (v == t)
            return f;
        for (int& i = iter[v]; i < (int)G[v].size(); i++) {
            edge& e = G[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                T d = dfs(e.to, t, std::min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

public:

    Dinic(const int node_size) : 
        V(node_size), level(V), iter(V), G(V) {}

    void add_edge(int from, int to, T cap) {
        G[from].push_back({to, (int)G[to].size(), cap});
        G[to].push_back({from, (int)G[from].size() - 1, (T)0});
    }

    T solve(int s, int t) {
        T flow = 0;
        for (;;) {
            bfs(s);
            if (level[t] < 0) return flow;
            T f;
            fill(iter.begin(), iter.end(), 0);
            while ((f = dfs(s, t, std::numeric_limits<T>::max())) > 0)
                flow += f;
        }
    }
};