#pragma once
#include <vector>
#include <assert.h>

// template<int MOD>class NaiveNTT {
//     using Mint = ModInt<MOD>;
// public:
//     NaiveNTT() {}

//     std::vector<Mint> convolution(const std::vector<Mint> &a, const std::vector<Mint> &b) const {
//         std::vector<Mint> ret(a.size() + b.size() - 1);
//         for (int i = 0; i < a.size(); i++)
//             for (int j = 0; j < b.size(); j++)
//                 ret[i + j] += a[i] * b[j];
//         return ret;
//     }
// };

template<int MOD, class NTTType> class BostanMori{
    using Mint = ModInt<MOD>;
    std::vector<Mint> p, q;
    NTTType ntt;
public:
    BostanMori(std::vector<Mint> &a, std::vector<Mint> &c) {
        assert(a.size() == c.size());
        int d = c.size();
        q.resize(d + 1);
        q[0] = 1;
        for (int i = 0; i < d; i++) q[i + 1] = -c[i];
        p = ntt.convolution(a, q);
        p.resize(d);
    }

    BostanMori() {}

    Mint rec(std::vector<Mint> _p, std::vector<Mint> _q, long long n) const {
        while (n) {
            assert(_q[0] == 1);
            int d = _q.size(); 
            std::vector<Mint> _qminus(_q);
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

    Mint operator[] (const long long n) const { return rec(p, q, n); }
};