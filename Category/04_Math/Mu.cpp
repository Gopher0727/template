vector<int> getMu(int n) {
    /*
    · 如果 n = 1, 则 μ(n) = 1。
    · 如果 n 是平方因子自由的（即 n 不被任何平方数除尽）, 且 n 有 k 个不同的素因子，则 μ(n) = (-1)^k。
    · 如果 n 不是平方因子自由的（即 n 被某个平方数除尽）, 则 μ(n) = 0。
    */
    vector<int> mu(n + 1, 0);
    mu[1] = 1;
    vector<int> flg(n + 1);
    vector<int> p;
    p.reserve(n + 1);
    for (int i = 2; i <= n; i++) {
        if (flg[i] == 0) {
            p.push_back(i);
            mu[i] = -1;
        }
        for (int j = 0; j < p.size() && i * p[j] <= n; j++) {
            flg[i * p[j]] = 1;
            if (i % p[j] == 0) {
                mu[i * p[j]] = 0;
                break;
            }
            mu[i * p[j]] = -mu[i];
        }
    }
    return mu;
}
