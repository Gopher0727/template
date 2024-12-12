// 乘法快速幂，带取模
ll qpow(ll a, ll b, int p, ll res = 1) {
    a = (a % p + p) % p;
    for (; b; b >>= 1, a = a * a % p) {
        if (b & 1) {
            res = a * res % p;
        }
    }
    return res;
}

// 乘法快速幂，未取模
ll qpow(ll a, ll b, ll res = 1) {
    for (; b; b >>= 1, a *= a) {
        if (b & 1) {
            res = res * a;
        }
    }
    return res;
}

// 矩阵快速幂（方阵）
vector<vector<ll>> multiply(vector<vector<ll>>& a, vector<vector<ll>>& b) { // 矩阵乘法
    int m = a.size(), n = b[0].size(), k = a[0].size();
    vector<vector<ll>> res(m, vector<ll>(n));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int c = 0; c < k; ++c) {
                res[i][j] += a[i][c] * b[c][j];
                // res[i][j] %= MOD;
            }
        }
    }
    return res;
}
vector<vector<ll>> qpow(vector<vector<ll>> a, ll b) {
    int n = a.size();
    vector<vector<ll>> ans(n, vector<ll>(n));
    for (int i = 0; i < n; ++i) {
        ans[i][i] = 1;
    }
    for (; b; b >>= 1) {
        if (b & 1) {
            ans = multiply(ans, a);
        }
        a = multiply(a, a);
    }
    return ans;
}
