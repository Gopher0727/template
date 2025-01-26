// 求行列式
//
template <typename T>
int determinant(vector<vector<T>> a, int MOD) {
    int n = a.size();
    int sign = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            while (a[i][i]) {
                int div = a[j][i] / a[i][i];
                for (int k = i; k < n; ++k) {
                    a[j][k] = (a[j][k] - a[i][k] * 1ll * div % MOD + MOD) % MOD;
                }
                swap(a[i], a[j]);
                sign *= -1;
            }
            swap(a[i], a[j]);
            sign *= -1;
        }
    }

    int res = 1;
    for (int i = 0; i < n; ++i) {
        res = res * 1ll * a[i][i] % MOD;
    }
    res = (res * sign + MOD) % MOD;
    return res;
}


