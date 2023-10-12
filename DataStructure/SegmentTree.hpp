#pragma once
#include <vector>

template <typename T>
struct MinSegTree {
    const T INF = std::numeric_limits<T>::max();
    int n;
    std::vector<T> dat;
    MinSegTree(int n_) {
        n = 1;
        while (n < n_)
            n *= 2;
        dat.assign(2*n, INF);
    }

    void update(int i, T x) {
        i += n-1;
        dat[i] = x;
        while (i > 0) {
            i = (i-1) / 2;
            dat[i] = std::min(dat[i*2 + 1], dat[i*2 + 2]);
        }
    }

    // [a, b)
    T query(int a, int b) {
        return query_impl(a, b, 0, 0, n);
    }

    T query_impl(int a, int b, int k, int l, int r) {
        if (r <= a || b <= l) {
            return INF;
        } else if (a <= l && r <= b) {
            return dat[k];
        } else {
            T vl = query_impl(a, b, k*2 + 1, l, (l+r)/2);
            T vr = query_impl(a, b, k*2 + 2, (l+r)/2, r);
            return std::min(vl, vr);
        }
    }
};


template <typename T>
struct MaxSegTree {
    const T MINF = std::numeric_limits<T>::min();
    int n;
    std::vector<T> dat;
    MaxSegTree(int n_) {
        n = 1;
        while (n < n_)
            n *= 2;
        dat.assign(2*n, MINF);
    }

    void update(int i, T x) {
        i += n-1;
        dat[i] = x;
        while (i > 0) {
            i = (i-1) / 2;
            dat[i] = std::max(dat[i*2 + 1], dat[i*2 + 2]);
        }
    }

    // [a, b)
    T query(int a, int b) {
        return query_impl(a, b, 0, 0, n);
    }

    T query_impl(int a, int b, int k, int l, int r) {
        if (r <= a || b <= l) {
            return MINF;
        } else if (a <= l && r <= b) {
            return dat[k];
        } else {
            T vl = query_impl(a, b, k*2 + 1, l, (l+r)/2);
            T vr = query_impl(a, b, k*2 + 2, (l+r)/2, r);
            return std::max(vl, vr);
        }
    }
};