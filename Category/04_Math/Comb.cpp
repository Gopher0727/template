// 杨辉三角（左对齐）
const int MX = 34;
vector<int> c[MX];
auto init = []() {
    for (int i = 0; i < MX; i++) {
        c[i].resize(i + 1, 1);
        for (int j = 1; j < i; j++) {
            c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
        }
    }
    return 0;
}();


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


// 直接实现，且不取模
// binom(n, m) 适用于 m 的范围正常，但 n 的范围较大，整体数值范围在 long long 以内
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


// 预处理连续一组数的逆元，模数为质数
// binom(n, m) 适用于 m 的范围正常，但 n 的范围较大
namespace Comb {
    const int MX = 2E5 + 1;

    vector<int> inv;
    auto __getInv = []() {
        inv.resize(MX + 1);
        inv[1] = 1;
        // inv[i] = p / i * inv[p % i]
        for (int i = 2; i <= MX; i++) {
            inv[i] = MOD - 1ll * inv[MOD % i] * (MOD / i) % MOD;
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


//// 组合数取模：
// Lucas 定理：（p 大概 1E6 范围，且为质数）
//
// 由于，C(p, i) = p/i * C(p-1, i-1) = 0 (mod p)  1 <= i <= p-1，有 (1+x)^p = 1 + x^p (mod p)
//
// Lucas(n, m) = C(n%p, m%p) * Lucas(n/p, m/p) (mod p)，其中 Lucas(x, 0) = 1 (mod p)
//
int Lucas(i64 n, i64 k, int p) {
    int ans = 1;
    while (n > 0 || k > 0) {
        int ni = n % p;
        int ki = k % p;
        if (ni < ki) {
            return 0;
        }
        ans = 1ll * ans * binom(ni, ki) % p;
        n /= p;
        k /= p;
    }
    return ans;
}

// 当模数为 2 时，判断组合数奇偶性
bool check(i64 n, i64 i) {
    return (n & i) == i;
}
