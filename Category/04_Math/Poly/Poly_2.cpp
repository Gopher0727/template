constexpr int C = 1024;
constexpr int P = 998244353;
vector<int> rev, roots {0, 1};
int qpow(int a, int b) {
    int res = 1;
    for (; b; b >>= 1, a = 1ll * a * a % P) {
        if (b & 1) {
            res = 1ll * res * a % P;
        }
    }
    return res;
}
void dft(vector<int>& a) {
    int n = a.size();
    if (rev.size() != n) {
        int k = __builtin_ctz(n) - 1;
        rev.resize(n);
        for (int i = 0; i < n; ++i) {
            rev[i] = rev[i >> 1] >> 1 | (i & 1) << k;
        }
    }
    for (int i = 0; i < n; ++i) {
        if (rev[i] < i) {
            swap(a[i], a[rev[i]]);
        }
    }
    if (int(roots.size()) < n) {
        int k = __builtin_ctz(roots.size());
        roots.resize(n);
        for (; (1 << k) < n; ++k) {
            int e = qpow(3, (P - 1) >> (k + 1));
            for (int i = 1 << (k - 1); i < (1 << k); ++i) {
                roots[2 * i] = roots[i];
                roots[2 * i + 1] = 1ll * roots[i] * e % P;
            }
        }
    }
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; ++j) {
                int u = a[i + j];
                int v = 1ll * a[i + j + k] * roots[k + j] % P;
                int x = u + v;
                if (x >= P) {
                    x -= P;
                }
                a[i + j] = x;
                x = u - v;
                if (x < 0) {
                    x += P;
                }
                a[i + j + k] = x;
            }
        }
    }
}
void idft(vector<int>& a) {
    int n = a.size();
    reverse(a.begin() + 1, a.end());
    dft(a);
    int inv = qpow(n, P - 2);
    for (int i = 0; i < n; ++i) {
        a[i] = 1ll * a[i] * inv % P;
    }
}

struct Poly {
    vector<int> a;

public:
    explicit Poly() {}
    explicit Poly(int a0) {
        if (a0) {
            a = {a0};
        }
    }
    explicit Poly(const vector<int>& a1) : a(a1) {
        while (!a.empty() && !a.back()) {
            a.pop_back();
        }
    }

    int size() const { return a.size(); }

    int operator[](int idx) const {
        if (idx < 0 || idx >= size()) {
            return 0;
        }
        return a[idx];
    }

    Poly mulxk(int k) const {
        auto b = a;
        b.insert(b.begin(), k, 0);
        return Poly(b);
    }

    Poly modxk(int k) const {
        k = min(k, size());
        return Poly(vector<int>(a.begin(), a.begin() + k));
    }

    Poly divxk(int k) const {
        if (size() <= k) {
            return Poly();
        }
        return Poly(vector<int>(a.begin() + k, a.end()));
    }

public:
    friend Poly operator+(const Poly a, const Poly& b) {
        vector<int> res(max(a.size(), b.size()));
        for (int i = 0; i < int(res.size()); ++i) {
            res[i] = a[i] + b[i];
            if (res[i] >= P) {
                res[i] -= P;
            }
        }
        return Poly(res);
    }
    friend Poly operator-(const Poly a, const Poly& b) {
        vector<int> res(max(a.size(), b.size()));
        for (int i = 0; i < int(res.size()); ++i) {
            res[i] = a[i] - b[i];
            if (res[i] < 0) res[i] += P;
        }
        return Poly(res);
    }
    friend Poly operator*(Poly a, Poly b) {
        int sz = 1, tot = a.size() + b.size() - 1;
        while (sz < tot) sz *= 2;
        a.a.resize(sz);
        b.a.resize(sz);
        dft(a.a);
        dft(b.a);
        for (int i = 0; i < sz; ++i) a.a[i] = 1ll * a[i] * b[i] % P;
        idft(a.a);
        return Poly(a.a);
    }
    Poly& operator+=(Poly b) { return (*this) = (*this) + b; }
    Poly& operator-=(Poly b) { return (*this) = (*this) - b; }
    Poly& operator*=(Poly b) { return (*this) = (*this) * b; }

public:
    Poly derivative() const {
        if (a.empty()) {
            return Poly();
        }
        vector<int> res(size() - 1);
        for (int i = 0; i < size() - 1; ++i) {
            res[i] = 1ll * (i + 1) * a[i + 1] % P;
        }
        return Poly(res);
    }
    Poly integral() const {
        if (a.empty()) {
            return Poly();
        }
        vector<int> res(size() + 1);
        for (int i = 0; i < size(); ++i) {
            res[i + 1] = 1ll * a[i] * qpow(i + 1, P - 2) % P;
        }
        return Poly(res);
    }
    Poly inv(int m) const {
        Poly x(qpow(a[0], P - 2));
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x * (2 - modxk(k) * x)).modxk(k);
        }
        return x.modxk(m);
    }
    Poly log(int m) const { return (derivative() * inv(m)).integral().modxk(m); }
    Poly exp(int m) const {
        Poly x(1);
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x * (1 - x.log(k) + modxk(k))).modxk(k);
        }
        return x.modxk(m);
    }
    Poly sqrt(int m) const {
        Poly x(1);
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x + (modxk(k) * x.inv(k)).modxk(k)) * ((P + 1) / 2);
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
    vector<int> eval(vector<int> x) const {
        if (size() == 0) {
            return vector<int>(x.size(), 0);
        }
        const int n = max(int(x.size()), size());
        vector<Poly> q(4 * n);
        vector<int> ans(x.size());
        x.resize(n);
        auto build = [&](auto&& self, int p, int l, int r) -> void {
            if (r - l == 1) {
                q[p] = vector<int> {1, (P - x[l]) % P};
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
                if (l < int(ans.size())) ans[l] = num[0];
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

void dft(vector<vector<int>>& a) {
    int n = a.size();
    for (auto& v : a) {
        dft(v);
    }
    for (int i = 0; i < int(a[0].size()); i++) {
        vector<int> v(n);
        for (int j = 0; j < n; j++) {
            v[j] = a[j][i];
        }
        dft(v);
        for (int j = 0; j < n; j++) {
            a[j][i] = v[j];
        }
    }
}
void idft(vector<vector<int>>& a) {
    int n = a.size();
    for (auto& v : a) {
        idft(v);
    }
    for (int i = 0; i < int(a[0].size()); i++) {
        vector<int> v(n);
        for (int j = 0; j < n; j++) {
            v[j] = a[j][i];
        }
        idft(v);
        for (int j = 0; j < n; j++) {
            a[j][i] = v[j];
        }
    }
}
auto inv(const vector<vector<int>>& a) {
    int m = 1;
    vector g(1, vector {Poly(a[0]).inv(C).a});
    while (m < C) {
        vector a0(4 * m, vector<int>(4 * C));
        for (int i = 0; i < 2 * m; i++) {
            for (int j = 0; j < C; j++) {
                a0[i][j] = a[i][j];
            }
        }
        dft(a0);
        g.resize(4 * m);
        for (auto& v : g) {
            v.resize(4 * C);
        }
        dft(g);
        for (int i = 0; i < 4 * m; i++) {
            for (int j = 0; j < 4 * C; j++) {
                g[i][j] = i64(g[i][j]) * (2 + i64(P - a0[i][j]) * g[i][j] % P) % P;
            }
        }
        idft(g);
        m *= 2;
        g.resize(m);
        for (auto& v : g) {
            v.resize(C);
        }
    }
    return g;
}
