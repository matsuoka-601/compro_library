#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;
typedef long long ll;




constexpr ll mod = 998244353;
constexpr ll root = 3;

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

struct NTT{
public:
    vector<mint> wlist; // list of 2^i th root

    mint modpow(mint a, ll n) {
        mint res = 1;
        while (n > 0) {
            if (n & 1) res = res * a;
            a = a * a;
            n >>= 1;
        }
        return res;
    }

    bool ispow2(int N) {
        if (N <= 0) return false;
        while (N > 1) {
            if (N % 2 != 0) return false;
            N /= 2;
        }
        return true;
    }

    vector<mint> dft (vector<mint> A) {
        int N = A.size();
        assert(ispow2(N));
        // bit reverse
        int h = 0;
        for (int i = 0; (1 << i) < N; i++) h++;
        for (int i = 0; i < N; i++) {
            int j = 0;
            for (int k = 0; k < h; k++) j |= (i >> k & 1) << (h - 1 - k);
            if (i < j) swap(A[i], A[j]);
        }

        // butterfly
        int cnt = 1;
        for (int b = 1; b < N; b <<= 1) { // b: half of the block size
            mint w = wlist[cnt];
            mint cur = 1;
            for (int j = 0; j < b; j++) {
                int double_b = b << 1;
                for (int k = 0; k < N; k += double_b) { // iterate over each block
                    mint s = A[k + j];
                    mint t = cur * A[k + j + b];
                    A[k + j] = s + t;
                    A[k + j + b] = s - t;
                }
                cur *= w;
            }
            cnt++;
        }

        return A;
    }

    vector<mint> inv_dft (vector<mint> A) {
        int N = A.size();
        assert(ispow2(N));
        vector<mint> arranged = dft(A);
        vector<mint> swapped(N);
        for (int i = 0; i < N; i++) swapped[i] = arranged[(N - i) % N];
        mint Ninv = modpow(N, mod - 2);
        for (int i = 0; i < N; i++) swapped[i] *= Ninv;
        return swapped;
    }

    vector<mint> convolution (vector<mint> A, vector<mint> B) {
        ll P = mod;
        P--;
        int N = 1;
        int cnt = 0;
        while ((P & 1) == 0) {
            N <<= 1;
            cnt++;
            P >>= 1;
            if (A.size() + B.size() < N) break;
        }
        mint Nthroot = modpow(root, P);

        mint cur = Nthroot;
        wlist.resize(cnt + 1);
        for (int i = cnt; i >= 0; i--) {
            wlist[i] = cur;
            cur = cur * cur;
        }

        A.resize(N);
        B.resize(N);

        A = dft(A);
        B = dft(B);

        vector<mint> C(N);
        for (int i = 0; i < N; i++) C[i] = A[i] * B[i];
        return inv_dft(C);
    }
};

NTT ntt;

int main () {
    int N, M;
    cin >> N >> M;
    vector<mint> a, b;
    a.resize(N);
    b.resize(M);
    for (int i = 0; i < N; i++) cin >> a[i].x;
    for (int i = 0; i < M; i++) cin >> b[i].x;

    vector<mint> c = ntt.convolution(a, b);

    for (int i = 0; i < N + M - 1; i++) 
        cout << c[i].x << " ";
    cout << endl;

    return 0;
}