#pragma once
#include <vector>

class CentroidDecomposition {
    using Graph = std::vector<std::vector<int>>;
public:
    std::vector<bool> checked;
    std::vector<size_t> subtree_size;
    const Graph& g;
    int N;

    CentroidDecomposition(const Graph& g): g(g) {
        N = g.size();
        checked.resize(N);
        subtree_size.resize(N);
        for (int i = 0; i < N; i++) checked[i] = false;
        for (int i = 0; i < N; i++) subtree_size[i] = 0;
    }

    size_t count_subtree_size(int v, int par = -1) {
        subtree_size[v] = 1;
        for (auto to: g[v]) {
            if (to == par || checked[to]) continue;
            subtree_size[v] += count_subtree_size(to, v);
        }
        return subtree_size[v];
    }

    int find_centroid(int v, size_t mid_size, int par = -1) {
        for (auto to: g[v]) {
            if (to == par || checked[to]) continue;
            if (subtree_size[to] > mid_size) return find_centroid(to, mid_size, v);
        }
        return v; 
    }

    void solve(int root) {
        int centroid = find_centroid(root, count_subtree_size(root, -1) / 2, -1);
        checked[centroid] = true;

        solve_main(centroid);

        for (auto to: g[centroid]) {
            if (checked[to]) continue;
            solve(to);
        }
    }


    void solve_main(int centroid);
};



// usage:
// ll dfs(int v, int par, vector<int> &col_to_vec, const Graph &g, vector<bool> &checked) {
//     col_cnt[c[v]]++;
 
//     if (!col_tmp_fl[c[v]]) {
//         col_tmp_fl[c[v]] = true;
//         col_to_vec.push_back(c[v]);
//     }
 
//     int sz = 1;
//     for (auto to: g[v]) {
//         if (checked[to] || to == par) continue;
//         sz += dfs(to, v, col_to_vec, g, checked);
//     }
 
//     if (col_cnt[c[v]] == 1) {
//         col_subtree_size_sum[c[v]] += sz;
//     }
 
//     col_cnt[c[v]]--;
//     return sz;
// }
 
// void CentroidDecomposition::solve_main(int centroid) {
//     vector<int> col_vec;
 
//     ll subsum = 0;
//     ll sub_sqsum = 0;
//     for (auto to: g[centroid]) {
//         if (checked[to]) continue;
 
//         vector<int> col_to_vec;
//         ll to_subtree_size = dfs(to, centroid, col_to_vec, g, checked);
//         subsum += to_subtree_size;
//         sub_sqsum += to_subtree_size * to_subtree_size;
        
//         for (auto col: col_to_vec) {
//             if (!col_fl[col]) {
//                 col_fl[col] = true;
//                 col_vec.push_back(col);
//             }
//             if (col == c[centroid]) continue;
//             ll y = col_subtree_size_sum[col];
//             col_sum[col] += y;
//             col_sqsum[col] += y * y;
//             col_subsum[col] += to_subtree_size * y;
 
//             col_subtree_size_sum[col] = 0;
//             col_tmp_fl[col] = false;
//         }
//     }
 
//     if (!col_fl[c[centroid]]) {
//         col_vec.push_back(c[centroid]);
//     }
 
//     for (auto col: col_vec) {
//         if (col != c[centroid]) {
//             ll term1 = (subsum - col_sum[col]) * col_sum[col] + col_sqsum[col] - col_subsum[col];
//             ll term2 = (col_sum[col] * col_sum[col] - col_sqsum[col]) / 2;
//             ll term3 = col_sum[col];
//             ans[col] += term1 + term2 + term3;
//         }
//         else {
//             ans[col] += subsum + (subsum * subsum - sub_sqsum) / 2 + 1;
//         }
//         col_sum[col] = col_sqsum[col] = col_subtree_size_sum[col] = col_subsum[col] = 0;
//         col_fl[col] = col_tmp_fl[col] = false;
//     }
// }