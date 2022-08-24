// verified at:
// https://atcoder.jp/contests/abc145/submissions/33704571

template <long long MOD, int NMAX> class ModComb {
    using ll = long long;

    // fac[i]: i! % MOD
    // finv[i]: i^{-1} 
    ll fac[NMAX + 1], finv[NMAX + 1], inv[NMAX + 1];
public:
    ModComb() {
        fac[0] = fac[1] = 1;
        finv[0] = finv[1] = 1;
        inv[1] = 1;

        for (int i = 2; i < NMAX + 1; i++) {
            fac[i] = fac[i - 1] * i % MOD;
            inv[i] = MOD - inv[MOD % i] * (MOD / i) % MOD; // a^{-1} = -(p / a) * (p % a)^{-1}
            finv[i] = finv[i - 1] * inv[i] % MOD;
        }
    }

    ll calc(int n, int k) {
        if (n < k) return 0;
        if (n < 0 || k < 0) return 0;
        return fac[n] * (finv[k] * finv[n - k] % MOD) % MOD;
    }
};
