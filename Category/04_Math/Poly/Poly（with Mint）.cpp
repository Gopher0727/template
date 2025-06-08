vector<Mint> roots {0, 1};
vector<int> rev;

const Mint primitiveRoot = []() {
    Mint i = 2;
    int k = __builtin_ctz(MOD - 1);
    while (true) {
        if (qpow(i, (MOD - 1) / 2) != 1) {
            break;
        }
        i++;
    }
    return qpow(i, (MOD - 1) >> k);
}();

void dft(vector<Mint>& a) {
    int n = a.size();
    assert((n & (n - 1)) == 0);

    // 预处理单位根 roots 数组
    if (roots.size() < n) {
        roots.resize(n);
        for (int k = __builtin_ctz(roots.size()); (1 << k) < n; k++) {
            auto e = qpow(primitiveRoot, 1 << (__builtin_ctz(MOD - 1) - k - 1));
            for (int i = 1 << (k - 1); i < (1 << k); i++) {
                roots[2 * i] = roots[i];
                roots[2 * i + 1] = roots[i] * e;
            }
        }
    }

    // 位逆序置换 (Bit-Reversal Permutation)
    if (rev.size() != n) {
        rev.resize(n);
        int k = __builtin_ctz(n) - 1;
        for (int i = 0; i < n; i++) {
            rev[i] = rev[i >> 1] >> 1 | (i & 1) << k;
        }
    }

    // 重排
    for (int i = 0; i < n; i++) {
        if (rev[i] < i) {
            swap(a[i], a[rev[i]]);
        }
    }

    // 核心操作：蝶形合并
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                Mint u = a[i + j];
                Mint v = a[i + j + k] * roots[k + j];
                a[i + j] = u + v;
                a[i + j + k] = u - v;
            }
        }
    }
}

void idft(vector<Mint>& a) {
    int n = a.size();

    // 在模数意义下，复数域的 “取共轭” 对应把根的幂次取反
    // 就是把所有的旋转因子变成对应的逆元 w^-j
    reverse(a.begin() + 1, a.end());
    dft(a);
    Mint inv = Mint(1 - MOD) / n; // 隐含了 reverse 带来的整体的 -1
    for (int i = 0; i < n; i++) {
        a[i] *= inv;
    }
}

const Mint inv2 = qpow(Mint(2), MOD - 2);

struct Poly : public vector<Mint> {
    Poly() : vector<Mint>() {}
    explicit constexpr Poly(int n) : vector<Mint>(n) {}
    explicit constexpr Poly(const vector<Mint>& a) : vector<Mint>(a) {}
    explicit constexpr Poly(const initializer_list<Mint>& a) : vector<Mint>(a) {}
    template <class InputIt, class = _RequireInputIter<InputIt>>
    explicit constexpr Poly(InputIt first, InputIt last) : vector<Mint>(first, last) {}
    template <class F>
    explicit constexpr Poly(int n, F f) : vector<Mint>(n) {
        for (int i = 0; i < n; i++) {
            (*this)[i] = f(i);
        }
    }

public:
    constexpr friend Poly operator+(const Poly& a, const Poly& b) {
        Poly res(std::max(a.size(), b.size()));
        for (int i = 0; i < a.size(); i++) {
            res[i] += a[i];
        }
        for (int i = 0; i < b.size(); i++) {
            res[i] += b[i];
        }
        return res;
    }

    constexpr friend Poly operator-(const Poly& a, const Poly& b) {
        Poly res(std::max(a.size(), b.size()));
        for (int i = 0; i < a.size(); i++) {
            res[i] += a[i];
        }
        for (int i = 0; i < b.size(); i++) {
            res[i] -= b[i];
        }
        return res;
    }

    constexpr friend Poly operator-(const Poly& a) {
        Poly res(a.size());
        for (int i = 0; i < res.size(); i++) {
            res[i] = -a[i];
        }
        return res;
    }

