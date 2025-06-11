struct Fenwick {
    int n;
    vector<i64> t;

    explicit Fenwick(int n = 0) : n(n), t(n) {}

    void add(int pos, const i64& val) {
        for (int i = pos + 1; i <= n; i += i & -i) {
            t[i - 1] += val;
        }
    }

    // [0, pos)
    i64 query(int pos) {
        i64 ans = 0;
        for (int i = pos; i > 0; i &= i - 1) {
            ans = ans + t[i - 1];
        }
        return ans;
    }

    // [l, r)
    i64 query(int l, int r) { return query(r) - query(l); }

    int select(const i64& k) {
        i64 cur = 0;
        int x = 0;
        for (int i = 1 << std::__lg(n); i; i >>= 1) {
            if (x + i <= n && cur + t[x + i - 1] <= k) {
                x += i;
                cur += t[x - 1];
            }
        }
        return x;
    }
};
// The node index starts from 0.


struct Fenwick_2D {
    int n, m;
    vector<vector<i64>> b1, b2, b3, b4;

private:
    void add(auto& f, int x, int y, i64 k) {
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                f[i - 1][j - 1] += k;
            }
        }
    }

    i64 ask(auto& f, int x, int y) {
        i64 ans = 0;
        for (int i = x; i; i -= i & -i) {
            for (int j = y; j; j -= j & -j) {
                ans += f[i - 1][j - 1];
            }
        }
        return ans;
    }

public:
    explicit Fenwick_2D(int n, int m) : n(n), m(m) {
        b1.resize(n, vector<i64>(m));
        b2.resize(n, vector<i64>(m));
        b3.resize(n, vector<i64>(m));
        b4.resize(n, vector<i64>(m));
    }

    void add(int x, int y, i64 k) {
        add(b1, x, y, k);
        add(b2, x, y, k * (x - 1));
        add(b3, x, y, k * (y - 1));
        add(b4, x, y, k * (x - 1) * (y - 1));
    }

    void add(int x, int y, int X, int Y, int k) {
        add(x, y, k);
        add(X, y, -k);
        add(X, Y, k);
        add(x, Y, -k);
    }

    i64 ask(int x, int y) { return x * y * ask(b1, x, y) - y * ask(b2, x, y) - x * ask(b3, x, y) + ask(b4, x, y); }

    i64 ask(int x, int y, int X, int Y) { return ask(X, Y) - ask(x, Y) - ask(X, y) + ask(x, y); }
};
// The node index starts from 0.
