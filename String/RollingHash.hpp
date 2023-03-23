#pragma once
#include <string>
#include "Utils/ModInt.hpp"

template<uint32_t MOD> class RollingHash {
    using Mint = ModInt<MOD>;
    std::vector<Mint> prefix_hash, power;
public:
    RollingHash(const std::string &s, Mint base = 10007) {
        int sz = (int)s.size();
        prefix_hash.assign(sz+1, 0);
        power.assign(sz+1, 0);
        power[0] = 1;
        for (int i = 0; i < sz; i++) {
            power[i+1] = power[i]*base;
            prefix_hash[i+1] = prefix_hash[i]*base + Mint(s[i]);
        }
    }

    // [l, r)
    Mint get(int l, int r) const {
        return prefix_hash[r] - prefix_hash[l] * power[r-l];
    }

    Mint concat(Mint h1, Mint h2, int h2_len) const {
        return h1 * power[h2_len] + h2;
    }
};

using RH = RollingHash<1000000007>;