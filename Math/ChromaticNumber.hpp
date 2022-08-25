#pragma once
#include <vector>
#include <bit>

int chromatic_number(std::vector< std::vector<int> > &g) {
    int N = g.size();
    std::vector<bool> is_independent_set(1 << N, true);
    std::vector<int> dp(1 << N);
    std::vector< std::vector<bool> > adj_mat(N, std::vector<bool>(N, false));

    for (int i = 0; i < N; i++) 
        for (auto v : g[i])
            adj_mat[i][v] = true;

    for (int i = 0; i < (1 << N); i++) {
        std::vector<int> cur_v;
        for (int j = 0; j < N; j++) 
            if (i & (1 << j)) 
                cur_v.push_back(j);
        
        for (int j = 0; j < cur_v.size(); j++) {
            for (int k = j + 1; k < cur_v.size(); k++) {
                if (adj_mat[cur_v[j]][cur_v[k]]) { 
                    is_independent_set[i] = false;
                    break;
                }
            }
            if (!is_independent_set[i]) break;
         }
    }

    dp[0] = 0;
    for (int i = 1; i < (1 << N); i++) {
        int cur_set = (i - 1) & i;
        dp[i] = 1e9;
        while (1) {
            int diff_set = i & (~cur_set);
            if (is_independent_set[diff_set])
                dp[i] = std::min(dp[i], dp[cur_set] + 1);  
            if (cur_set == 0) break;
            cur_set = (cur_set - 1) & i;
        }
    }

    return dp[(1 << N) - 1];
}