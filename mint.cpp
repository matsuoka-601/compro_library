#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

ll mod = 998244353;
ll root = 3;

struct mint {
public:
    ll x;

    mint(ll x = 0) :x((x%mod + mod)%mod) {}

    mint& operator+=(const mint a) {
        if ((x += a.x) >= mod) x -= mod;
        return *this;
    }
    mint& operator-=(const mint a) {
        if ((x += mod - a.x) >= mod) x -= mod;
        return *this;
    }
    mint& operator*=(const mint a) {
        (x *= a.x) %= mod;
        return *this;
    }
    mint operator+(const mint a) const {
        mint res = *this;
        return res += a;
    }
    mint operator-(const mint a) const {
        mint res = *this;
        return res -= a;
    }
    mint operator*(const mint a) const {
        mint res = *this;
        return res *= a;
    }
};



int main () {
}