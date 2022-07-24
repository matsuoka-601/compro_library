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

long long modlog(long long x, long long r, long long m) {
    x %= m, r %= m;

    if (x == 0) {
        if (r == 1) return 0;
        if (r == 0) return 1;
        else return -1;
    }

    long long lo = 0; long long hi = m;
    long long mid;
    while(hi - lo > 1) { 
        mid = (hi + lo) / 2;
        if (mid * mid >= m) hi = mid;
        else lo = mid;
    }
    long long sqrtM = hi;

    // map<long long, long long> xpow;
    std::unordered_map<long long, long long> xpow;
    long long tmp = x;
    for (long long q = 1; q < sqrtM; q++) {
        if (!xpow.count(tmp)) xpow[tmp] = q;
        tmp *= x;
        tmp %= m;
    }


    long long A = modpow(modinv(x, m), sqrtM, m);
    tmp = r;
    for (long long i = 0; i < sqrtM; i++) {
        if (i > 0 && tmp == 1) return i * sqrtM;
        else if (xpow.count(tmp)) return i * sqrtM + xpow[tmp];
        tmp *= A;
        tmp %= m;
    }

    return -1;
}

