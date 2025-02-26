// 自动扩容类 （Mint）
//
struct Comb {
    int n;
    vector<Mint> _fac, _ifac, _inv;

    explicit Comb() : n {0}, _fac {1}, _ifac {1}, _inv {0} {}
    explicit Comb(int n) : Comb() { init(n); }

    void init(int m) {
        if (m <= n) {
            return;
        }
        _fac.resize(m + 1);
        _ifac.resize(m + 1);
        _inv.resize(m + 1);

        for (int i = n + 1; i <= m; i++) {
            _fac[i] = _fac[i - 1] * i;
        }
        _ifac[m] = _fac[m].inv();
        for (int i = m; i > n; i--) {
            _ifac[i - 1] = _ifac[i] * i;
            _inv[i] = _ifac[i] * _fac[i - 1];
        }
        n = m;
    }

    Mint fac(int m) {
        if (m > n) {
            init(2 * m);
        }
        return _fac[m];
    }
    Mint ifac(int m) {
        if (m > n) {
            init(2 * m);
        }
        return _ifac[m];
    }
    Mint inv(int m) {
        if (m > n) {
            init(2 * m);
        }
        return _inv[m];
    }
    Mint binom(int n, int m) {
        if (n < m || m < 0) {
            return 0;
        }
        return fac(n) * ifac(m) * ifac(n - m);
    }
    Mint perm(int n, int m) {
        if (n < m || m < 0) {
            return 0;
        }
        return fac(n) * ifac(n - m);
    }
    Mint catalan(int n) {
        if (n <= 0) {
            return 0;
        }
        return binom(2 * n, n) - binom(2 * n, n - 1);
    }
} C;


// 预处理阶乘及阶乘逆元，模数为质数
//
namespace Comb {
    i64 qpow(i64 a, i64 b, int p) {
        i64 res = 1;
        a = (a % p + p) % p;
        for (; b; b >>= 1, a = a * a % p) {
            if (b & 1) {
                res = a * res % p;
            }
        }
        return res;
    }

    const int MX = 2e5 + 1;

    vector<i64> Fac, iFac;
    auto init = [] {
        Fac.resize(MX); // Fac[i] = i!
        iFac.resize(MX); // iFac[i] = i!^-1

        Fac[0] = 1;
        for (int i = 1; i < MX; i++) {
            Fac[i] = Fac[i - 1] * i % MOD;
        }
        // 连续阶乘的逆元 从右往左 线性递推
        iFac[MX - 1] = qpow(Fac[MX - 1], MOD - 2, MOD);
        for (int i = MX - 1; i; i--) {
            iFac[i - 1] = iFac[i] * i % MOD;
        }
        return 0;
    }();

    i64 binom(int n, int m) {
        return n < m || m < 0 ? 0 : Fac[n] * iFac[m] % MOD * iFac[n - m] % MOD;
    }
    i64 perm(int n, int m) {
        return n < m || m < 0 ? 0 : Fac[n] * iFac[n - m] % MOD;
    }
    i64 catalan(int n) {
        return n < 0 ? 0 : binom(2 * n, n) - binom(2 * n, n - 1);
    }
};
using namespace Comb;


// 直接实现，且不取模
i64 binom(int n, int m) {
    if (n < m || m < 0) {
        return 0;
    }
    i64 ans = 1;
    for (int i = 0; i < m; i++) {
        ans = ans * (n - i) / (i + 1);
    }
    return ans;
}


// 预处理连续一组数的逆元，直接实现，模数为质数
//
namespace Comb {
    // 求连续一组数的逆元
    const int MX = 2E5 + 1;

    vector<int> inv;
    auto Inv = []() {
        inv.resize(MX + 1);
        inv[1] = 1;
        for (int i = 2; i <= MX; ++i) {
            inv[i] = p - inv[p % i] * 1ll * (p / i) % p;
        }
        return inv;
    }();
    i64 binom(int n, int m) {
        if (n < m || m < 0) {
            return 0;
        }
        i64 ans = 1;
        for (int i = 1, j = n - m + 1; i <= m; i++, j++) {
            ans = ans * j % MOD * inv[i] % MOD;
        }
        return ans;
    }
}; // namespace Comb
using namespace Comb;


// 逆元：
// https://www.cnblogs.com/zjp-shadow/p/7773566.html
//
// 根据费马小定理，利用快速幂求解逆元  inv(i) = qpow(i, mod-2, mod)


//// 组合数取模：
// Lucas 定理：（p 大概 1E6 范围，且为质数）
//
// 由于，C(p, i) = p/i * C(p-1, i-1) = 0 (mod p)  1 <= i <= p-1，有 (1+x)^p = 1 + x^p (mod p)
//
// Lucas(n, m) = C(n%p, m%p) * Lucas(n/p, m/p) (mod p)，其中 Lucas(x, 0) = 1 (mod p)
//
int Lucas(i64 n, i64 k, int p) {
    int res = 1;
    while (n > 0 || k > 0) {
        int ni = n % p;
        int ki = k % p;
        if (ki > ni) {
            return 0;
        }
        res = res * 1ll * binom(ni, ki) % p;
        n /= p;
        k /= p;
    }
    return res;
}
