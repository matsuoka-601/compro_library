#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;
using Graph = vector< vector<int> >;


void solve_main(int centroid);
struct CentroidDecomposition {
    vector<int> sub;
    vector<bool> checked;
    int N;
    Graph G;

    void init(const Graph &G_init) {
        N = (int)G_init.size();
        sub.resize(N);
        checked.resize(N);
        G = G_init;
        for (int i = 0; i < N; i++) sub[i]      = 0;
        for (int i = 0; i < N; i++) checked[i]  = false;
    }

    int count_subtree_size(int v, int par = -1) {
        sub[v] = 1;
        for (auto to : G[v]) {
            if (to == par || checked[to]) continue;
            sub[v] += count_subtree_size(to, v);
        }
        return sub[v];
    }

    int find_centroid(int v, int mid, int par = -1) {
        for (auto to : G[v]) {
            if (to == par || checked[to]) continue;
            if (sub[to] > mid) return find_centroid(to, mid, v);
        }
        return v;
    }

    void solve(int root) {
        int centroid = find_centroid(root, count_subtree_size(root, -1) / 2, -1);
        checked[centroid] = true;

        solve_main(centroid);

        for (auto to : G[centroid]) {
            if (checked[to]) continue;
            solve(to);
        }
    }
};


ll ans = 0;
ll mod = 1e9 + 7;
vector<int> distcnt;
CentroidDecomposition cd;
ll N, K;


void cnt_dist(int v, int curdist, int par, int delta) {
    distcnt[curdist] += delta;
    for (auto to : cd.G[v]) {
        if(to == par || cd.checked[to]) continue;
        cnt_dist(to, curdist + 1, v, delta);
    }
}

void cnt_tmp_ans(int v, int curdist, int par, ll* tmp_ans) {
    if (curdist >= K) return;
    *tmp_ans += distcnt[K - curdist];
    for (auto to : cd.G[v]) {
        if(to == par || cd.checked[to]) continue;
        cnt_tmp_ans(to, curdist + 1, v, tmp_ans);
    }
}

void solve_main(int centroid) {
    for (auto to : cd.G[centroid]){
        if (cd.checked[to]) continue;
        cnt_dist(to, 1, centroid, 1);
    }
    ans += distcnt[K];

    ll tmp_ans = 0;
    for (auto to : cd.G[centroid]) {
        if (cd.checked[to]) continue; 
        cnt_dist(to, 1, centroid, -1);
        cnt_tmp_ans(to, 1, centroid, &tmp_ans);
        cnt_dist(to, 1, centroid, 1); 
    }
    ans += tmp_ans / 2;

    for (auto to : cd.G[centroid]) {
        if (cd.checked[to]) continue;
        cnt_dist(to, 1, centroid, -1);
    }
}



int main(){
    cin >> N >> K;
    Graph G(N);
    distcnt.resize(N);
    for (int i = 0; i < N - 1; i++) {
        int v1, v2;
        cin >> v1 >> v2;
        v1--; v2--;
        G[v1].push_back(v2);
        G[v2].push_back(v1);
    }

    cd.init(G);
    cd.solve(0);
    ans %= mod;
    ans *= (K * (K + 1) / 2) % mod;
    cout << ans % mod << endl;

    return 0;
}