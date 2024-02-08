#pragma once
#include "Utils/ModInt.hpp"
#include "Math/ModOp.hpp"
#include <iostream>
#include <assert.h>
#include <vector>

template<uint32_t MD, uint32_t root> class NTT {
public:
    using Mint = ModInt<MD>;
    NTT(){}

    // type : ift or not
    // a.size() should be less than 1 << 23
    void nft(bool type, std::vector<Mint>& a) const {
        int n = (int)a.size(), s = 0;
        while ((1 << s) < n) s++;
        assert(1 << s == n);

        // these are calculated only once because the type is static
        static std::vector<Mint> ep, iep;
        Mint g = root;
        while (ep.size() <= s) {
            ep.push_back(g.pow(Mint(-1).v / (1 << ep.size())));
            iep.push_back(ep.back().inv());
        }


        std::vector<Mint> b(n);
        // Stockham FFT
        // no need to perform bit reversal (but not in-place)
        // memory access is sequantial
        for (int i = 1; i <= s; i++) { 
            int w = 1 << (s - i);
            Mint base = type ? iep[i] : ep[i];
            Mint now = 1;
            for (int y = 0; y < n / 2; y += w) {
                for (int x = 0; x < w; x++) {
                    auto s = a[y << 1 | x];
                    auto t = now * a[y << 1 | x | w];
                    b[y | x] = s + t;
                    b[y | x | n >> 1] = s - t;
                }
                now *= base;
            }
            swap(a, b);
        }
    }

    std::vector<Mint> convolution(const std::vector<Mint> &a, const std::vector<Mint> &b) const {
        int n = (int)a.size(), m = (int)b.size();
        if (!n || !m) return {};
        int lg = 0;
        while ((1 << lg) < n + m - 1) lg++;
        int z = 1 << lg;
        // The type of a2 and b2 is std::vector<Mint> (not reference)
        // Therefore, a2 and b2 are copies of a and b, respectively.
        auto a2 = a, b2 = b;
        a2.resize(z);
        b2.resize(z);
        nft(false, a2);
        nft(false, b2);
        for (int i = 0; i < z; i++) a2[i] *= b2[i];
        nft(true, a2);
        a2.resize(n + m - 1);
        Mint iz = Mint(z).inv();
        for (int i = 0; i < n + m - 1; i++) a2[i] *= iz;
        return a2;
    }
};



template<int MOD> class ArbitraryModNTT {
    using Mint = ModInt<MOD>;
public:
    ArbitraryModNTT() {}

    std::vector<Mint> convolution(std::vector<Mint> &a, std::vector<Mint> &b) const {
        int n = a.size();
        int m = b.size();
        constexpr size_t MOD1 = 167772161;
        constexpr size_t MOD2 = 469762049;
        constexpr size_t MOD3 = 1224736769;
        using Mint1 = ModInt<MOD1>;
        using Mint2 = ModInt<MOD2>;
        using Mint3 = ModInt<MOD3>;

        NTT<MOD1, 3> ntt1;
        NTT<MOD2, 3> ntt2;
        NTT<MOD3, 3> ntt3;

        std::vector<Mint1> a1(n), b1(m);
        std::vector<Mint2> a2(n), b2(m);
        std::vector<Mint3> a3(n), b3(m);

        for (int i = 0; i < n; i++) a1[i] = (a[i].v) % MOD1, a2[i] = (a[i].v) % MOD2, a3[i] = (a[i].v) % MOD3;
        for (int i = 0; i < m; i++) b1[i] = (b[i].v) % MOD1, b2[i] = (b[i].v) % MOD2, b3[i] = (b[i].v) % MOD3;
        auto c1 = ntt1.convolution(a1, b1);
        auto c2 = ntt2.convolution(a2, b2);
        auto c3 = ntt3.convolution(a3, b3);

        std::vector<Mint> c(c1.size());
        const Mint2 m1_inv_m2 = modinv(MOD1, MOD2);
        const Mint3 m1m2_inv_m3 = modinv((Mint3(MOD1) * MOD2).v, MOD3);
        for (int i = 0; i < c1.size(); i++) {
            long long t1 = (m1_inv_m2 * ((long long)c2[i].v - c1[i].v)).v;
            long long t = (m1m2_inv_m3 * ((long long)c3[i].v - t1 * MOD1 - c1[i].v)).v;
            c[i] = Mint(t) * MOD1 * MOD2 + Mint(t1) * MOD1 + c1[i].v;
        }

        return c;
    }
};