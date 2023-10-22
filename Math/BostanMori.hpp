#pragma once
#include <vector>
#include <assert.h>

// template<int MOD>class NaiveNTT {
//     using Mint = ModInt<MOD>;
// public:
//     NaiveNTT() {}

//     vector<Mint> convolution(const vector<Mint> &a, const vector<Mint> &b) const {
//         vector<Mint> ret(a.size() + b.size() - 1);
//         for (int i = 0; i < a.size(); i++)
//             for (int j = 0; j < b.size(); j++)
//                 ret[i + j] += a[i] * b[j];
//         return ret;
//     }
// };

template<class T, class NTTType> class BostanMori{
    std::vector<T> p, q;
    NTTType ntt;
public:
    BostanMori(std::vector<T> &a, std::vector<T> &c) {
        assert(a.size() == c.size());
        int d = c.size();
        q.resize(d + 1);
        q[0] = 1;
        for (int i = 0; i < d; i++) q[i + 1] = -c[i];
        p = ntt.convolution(a, q);
        p.resize(d);
    }

    T rec(vector<T> _p, vector<T> _q, ll n) const {
        while (n) {
            assert(_q[0] == 1);
            int d = _q.size(); vector<T> _qminus(_q);
            for (int i = 1; i < d; i += 2) _qminus[i] = -_qminus[i];
            _p = ntt.convolution(_p, _qminus);
            _q = ntt.convolution(_q, _qminus);
            for (int i = 0; i < d; i++) _q[i] = _q[2*i];
            _q.resize(d);
            if (n & 1) for (int i = 0; i < d; i++) _p[i] = _p[2*i + 1]; 
            else       for (int i = 0; i < d; i++) _p[i] = _p[2*i];
            _p.resize(d - 1);
            n >>= 1;
        }
        assert(_q[0] == 1);
        return _p[0]; 
    }

    T operator[] (const ll n) const { return rec(p, q, n); }
};