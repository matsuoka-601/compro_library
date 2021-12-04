#include <vector>

using namespace std;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
template <class T> using V = vector<T>;
using namespace std;
 
template <uint MD> struct ModInt {
    using M = ModInt;
    const static M G;
    uint v;
    ModInt(ll _v = 0) { set_v(_v % MD + MD); }
    M& set_v(uint _v) {
        v = (_v < MD) ? _v : _v - MD;
        return *this;
    }
    explicit operator bool() const { return v != 0; }
    M operator-() const { return M() - *this; }
    M operator+(const M& r) const { return M().set_v(v + r.v); }
    M operator-(const M& r) const { return M().set_v(v + MD - r.v); } // "v + MD - r.v" can exceed MD, so set_v is needed
    M operator*(const M& r) const { return M().set_v(ull(v) * r.v % MD); }
    M operator/(const M& r) const { return *this * r.inv(); }
    M& operator+=(const M& r) { return *this = *this + r; }
    M& operator-=(const M& r) { return *this = *this - r; }
    M& operator*=(const M& r) { return *this = *this * r; }
    M& operator/=(const M& r) { return *this = *this / r; }
    bool operator==(const M& r) const { return v == r.v; }
    M pow(ll n) const {
        M x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }
    M inv() const { return pow(MD - 2); }
    friend ostream& operator<<(ostream& os, const M& r) { return os << r.v; }
};




using Mint = ModInt<998244353>;
template<> const Mint Mint::G = Mint(3);


template<class T> class LazyPropagationSegmentTree {

};


auto make_segtree = [](int N) {
    using F = pair<Mint, Mint>; // type of function 
    using T = Mint; // type of value
    F fe = {1, 0};
    T te = 0;

    auto merge_function = [](F& f, F& g) {
        
    };

    auto operate = [](T& v, F& f, int w) {
        Mint w_ = Mint(w);
        v = f.first * v + f.second * w_;
    };

    auto merge_values = [](T& a, T& b) {
        return a + b;
    };
};


int main() {

}