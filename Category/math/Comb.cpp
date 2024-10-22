ll comb(int x, int y) {
    if (y > x) {
        return 0;
    }
    ll res = 1;
    for (int i = 0; i < y; i++) {
        res = res * (x - i) / (i + 1);
    }
    return res;
}

// 预处理
namespace Comb {
    const int MX = 2e5 + 1;

    ll F[MX]; // F[i] = i!
    ll INV_F[MX]; // INV_F[i] = i!^-1

    ll qpow(ll a, ll b, int p) {
        ll res = 1;
        a = (a % p + p) % p;
        for (; b; b >>= 1, a = a * a % p) {
            if (b & 1) {
                res = a * res % p;
            }
        }
        return res;
    }

    auto init = [] {
        F[0] = 1;
        for (int i = 1; i < MX; i++) {
            F[i] = F[i - 1] * i % MOD;
        }

        // 连续阶乘的逆元 从右往左 线性递推
        INV_F[MX - 1] = qpow(F[MX - 1], MOD - 2, MOD);
        for (int i = MX - 1; i; i--) {
            INV_F[i - 1] = INV_F[i] * i % MOD;
        }
        return 0;
    }();

    ll comb(int n, int m) {
        return n < m ? 0 : F[n] * INV_F[m] % MOD * INV_F[n - m] % MOD;
    }
};
