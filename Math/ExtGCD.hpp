#pragma once

// Calculate the solution of ax + by = gcd(a, b)
// and return gcd(a, b)
long long extGCD(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    long long d = extGCD(b, a % b, y, x);
    y -= (a / b) * x;
    return d;
}

long long gcd(long long a, long long b) {
    long long unused1, unused2;
    return extGCD(a, b, unused1, unused2);
}