#pragma once
#include <vector>
#include <bit>
#include "Utils/ModInt.hpp"

// verified at:
// https://judge.yosupo.jp/submission/101648

int chromatic_number_naive(std::vector< std::vector<int> > &g) {
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


int chromatic_number(std::vector< std::vector<int> > &g) {
    using Mint = ModInt<1000000021>;
    int N = g.size();
    std::vector<Mint> independent_set_cnt_in(1 << N, 0);
    std::vector<int> adj_bits(N);

    for (int v = 0; v < N; v++) {
        adj_bits[v] |= (1 << v);
        for (auto adj_v : g[v])
            adj_bits[v] |= (1 << adj_v);
    }

    independent_set_cnt_in[0] = 1;
    for (int i = 0; i < (1 << N); i++) {
        int v = __builtin_ctz(i);
        independent_set_cnt_in[i] = independent_set_cnt_in[i & ~(1 << v)] + independent_set_cnt_in[i & ~(adj_bits[v])];
    }

    std::vector<Mint> f = independent_set_cnt_in;
    std::vector<Mint> coeffs(1 << N);
    for (int i = 0; i < (1 << N); i++) {
        if ((N - __builtin_popcount(i)) % 2 == 0) 
            coeffs[i] = 1;
        else
            coeffs[i] = -1; 
    }

    for (int col = 1; col < N; col++) {
        // calc g
        Mint g = 0;
        for (int i = 0; i < (1 << N); i++)
            g += coeffs[i] * f[i];

        if (g != 0) 
            return col;

        // new f
        for (int i = 0; i < (1 << N); i++) 
            f[i] *= independent_set_cnt_in[i];
    }

    return N;
}