// Problems:
// https://www.luogu.com.cn/problem/P4781    模板题
// https://codeforces.com/contest/622/problem/F    2600


// P_k(n) = 1^k + 2^k + ... + n^k
// 根据 x = 0, 1, ..., k-1 的结果，计算 P_k(n)
struct Lagrange {
    vector<Mint> y, fac, invfac;

public:
    explicit Lagrange(int k) { init(k); }

    void init(int k) {
        y.assign(k + 2, 0);
        for (int i = 1; i < k + 2; i++) {
            y[i] = y[i - 1] + qpow(Mint(i), k);
        }

        Mint fac = 1;
        for (int i = 1; i < k + 2; i++) {
            fac = fac * i;
        }

        invfac.assign(k + 2, 1);
        invfac.back() = fac.inv();
        for (int i = k + 1; i > 0; i--) {
            invfac[i - 1] = invfac[i] * i;
        }
    }

    Mint solve(i64 n) {
        int k = y.size();
        if (n < k) {
            return y[n];
        }

        vector<Mint> pre(k);
        pre[0] = 1;
        for (int i = 0; i < k; i++) {
            pre[i + 1] = pre[i] * (n - i);
        }

        vector<Mint> suf(k);
        suf[k - 1] = 1;
        for (int i = k - 1; i > 0; --i) {
            suf[i - 1] = suf[i] * (n - i);
        }

        Mint ans = 0;
        for (int i = 0; i < k; i++) {
            ans += y[i] * pre[i] * suf[i] * ((k - 1 - i) % 2 == 0 ? 1 : -1) * invfac[i] * invfac[k - 1 - i];
        }
        return ans;
    }
};
