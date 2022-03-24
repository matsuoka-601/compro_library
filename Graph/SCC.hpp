#pragma once
#include <vector>
#include <algorithm>

// verify:
// https://judge.yosupo.jp/submission/83473
// https://atcoder.jp/contests/arc010/submissions/30376565


class SCC {
    using Graph = std::vector< std::vector<int> >;

    void dfs(int v) {
        used[v] = true;
        for (auto &to: g[v]) {
            if (!used[to]) dfs(to);
        }
        ord.push_back(v);
    }

    void rdfs(int v, int group_id) {
        used[v] = true;
        cmp[v] = group_id;
        for (auto &to: rg[v]) {
            if (!used[to]) rdfs(to, group_id);
        }
    }

    void build() {
        for (int v = 0; v < n; v++)
            if (!used[v]) dfs(v);

        std::fill(used.begin(), used.end(), 0);

        std::reverse(ord.begin(), ord.end());
        for (auto &v: ord)
            if (!used[v]) rdfs(v, n_group++);
    }

    int n, n_group;
    Graph g, rg;
    std::vector<bool> used;
    std::vector<int> ord;
public:
    std::vector< std::vector<int> > group_list;
    std::vector<int> cmp;

    SCC(Graph &g): n((int)g.size()), n_group(0), g(g), used(n), cmp(n){
        rg.resize(n);
        for (int i = 0; i < n; i++)
            for (auto to: g[i]) 
                rg[to].push_back(i);
        build();
    }

    void make_group_list() {
        group_list.resize(n_group);
        for (int i = 0; i < n; i++) group_list[cmp[i]].push_back(i);
    }

    Graph rebuild() {
        Graph ret_g(n_group);
        make_group_list();

        std::vector<bool> marked_fl(n_group, false);
        for (int i = 0; i < n_group; i++) {
            std::vector<int> rec_marked_group;
            for (auto &v: group_list[i]) {
                for (auto &to: g[v]) {
                    int to_group_id = cmp[to];
                    if (to_group_id != i && !marked_fl[to_group_id]) {
                        marked_fl[to_group_id] = true;
                        rec_marked_group.push_back(to_group_id);
                        ret_g[i].push_back(to_group_id);
                    }
                }
            }
            for (auto &group_id: rec_marked_group) marked_fl[group_id] = false;
        }

        return ret_g;
    }
};