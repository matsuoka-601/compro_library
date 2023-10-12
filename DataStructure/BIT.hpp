#pragma once
#include <vector>
#include <cassert>

// verified at:
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_B&lang=jp
template<typename T> class BIT {
    int n;
    std::vector<T> bit;

    void add_impl(int idx, T x) {
        for (int i = idx; i < n; i += i & -i)
            bit[i] += x;
    }

    // v[1] + v[2] + ... + v[idx] (1-indexed)
    T sum_impl(int idx) {
        T ret = 0;
        for (int i = idx; i > 0; i -= i & -i)
            ret += bit[i];
        return ret;
    }
public:
    BIT(int n_) : n(n_+1), bit(n, 0) {}

    // v[l] + v[l+1] + ... + v[r-1] (0-indexed)
    T query(const int l, const int r) {
        if (l >= r)
            return 0;
        return sum_impl(r) - sum_impl(l);
    }

    // v[idx] += x (0-indexed)
    void add(const int idx, const T x) {
        assert(idx < n-1);
        add_impl(idx + 1, x);
    }

    // v[idx] = x (0-indexed)
    void update(int idx, T x) {
        assert(idx < n-1);
        T cur = query(idx, idx+1);
        add(idx, x-cur);
    }

    T get(int idx) {
        assert(idx < n-1);
        return query(idx, idx+1);
    }
};