#pragma once
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <assert.h>
#include "DataStructure/SparseTable.hpp"

// this implemantation is based on
// https://megalodon.jp/2022-0430-1612-24/https://wk1080id.hatenablog.com:443/entry/2018/12/25/005926
// the construction of suffix array is O(nlogn) where n is the length of the string

class SuffixArray
{

    std::vector<int> sort_cyclic_shifts(const std::string &s)
    {
        int n = s.size();

        std::vector<int> cn(n), pn(n), p(n), c(n), cnt(n);
        std::iota(p.begin(), p.end(), 0);
        std::sort(p.begin(), p.end(), [&](int a, int b)
                  { return s[a] == s[b] ? a < b : s[a] < s[b]; });
        c[p[0]] = 0;
        for (int i = 1; i < n; i++) c[p[i]] = (s[p[i]] == s[p[i - 1]]) ? c[p[i - 1]] : i;

        for (int len = 1; len < n; len <<= 1)
        {
            for (int i = 0; i < n; i++) {
                pn[i] = p[i] - len;
                if (pn[i] < 0) pn[i] += n;
            }

            std::iota(cnt.begin(), cnt.end(), 0);
            for (int i = 0; i < n; i++) p[cnt[c[pn[i]]]++] = pn[i];

            if (2 * len >= n) break;

            c[p[0]] = 0;
            for (int i = 1; i < n; i++)
            {
                if (c[p[i]] == c[p[i - 1]] && c[(p[i] + len) % n] == c[(p[i - 1] + len) % n])
                    cn[p[i]] = cn[p[i - 1]];
                else
                    cn[p[i]] = i;
            }
            std::swap(c, cn);
        }
        return p;
    }

    void calc_lcp(std::string &s) {
        int n = sa.size();
        int slen = n - 1;

        rank.resize(n);
        lcp.resize(n);
        for (int i = 0; i < n; i++) rank[sa[i]] = i; 

        int h = 0;
        lcp[0] = 0;
        for (int i = 0; i < slen; i++) {
            int j = sa[rank[i] - 1];

            if (h > 0) h--;
            for (; j + h < n && i + h < n; h++)
                if (s[j + h] != s[i + h]) break;

            lcp[rank[i] - 1] = h;
        }

        st.build(lcp);
    }
    

public:
    std::vector<int> sa, lcp, rank;
    SparseTable<int> st;
    bool need_lcp;

    SuffixArray(std::string &s, bool need_lcp = true): 
        need_lcp(need_lcp)
    {
        s.push_back('$');
        sa = sort_cyclic_shifts(s);
        s.pop_back();

        if (need_lcp) calc_lcp(s);
    }

    int get_lcp(int i, int j) {
        assert(need_lcp && i != j);
        int rank_i = rank[i];
        int rank_j = rank[j];
        if (rank_i > rank_j) std::swap(rank_i, rank_j);
        return st.rmq(rank_i, rank_j);
    }
    
};