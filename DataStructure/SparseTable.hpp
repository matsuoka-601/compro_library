#pragma once
#include <vector>


// verified at:
// https://yukicoder.me/submissions/757903

template <class T> class SparseTable {
public:
    std::vector<std::vector<T> > tab;
    std::vector<int> lookup;


    void build(const std::vector<T> &v) {
        int b = 0;
        int n = v.size();
        while (1 << b <= n) b++;
        tab.assign(b, std::vector<T>(1 << b));

        for (int i = 0; i < n; i++) tab[0][i] = v[i];
        
        for (int i = 1; i < b; i++) {
            int pre_len = 1 << (i - 1);
            int ub = n - (1 << i);
            for (int j = 0; j <= ub; j++) {
                tab[i][j] = std::min(tab[i - 1][j], tab[i - 1][j + pre_len]);
            }
        }

        lookup.resize(v.size() + 1);
        for (int i = 2; i < lookup.size(); i++)
            lookup[i] = lookup[i >> 1] + 1;
    }

    // [l, r)
    inline T rmq(int l, int r) {
        // assert(l < r);
        int b = lookup[r - l];
        return std::min(tab[b][l], tab[b][r - (1 << b)]);
    }
};