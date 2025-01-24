// 1> 在模意义下当且仅当 [x^0]f(x)=1 时，f(x) 有对数多项式。
//

// todo
vector<int> rev;
vector<Mint> roots {0, 1};
void dft(vector<Mint>& a) {
    int n = a.size();
    if (int(rev.size()) != n) {
        int k = __builtin_ctz(n) - 1;
        rev.resize(n);
        for (int i = 0; i < n; i++) {
            rev[i] = rev[i >> 1] >> 1 | (i & 1) << k;
        }
    }
    for (int i = 0; i < n; i++) {
        if (rev[i] < i) {
            swap(a[i], a[rev[i]]);
        }
    }
    if (int(roots.size()) < n) {
        int k = __builtin_ctz(roots.size());
        roots.resize(n);
        for (; (1 << k) < n; ++k) {
            Mint e = qpow(Mint(3), (MOD - 1) >> (k + 1));
            for (int i = 1 << (k - 1); i < (1 << k); i++) {
                roots[2 * i] = roots[i];
                roots[2 * i + 1] = roots[i] * e;
            }
        }
    }
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                Mint u = a[i + j], v = a[i + j + k] * roots[k + j];
                a[i + j] = u + v;
                a[i + j + k] = u - v;
            }
        }
    }
}
void idft(vector<Mint>& a) {
    int n = a.size();
    reverse(a.begin() + 1, a.end());
    dft(a);
    Mint inv = (1 - MOD) / n;
    for (int i = 0; i < n; i++) {
        a[i] *= inv;
    }
}

struct Poly {
    vector<Mint> a;

    Poly() {}
    Poly(const vector<Mint>& a) : a(a) {}
    Poly(const initializer_list<Mint>& a) : a(a) {}
    explicit Poly(int size) : a(size) {
        for (int i = 0; i < size; i++) {
            a[i] = Mint(i);
        }
    }

    int size() const { return a.size(); }
    void resize(int n) { a.resize(n); }

    Mint operator[](int idx) const {
        if (idx < size()) {
            return a[idx];
        } else {
            return 0;
        }
    }
    Mint& operator[](int idx) { return a[idx]; }

    Poly mulxk(int k) const {
        auto b = a;
        b.insert(b.begin(), k, 0);
        return Poly(b);
    }

    Poly modxk(int k) const {
        k = min(k, size());
        return Poly(vector<Mint>(a.begin(), a.begin() + k));
    }

    Poly divxk(int k) const {
        if (size() <= k) {
            return Poly();
        }
        return Poly(vector<Mint>(a.begin() + k, a.end()));
    }

public:
    friend Poly operator+(const Poly& a, const Poly& b) {
        vector<Mint> res(max(a.size(), b.size()));
        for (int i = 0; i < int(res.size()); i++) {
            res[i] = a[i] + b[i];
        }
        return Poly(res);
    }
    friend Poly operator-(const Poly& a, const Poly& b) {
        vector<Mint> res(max(a.size(), b.size()));
        for (int i = 0; i < int(res.size()); i++) {
            res[i] = a[i] - b[i];
        }
        return Poly(res);
    }
    friend Poly operator-(const Poly& a) {
        vector<Mint> res(a.size());
        for (int i = 0; i < int(res.size()); i++) {
            res[i] = -a[i];
        }
        return Poly(res);
    }
    friend Poly operator*(Poly a, Poly b) {
        if (a.size() == 0 || b.size() == 0) {
            return Poly();
        }
        if (a.size() < b.size()) {
            swap(a, b);
        }
        if (b.size() < 128) {
            Poly c(a.size() + b.size() - 1);
            for (int i = 0; i < a.size(); i++) {
                for (int j = 0; j < b.size(); j++) {
                    c[i + j] += a[i] * b[j];
                }
            }
            return c;
        }
        int sz = 1, tot = a.size() + b.size() - 1;
        while (sz < tot) {
            sz *= 2;
        }
        a.a.resize(sz);
        b.a.resize(sz);
        dft(a.a);
        dft(b.a);
        for (int i = 0; i < sz; ++i) {
            a.a[i] = a[i] * b[i];
        }
        idft(a.a);
        a.resize(tot);
        return a;
    }
    friend Poly operator*(Mint a, Poly b) {
        for (int i = 0; i < int(b.size()); i++) {
            b[i] *= a;
        }
        return b;
    }
    friend Poly operator*(Poly a, Mint b) {
        for (int i = 0; i < int(a.size()); i++) {
            a[i] *= b;
        }
        return a;
    }

    Poly& operator+=(Poly b) { return (*this) = (*this) + b; }
    Poly& operator-=(Poly b) { return (*this) = (*this) - b; }
    Poly& operator*=(Poly b) { return (*this) = (*this) * b; }
    Poly& operator*=(Mint b) { return (*this) = (*this) * b; }

public:
    Poly deriv() const {
        if (a.empty()) {
            return Poly();
        }
        vector<Mint> res(size() - 1);
        for (int i = 0; i < size() - 1; ++i) {
            res[i] = (i + 1) * a[i + 1];
        }
        return Poly(res);
    }

    Poly integr() const {
        vector<Mint> res(size() + 1);
        for (int i = 0; i < size(); ++i) {
            res[i + 1] = a[i] / (i + 1);
        }
        return Poly(res);
    }

    Poly inv(int m) const {
        Poly x {a[0].inv()};
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x * (Poly {2} - modxk(k) * x)).modxk(k);
        }
        return x.modxk(m);
    }

    Poly log(int m) const { return (deriv() * inv(m)).integr().modxk(m); }

    Poly exp(int m) const {
        Poly x {1};
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x * (Poly {1} - x.log(k) + modxk(k))).modxk(k);
        }
        return x.modxk(m);
    }

    Poly pow(int k, int m) const {
        int i = 0;
        while (i < size() && a[i].val() == 0) {
            i++;
        }
        if (i == size() || 1LL * i * k >= m) {
            return Poly(vector<Mint>(m));
        }
        Mint v = a[i];
        auto f = divxk(i) * v.inv();
        return (f.log(m - i * k) * k).exp(m - i * k).mulxk(i * k) * qpow(v, k);
    }

    Poly sqrt(int m) const {
        Poly x {1};
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x + (modxk(k) * x.inv(k)).modxk(k)) * ((MOD + 1) / 2);
        }
        return x.modxk(m);
    }

    Poly mulT(Poly b) const {
        if (b.size() == 0) {
            return Poly();
        }
        int n = b.size();
        reverse(b.a.begin(), b.a.end());
        return ((*this) * b).divxk(n - 1);
    }

    vector<Mint> eval(vector<Mint> x) const {
        if (size() == 0) {
            return vector<Mint>(x.size(), 0);
        }
        const int n = max(int(x.size()), size());
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
                self(self, 2 * p, l, m, num.mulT(q[2 * p + 1]).modxk(m - l));
                self(self, 2 * p + 1, m, r, num.mulT(q[2 * p]).modxk(r - m));
            }
        };
        work(work, 1, 0, n, mulT(q[1].inv(n)));
        return ans;
    }
};
