#include <vector>
#include <assert.h>

template <class T> class FormalPowerSeries {
    using F = FormalPowerSeries;

    std::vector<T> coeffs;


public:
    FormalPowerSeries(std::vector<T> v): coeffs(v) {}

    void resize(size_t n) { coeffs.resize(n); }

    T get(size_t i) { return coeffs[i]; }

    F &operator*=(std::vector<std::pair<int, T>> g) {
        return *this;
    }

    // g is sparse
    F &operator/=(std::vector<std::pair<int, T>> g) {
        size_t n = coeffs.size();
        auto [d, c] = g.front();
        assert(d == 0 && c != T(0));
        
        T inv = c.inv();
        for (int i = 0; i < n; i++) {
            for (auto &[j, b]: g) {
                if (j > i) break;
                coeffs[i] -= coeffs[i - j] * b;
            }
            coeffs[i] *= inv;
        }

        return *this;
    }
};


// usage
// #include <iostream>
// #include <vector>
// #include "Utils/ModInt.hpp"
// #include "FPS/FormalPowerSeries.hpp"

// using namespace std;
// using ll = long long;

// constexpr int MOD = 1e9 + 7;
// using Mint = ModInt<MOD>;
// using FPS = FormalPowerSeries<Mint>;
// using SFPS = vector<pair<int, Mint>>;

// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);

//     int N, K;
//     cin >> N >> K;


//     FPS f({1, -1});
//     f.resize(N + 1);
//     f *= SFPS{{1, 1}, {K, -1}};
//     f /= SFPS{{0, 1}, {1, -2}, {K + 1, 1}};

//     cout << f.get(N).v << endl;

//     return 0;
// }

