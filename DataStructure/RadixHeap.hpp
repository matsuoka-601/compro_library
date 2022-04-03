#pragma once
#include <array>
#include <assert.h>

template <class Key, class Value> class RadixHeap {
    using uint = typename std::make_unsigned<Key>::type;
    static constexpr int bit = sizeof(Key) * 8;
    int sz;
    uint last;

    std::array<std::vector< std::pair<uint, Value> >, bit + 1> v;
    std::array<uint, bit + 1> min_arr;

    // returns where the leftmost bit is in x (1-indexed)
    // when there is no bit in x, it returns 0.
    inline int getbit(uint x) {
        if (x == 0) return 0;
        return 64 - __builtin_clzll(x);
    }

public:
    RadixHeap() : sz(0), last(0) { std::fill(begin(min_arr), end(min_arr), uint(-1)); }

    bool empty() const { return sz == 0; }

    int size() const { return sz; }

    void push(const uint &key, const Value &val) {
        assert(last <= key);
        sz++;
        int b = getbit(key ^ last);
        v[b].push_back({key, val});
        min_arr[b] = std::min(min_arr[b], key);
    }

    std::pair<Key, Value> pop() {
        assert(sz);
        if (min_arr[0] == uint(-1)) {
            // redistribute elements in a idx-th bucket.
            int idx = 1;
            while (min_arr[idx] == uint(-1)) idx++; 
            last = min_arr[idx]; // update last
            for (auto &p : v[idx]) {
                int b = getbit(p.first ^ last);
                v[b].push_back(p);
                min_arr[b] = std::min(p.first, min_arr[b]);
            }
            v[idx].clear();

            // all elements in idx-th bucket go to j-th bucket (j < idx)
            min_arr[idx] = uint(-1); 
        }
        auto res = v[0].back();
        v[0].pop_back();
        sz--;
        if (v[0].empty()) min_arr[0] = uint(-1);
        return res;
    }
};