    constexpr friend Poly operator*(Poly a, Poly b) {
        if (a.size() == 0 || b.size() == 0) {
            return Poly();
        }
        if (a.size() < b.size()) {
            std::swap(a, b);
        }
        int n = 1, tot = a.size() + b.size() - 1;
        while (n < tot) {
            n <<= 1;
        }
        if (((MOD - 1) & (n - 1)) != 0 || b.size() < 128) {
            Poly c(a.size() + b.size() - 1);
            for (int i = 0; i < a.size(); i++) {
                for (int j = 0; j < b.size(); j++) {
                    c[i + j] += a[i] * b[j];
                }
            }
            return c;
        }
        a.resize(n);
        b.resize(n);
        dft(a);
        dft(b);
        for (int i = 0; i < n; ++i) {
            a[i] *= b[i];
        }
        idft(a);
        a.resize(tot);
        return a;
    }

    constexpr friend Poly operator*(Mint a, Poly b) {
        for (int i = 0; i < b.size(); i++) {
            b[i] *= a;
        }
        return b;
    }

    constexpr friend Poly operator*(Poly a, Mint b) {
        for (int i = 0; i < a.size(); i++) {
            a[i] *= b;
        }
        return a;
    }

    constexpr friend Poly operator/(Poly a, Mint b) {
        for (int i = 0; i < a.size(); i++) {
            a[i] /= b;
        }
        return a;
    }

    constexpr Poly& operator+=(Poly b) { return (*this) = (*this) + b; }
    constexpr Poly& operator-=(Poly b) { return (*this) = (*this) - b; }
    constexpr Poly& operator*=(Poly b) { return (*this) = (*this) * b; }
    constexpr Poly& operator*=(Mint b) { return (*this) = (*this) * b; }
    constexpr Poly& operator/=(Mint b) { return (*this) = (*this) / b; }

public:
    constexpr Poly shift(int k) const {
        auto b = *this;
        if (k >= 0) {
            b.insert(b.begin(), k, 0);
            return b;
        } else if (b.size() <= -k) {
            return Poly();
        } else {
            return Poly(b.begin() - k, b.end());
        }
    }

    constexpr Poly trunc(int k) const {
        Poly b = *this;
        b.resize(k);
        return b;
    }

    constexpr Poly derivative() const {
        auto& b = *this;
        if (b.empty()) {
            return Poly();
        }
        Poly res(b.size() - 1);
        for (int i = 0; i < b.size() - 1; i++) {
            res[i] = (i + 1) * b[i + 1];
        }
        return res;
    }

    constexpr Poly integral() const {
        auto& b = *this;
        Poly res(b.size() + 1);
        for (int i = 0; i < b.size(); i++) {
            res[i + 1] = b[i] / (i + 1);
        }
        return res;
    }

