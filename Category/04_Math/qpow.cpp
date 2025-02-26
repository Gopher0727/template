// 快速幂的应用：
// > 定长路径计数：
//   把该图的邻接矩阵 M 取 k 次幂，那么 M[i][j] 就表示从 i 到 j 长度为 k 的路径的数目。时间复杂度是 O(n^3 log k)
// > 置换

// 乘法快速幂，带取模
i64 qpow(i64 a, i64 b, int p, i64 res = 1) {
    a = (a % p + p) % p;
    for (; b; b >>= 1, a = a * a % p) {
        if (b & 1) {
            res = a * res % p;
        }
    }
    return res;
}

// 乘法快速幂，未取模
i64 qpow(i64 a, i64 b, i64 res = 1) {
    for (; b; b >>= 1, a *= a) {
        if (b & 1) {
            res = res * a;
        }
    }
    return res;
}

// 矩阵快速幂（方阵）
vector<vector<i64>> multiply(vector<vector<i64>>& a, vector<vector<i64>>& b) { // 矩阵乘法
    int m = a.size(), n = b[0].size(), k = a[0].size();
    vector<vector<i64>> res(m, vector<i64>(n));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int c = 0; c < k; ++c) {
                res[i][j] += a[i][c] * b[c][j];
                res[i][j] %= MOD;
            }
        }
    }
    return res;
}
// a^n @ f，其中 @ 是矩阵乘法，f 为列向量
vector<vector<i64>> qpow_mul(vector<vector<i64>> a, i64 b, vector<vector<i64>> f = {}) {
    int n = a.size();
    if (f.empty()) {
        f = vector(n, vector<i64>(n));
        for (int i = 0; i < n; ++i) {
            f[i][i] = 1;
        }
    }
    for (; b; b >>= 1) {
        if (b & 1) {
            f = multiply(a, f);
        }
        a = multiply(a, a);
    }
    return f;
}
