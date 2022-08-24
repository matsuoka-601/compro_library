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