    constexpr Poly inv(int m) const {
        Poly x {(*this)[0].inv()};
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x * (Poly {2} - trunc(k) * x)).trunc(k);
        }
        return x.trunc(m);
    }

    constexpr Poly log(int m) const { return (derivative() * inv(m)).integral().trunc(m); }

    constexpr Poly exp(int m) const {
        Poly x {1};
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x * (Poly {1} - x.log(k) + trunc(k))).trunc(k);
        }
        return x.trunc(m);
    }

    constexpr Poly pow(int k, int m) const {
        auto& b = *this;
        int i = 0;
        while (i < b.size() && b[i].val() == 0) {
            i++;
        }
        if (i == b.size() || 1LL * i * k >= m) {
            return Poly(m);
        }
        auto f = shift(-i) * b[i].inv();
        return (f.log(m - i * k) * k).exp(m - i * k).shift(i * k) * qpow(b[i], k);
    }

    constexpr Poly sqrt(int m) const {
        Poly x {1};
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x + (trunc(k) * x.inv(k)).trunc(k)) * inv2;
        }
        return x.trunc(m);
    }

    constexpr Poly mulT(Poly b) const {
        if (b.size() == 0) {
            return Poly();
        }
        int n = b.size();
        std::reverse(b.begin(), b.end());
        return ((*this) * b).shift(-(n - 1));
    }

    constexpr vector<Mint> eval(vector<Mint> x) const {
        if (this->size() == 0) {
            return vector<Mint>(x.size(), 0);
        }
        const int n = std::max(x.size(), this->size());
        vector<Poly> q(4 * n);
        vector<Mint> ans(x.size());
        x.resize(n);
        auto build = [&](auto&& self, int p, int l, int r) -> void {
            if (r - l == 1) {
                q[p] = Poly {1, -x[l]};
            } else {
                int m = (l + r) / 2;
                self(self, 2 * p, l, m);
                self(self, 2 * p + 1, m, r);
                q[p] = q[2 * p] * q[2 * p + 1];
            }
        };
        build(build, 1, 0, n);
        auto work = [&](auto&& self, int p, int l, int r, const Poly& num) -> void {
            if (r - l == 1) {
                if (l < int(ans.size())) {
                    ans[l] = num[0];
                }
            } else {
                int m = (l + r) / 2;
                self(self, 2 * p, l, m, num.mulT(q[2 * p + 1]).trunc(m - l));
                self(self, 2 * p + 1, m, r, num.mulT(q[2 * p]).trunc(r - m));
            }
        };
        work(work, 1, 0, n, mulT(q[1].inv(n)));
        return ans;
    }
};

Poly berlekampMassey(const Poly& s) {
    Poly c, oldC;
    int f = -1;
    for (int i = 0; i < s.size(); i++) {
        auto delta = s[i];
        for (int j = 1; j <= c.size(); j++) {
            delta -= c[j - 1] * s[i - j];
        }
        if (delta == 0) {
            continue;
        }
        if (f == -1) {
            c.resize(i + 1);
            f = i;
        } else {
            auto d = oldC;
            d *= -1;
            d.insert(d.begin(), 1);
            Mint df1 = 0;
            for (int j = 1; j <= d.size(); j++) {
                df1 += d[j - 1] * s[f + 1 - j];
            }
            assert(df1 != 0);
            auto coef = delta / df1;
            d *= coef;
            Poly zeros(i - f - 1);
            zeros.insert(zeros.end(), d.begin(), d.end());
            d = zeros;
            auto temp = c;
            c += d;
            if (i - temp.size() > f - oldC.size()) {
                oldC = temp;
                f = i;
            }
        }
    }
    c *= -1;
    c.insert(c.begin(), 1);
    return c;
}

Mint linearRecurrence(Poly p, Poly q, i64 n) {
    int m = q.size() - 1;
    while (n > 0) {
        auto newq = q;
        for (int i = 1; i <= m; i += 2) {
            newq[i] *= -1;
        }
        auto newp = p * newq;
        newq = q * newq;
        for (int i = 0; i < m; i++) {
            p[i] = newp[i * 2 + n % 2];
        }
        for (int i = 0; i <= m; i++) {
            q[i] = newq[i * 2];
        }
        n /= 2;
    }
    return p[0] / q[0];
}

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

Poly get(int n, int m) {
    if (m == 0) {
        return Poly(n + 1);
    }
    if (m % 2 == 1) {
        auto f = get(n, m - 1);
        Mint p = 1;
        for (int i = 0; i <= n; i++) {
            f[n - i] += C.binom(n, i) * p;
            p *= m;
        }
        return f;
    }
    auto f = get(n, m / 2);
    auto fm = f;
    for (int i = 0; i <= n; i++) {
        fm[i] *= C.fac(i);
    }
    Poly pw(n + 1);
    pw[0] = 1;
    for (int i = 1; i <= n; i++) {
        pw[i] = pw[i - 1] * (m / 2);
    }
    for (int i = 0; i <= n; i++) {
        pw[i] *= C.ifac(i);
    }
    fm = fm.mulT(pw);
    for (int i = 0; i <= n; i++) {
        fm[i] *= C.ifac(i);
    }
    return f + fm;
}
