#pragma once
#include <vector>
template <class F, class T, class Func_mf, class Func_op, class Func_mv>
class LazyPropagationSegmentTree
{
    int n_org, n;
    Func_mf merge_function;
    Func_op operate;
    Func_mv merge_values;
    F fe;
    T te;
    std::vector<T> dat;
    std::vector<F> lazy;

    void eval(int idx, int w) {
        if (lazy[idx] == fe) return;
        operate(dat[idx], lazy[idx], w);
        if (idx < n-1) {
            merge_function(lazy[2*idx + 1], lazy[idx]);
            merge_function(lazy[2*idx + 2], lazy[idx]);
        }
        lazy[idx] = fe;
    }

    // calculate the value of [a, b)
    T query(int a, int b, int idx, int lb, int rb) {
        eval(idx, rb - lb);
        if (rb <= a || b <= lb) return te;
        if (a <= lb && rb <= b) return dat[idx];
        int mb = (lb + rb) >> 1;
        T vl = query(a, b, 2*idx + 1, lb, mb);
        T vr = query(a, b, 2*idx + 2, mb, rb);
        return merge_values(vl, vr);
    }

    void update(int a, int b, F x, int idx, int lb, int rb) {
        eval(idx, rb - lb);
        if (b <= lb || rb <= a) return;
        if (a <= lb && rb <= b) {
            merge_function(lazy[idx], x);
            eval(idx, rb - lb);
        } else {
            int mb = (lb + rb) >> 1;
            update(a, b, x, 2*idx + 1, lb, mb);
            update(a, b, x, 2*idx + 2, mb, rb);
            dat[idx] = merge_values(dat[2*idx + 1], dat[2*idx + 2]);
        }
    }

public:
    LazyPropagationSegmentTree(int _n,
                               Func_mf _merge_function,
                               Func_op _operate,
                               Func_mv _merge_values,
                               F _fe, T _te) : n_org(_n),
                                               merge_function(_merge_function),
                                               operate(_operate),
                                               merge_values(_merge_values),
                                               fe(_fe),
                                               te(_te)
    {
        n = 1;
        while (n < n_org) n <<= 1;
        dat.assign(2*n - 1, te);
        lazy.assign(2*n - 1, fe);
    }

    void build(std::vector<T>& a) {
        for (int k = 0; k < (int)a.size(); k++) dat[k + n - 1] = a[k];
        for (int k = n - 2; k >= 0; k--) dat[k] = merge_values(dat[2*k + 1], dat[2*k + 2]);
    }

    // calculate the value of [a, b]
    T get_between(int a, int b) {
        return query(a, b + 1, 0, 0, n);
    }

    void update_between(int a, int b, F x) {
        update(a, b + 1, x, 0, 0, n);
    }
};



// Make Lazy Propagation Segment Tree like below in the source file.


// auto make_segtree = [](size_t n) {
//     using F = pair<Mint, Mint>;
//     using T = Mint;
//     auto merge_function = [](F& f, F& g) {
//         tie(f.first, f.second) = make_tuple(f.first*g.first, g.first*f.second + g.second);
//     };
//     auto operate = [](T& v, F& f, size_t w) {
//         Mint w_ = Mint(w);
//         v = f.first * v + w_ * f.second;
//     };
//     auto merge_values = [](T& a, T& b) {
//         return a + b;
//     };
//     F fe = {1, 0};
//     T te = 0;
//     return LazyPropagationSegmentTree<F, T, decltype(merge_function), decltype(operate), decltype(merge_values)>
//                                     (n, merge_function, operate, merge_values, fe, te);
// };