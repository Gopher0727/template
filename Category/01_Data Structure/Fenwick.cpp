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
    auto query(int pos) {
        i64 ans = 0;
        for (int i = pos; i > 0; i &= i - 1) {
            ans = ans + t[i - 1];
        }
        return ans;
    }

    // [l, r)
    auto query(int l, int r) { return query(r) - query(l); }

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


template <typename T>
struct Fenwick_2D {
    int n, m;
    vector<vector<T>> b1, b2, b3, b4;

public:
    explicit Fenwick_2D(int n, int m) : n(n), m(m) {
        b1.resize(n + 1, vector<T>(m + 1));
        b2.resize(n + 1, vector<T>(m + 1));
        b3.resize(n + 1, vector<T>(m + 1));
        b4.resize(n + 1, vector<T>(m + 1));
    }

private:
    void add(auto& f, int x, int y, T k) {
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                f[i][j] += k;
            }
        }
    }

    T ask(auto& f, int x, int y) {
        T ans = 0;
        for (int i = x; i; i -= i & -i) {
            for (int j = y; j; j -= j & -j) {
                ans += f[i][j];
            }
        }
        return ans;
    }

public:
    void add(int x, int y, T k) { // 单点修改
        add(b1, x, y, k);
        add(b2, x, y, k * (x - 1));
        add(b3, x, y, k * (y - 1));
        add(b4, x, y, k * (x - 1) * (y - 1));
    }

    void add(int x, int y, int X, int Y, int k) { // 区块修改：二维差分
        X++, Y++;
        add(x, y, k);
        add(X, y, -k);
        add(X, Y, k);
        add(x, Y, -k);
    }

    T ask(int x, int y) { // 单点查询
        T ans = 0;
        ans += x * y * ask(b1, x, y);
        ans -= y * ask(b2, x, y);
        ans -= x * ask(b3, x, y);
        ans += ask(b4, x, y);
        return ans;
    }

    T ask(int x, int y, int X, int Y) { // 区块查询：二维前缀和
        x--, y--;
        return ask(X, Y) - ask(x, Y) - ask(X, y) + ask(x, y);
    }
};
// The node index starts from 1.
