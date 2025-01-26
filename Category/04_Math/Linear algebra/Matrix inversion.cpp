// 矩阵的逆
//
using Matrix = vector<vector<int>>;
Matrix inv(Matrix A) {
    int n = A.size();
    int m = n * 2;
    Matrix a(A);
    for (int i = 0; i < n; ++i) {
        a[i].resize(m);
        a[i][n + i] = 1;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            if (a[j][i]) {
                swap(a[i], a[j]);
                break;
            }
        }
        if (a[i][i] == 0) { // 矩阵不是满秩的
            cout << "No Solution\n";
            return Matrix {};
        }
        int inv = qpow(a[i][i], MOD - 2, MOD);
        for (int j = i; j < m; ++j) {
            a[i][j] = a[i][j] * 1ll * inv % MOD;
        }
        for (int j = 0; j < n; ++j) {
            if (j != i) {
                int inv = a[j][i];
                for (int k = i; k < m; ++k) {
                    a[j][k] = (a[j][k] - inv * 1ll * a[i][k] % MOD + MOD) % MOD;
                }
            }
        }
    }

    Matrix ans(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            ans[i][j] = a[i][j + n];
        }
    }
    return ans;
}
