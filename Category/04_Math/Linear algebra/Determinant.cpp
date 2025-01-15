// 求行列式
//
int determinant(vector<vector<int>> a) {
    int n = a.size();
    int sign = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            while (a[i][i] != 0) {
                int div = a[j][i] / a[i][j];
                for (int k = i; k < n; ++k) {
                    a[j][k] = (a[j][k] - a[i][k] * div % MOD + MOD) % MOD;
                }
                swap(a[i], a[j]);
                sign *= -1;
            }
        }
    }

    int res = 1;
    for (int i = 0; i < n; ++i) {
        res = res * a[i][i] % MOD;
    }
    res = (res * sign + MOD) % MOD;
    return res;
}
