#pragma once
#include <tuple>

using ll = long long;


inline int calc_which_half(std::pair<ll, ll> p) {
    ll x, y;
    std::tie(x, y) = p;
    if (y > 0) return 0;
    if (y < 0) return 1;
    return x < 0;
}

bool argsort_compare(std::pair<ll, ll> p1, std::pair<ll, ll> p2) {
    ll x1, y1, x2, y2;
    std::tie(x1, y1) = p1;
    std::tie(x2, y2) = p2;
    int h1 = calc_which_half(p1);
    int h2 = calc_which_half(p2);
    if (h1 != h2) return h1 < h2;
    return 0 < x1 * y2 - x2 * y1;
}