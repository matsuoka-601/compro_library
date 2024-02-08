#pragma once
#include <vector>

template<class T, class MergeFunc> class SegmentTree {
    int n;
    std::vector<T> dat;
    T id;
    MergeFunc merge;

    T query_impl(int a, int b, int k, int l, int r) {
        if (r <= a || b <= l) return id;
        else if (a <= l && r <= b) return dat[k];
        else {
            int mid = (l+r)/2;
            T vl = query_impl(a, b, 2*k + 1, l, mid);
            T vr = query_impl(a, b, 2*k + 2, mid, r);
            return merge(vl, vr);
        }
    }

public:
    SegmentTree(int n_, T id_, MergeFunc merge_): n(), dat(n_*4, id_), id(id_), merge(merge_) {
        int x = 1;
        while (n_ > x) x *= 2;
        n = x;
    }

    void update(int i, T x) {
        i += n-1;
        dat[i] = x;
        while (i > 0) i = (i-1)/2, dat[i] = merge(dat[2*i + 1], dat[2*i + 2]);
    }

    T query(int a, int b) { return query_impl(a, b, 0, 0, n); }
};

// usage: 
// auto merge = [&](T l, T r) -> T {
//     return {l.first * beki[r.second] + r.first, l.second + r.second};
// };
// T id = {0,0};
// SegmentTree<T, decltype(merge)> st(n, id, merge);