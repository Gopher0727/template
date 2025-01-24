vector<int> rev;
vector<Mint> roots {0, 1};
constexpr Mint findPrimitiveRoot() {
    Mint i = 2;
    int k = __builtin_ctz(MOD - 1);
    while (true) {
        if (qpow(i, (MOD - 1) / 2).val() != 1) {
            break;
        }
        i += 1;
    }
    return qpow(i, (MOD - 1) >> k);
}
constexpr Mint primitiveRoot = findPrimitiveRoot();
// constexpr Mint primitiveRoot {31}; // todo

constexpr void dft(vector<Mint>& a) {
    int n = a.size();
    if (rev.size() != n) {
        int k = __builtin_ctz(n) - 1;
        rev.resize(n);
        for (int i = 0; i < n; i++) {
            rev[i] = rev[i >> 1] >> 1 | (i & 1) << k;
        }
    }
    for (int i = 0; i < n; i++) {
        if (rev[i] < i) {
            std::swap(a[i], a[rev[i]]);
        }
    }
    if (roots.size() < n) {
        int k = __builtin_ctz(roots.size());
        roots.resize(n);
        while ((1 << k) < n) {
            auto e = qpow(primitiveRoot, 1 << (__builtin_ctz(MOD - 1) - k - 1));
            for (int i = 1 << (k - 1); i < (1 << k); i++) {
                roots[2 * i] = roots[i];
                roots[2 * i + 1] = roots[i] * e;
            }
            k++;
        }
    }
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                Mint u = a[i + j], v = a[i + j + k] * roots[k + j];
                a[i + j] = u + v, a[i + j + k] = u - v;
            }
        }
    }
}
constexpr void idft(vector<Mint>& a) {
    int n = a.size();
    std::reverse(a.begin() + 1, a.end());
    dft(a);
    Mint inv = (1 - MOD) / n;
    for (int i = 0; i < n; i++) {
        a[i] *= inv;
    }
}

const Mint inv2 = qpow(Mint(2), MOD - 2);
struct Poly : public vector<Mint> {
    Poly() : vector<Mint>() {}
    explicit constexpr Poly(int n) : vector<Mint>(n) {}
    explicit constexpr Poly(const vector<Mint>& a) : vector<Mint>(a) {}
    constexpr Poly(const initializer_list<Mint>& a) : vector<Mint>(a) {}
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
        vector<Mint> res(a.size());
        for (int i = 0; i < int(res.size()); i++) {
            res[i] = -a[i];
        }
        return Poly(res);
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
        for (int i = 0; i < int(b.size()); i++) {
            b[i] *= a;
        }
        return b;
    }
    constexpr friend Poly operator*(Poly a, Mint b) {
        for (int i = 0; i < int(a.size()); i++) {
            a[i] *= b;
        }
        return a;
    }
    constexpr friend Poly operator/(Poly a, Mint b) {
        for (int i = 0; i < int(a.size()); i++) {
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
        if (k >= 0) {
            auto b = *this;
            b.insert(b.begin(), k, 0);
            return b;
        } else if (this->size() <= -k) {
            return Poly();
        } else {
            return Poly(this->begin() + (-k), this->end());
        }
    }

    constexpr Poly trunc(int k) const {
        Poly f = *this;
        f.resize(k);
        return f;
    }

    constexpr Poly derivative() const {
        if (this->empty()) {
            return Poly();
        }
        Poly res(this->size() - 1);
        for (int i = 0; i < this->size() - 1; ++i) {
            res[i] = (i + 1) * (*this)[i + 1];
        }
        return res;
    }

    constexpr Poly integral() const {
        Poly res(this->size() + 1);
        for (int i = 0; i < this->size(); ++i) {
            res[i + 1] = (*this)[i] / (i + 1);
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
        int i = 0;
        while (i < this->size() && (*this)[i].val() == 0) {
            i++;
        }
        if (i == this->size() || 1LL * i * k >= m) {
            return Poly(m);
        }
        Mint v = (*this)[i];
        auto f = shift(-i) * v.inv();
        return (f.log(m - i * k) * k).exp(m - i * k).shift(i * k) * qpow(v, k);
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

Mint linearRecurrence(Poly p, Poly q, ll n) {
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

struct Combinatorics {
    int n;
    vector<Mint> _fac, _invfac, _inv;

    Combinatorics() : n {0}, _fac {1}, _invfac {1}, _inv {0} {}
    Combinatorics(int n) : Combinatorics() { init(n); }

    void init(int m) {
        if (m <= n) {
            return;
        }
        _fac.resize(m + 1);
        _invfac.resize(m + 1);
        _inv.resize(m + 1);

        for (int i = n + 1; i <= m; i++) {
            _fac[i] = _fac[i - 1] * i;
        }
        _invfac[m] = _fac[m].inv();
        for (int i = m; i > n; i--) {
            _invfac[i - 1] = _invfac[i] * i;
            _inv[i] = _invfac[i] * _fac[i - 1];
        }
        n = m;
    }

    Mint fac(int m) {
        if (m > n) {
            init(2 * m);
        }
        return _fac[m];
    }
    Mint invfac(int m) {
        if (m > n) {
            init(2 * m);
        }
        return _invfac[m];
    }
    Mint inv(int m) {
        if (m > n) {
            init(2 * m);
        }
        return _inv[m];
    }
    Mint comb(int n, int m) {
        if (n < m || m < 0) {
            return 0;
        }
        return fac(n) * invfac(m) * invfac(n - m);
    }
    Mint perm(int n, int m) {
        if (n < m || m < 0) {
            return 0;
        }
        return fac(n) * invfac(n - m);
    }
    Mint catalan(int n) {
        if (n <= 0) {
            return 0;
        }
        return comb(2 * n, n) - comb(2 * n, n - 1);
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
            f[n - i] += C.comb(n, i) * p;
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
        pw[i] *= C.invfac(i);
    }
    fm = fm.mulT(pw);
    for (int i = 0; i <= n; i++) {
        fm[i] *= C.invfac(i);
    }
    return f + fm;
}
