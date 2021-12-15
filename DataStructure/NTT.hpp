#pragma once
#include "Utils/ModInt.hpp"
#include <iostream>
#include <assert.h>
#include <vector>

template<uint32_t MD, uint32_t root> class NTT {
public:
    using Mint = ModInt<MD>;
    NTT(){}

    // type : ift or not
    // a.size() should be less than 1 << 23
    void nft(bool type, std::vector<Mint>& a) {
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

    std::vector<Mint> convolution(const std::vector<Mint> &a, const std::vector<Mint> &b) {
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

