#pragma once
#include <vector>
#include <algorithm>

template <class T> class Compress {
public:
    std::vector<T> compressed;

    Compress (std::vector<T> &v) {
        for (auto &x : v)
            compressed.push_back(x);
        std::sort(compressed.begin(), compressed.end());
        compressed.erase(std::unique(compressed.begin(), compressed.end()), compressed.end());
    }

    int lb(const T &x) const {
        return std::distance(compressed.begin(), std::lower_bound(compressed.begin(), compressed.end(), x));
    }

    int ub(const T &x) const {
        return std::distance(compressed.begin(), std::upper_bound(compressed.begin(), compressed.end(), x));
    }

    int size() const {
        return compressed.size();
    }

    const T &operator[](int k) const {
        return compressed[k];
    }
};