// https://www.cnblogs.com/zjp-shadow/p/7773566.html

// 求连续一组数的逆元
vector<int> Inv(int n, int p) {
    vector<int> inv(n + 1);
    inv[1] = 1;
    for (int i = 2; i <= n; ++i) {
        inv[i] = p - inv[p % i] * 1ll * (p / i) % p;
    }
    return inv;
}

