#pragma once
#include "ExtGCD.hpp"
#include <unordered_map>

long long modpow(long long a, long long x, long long m) {
    long long ret = 1;
    while (x > 0) {
        if (x & 1) ret = ret * a % m;
        a = a * a % m;
        x >>= 1;
    }
    return ret;
}

long long modinv(long long a, long long m) {
    long long x, _;
    extGCD(a, m, x, _);
    x %= m;
    if (x < 0) x += m;
    return x;
}

