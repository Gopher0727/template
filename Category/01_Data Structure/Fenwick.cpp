template <typename T>
struct Fenwick {
    vector<T> tree;

    Fenwick(int n = 0) : tree(n, T {}) {}
    template <typename U>
    Fenwick(const vector<U>& a) : Fenwick(a.size()) {
        for (int i = 0; i < a.size(); ++i) {
            add(i, a[i]);
        }
    }

    void add(int k, const T& val) {
        for (int i = k + 1; i <= tree.size(); i += i & -i) {
            tree[i - 1] = tree[i - 1] + val;
        }
    }

    T query(int k) { // Sum of the first k elements
        T ans {};
        for (int i = k; i > 0; i &= i - 1) {
            ans = ans + tree[i - 1];
        }
        return ans;
    }
    T query(int l, int r) { return query(r) - query(l); }

    int select(const T& k) { // The length of the longest prefix that No sum of its prefix greater than k
        int x = 0, n = tree.size();
        T cur {};
        for (int i = 1 << std::__lg(n); i; i >>= 1) {
            if (x + i <= n && cur + tree[x + i - 1] <= k) {
                x += i;
                cur = cur + tree[x - 1];
            }
        }
        return x;
    }
};
// Fenwick index is aligned with the corresponding array index.
// The node index starts from 0

// 差分树状数组，利用差分数组，实现 O(log n) 的区间加、区间查询
// a[1] = diff[1], a[2] = diff[1] + diff[2], ..., a[m] = diff[1] + ... + diff[m]
// 所以   a[1] + ... + a[m] = ∑((m-i+1)* diff[i]) = (m+1)∑diff[i] - ∑(i* diff[i])
// [0] 维护 ∑ diff[i], [1] 维护 ∑ i*diff[i]
template <typename T>
class FenwickDiff {
public:
    vector<vector<T>> fd;

public:
    explicit FenwickDiff(int n) : fd(n, vector<int>(2)) {}
    template <typename U>
    explicit FenwickDiff(vector<U>& vec) : fd(vec.size(), vector<int>(2)) {
        for (int i = 1; i <= vec.size(); ++i) {
            add(i, vec[i]);
        }
    }

    void add(int id, T val) {
        for (int i = id, iv = i * val; i < fd.size(); i += i & -i) {
            fd[i][0] += val;
            fd[i][1] += iv;
        }
    }
    void add(int l, int r, T val) {
        add(l, val);
        add(r + 1, -val);
    }

    T query(int id) {
        T s0 = 0, s1 = 0;
        for (int i = id; i > 0; i &= i - 1) {
            s0 += fd[i][0];
            s1 += fd[i][1];
        }
        return (id + 1) * s0 - s1;
    }

    T query(int l, int r) { return query(r) - query(l - 1); }
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

public:
    void add(auto& f, int x, int y, T k) { // 单点修改
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                f[i][j] += k;
            }
        }
    }
    void add(int x, int y, T k) {
        add(b1, x, y, k);
        add(b2, x, y, k * (x - 1));
        add(b3, x, y, k * (y - 1));
        add(b4, x, y, k * (x - 1) * (y - 1));
    }
    void add(int x, int y, int X, int Y, int k) { // 区块修改：二维差分
        X++, Y++;
        add(x, y, k), add(X, y, -k);
        add(X, Y, k), add(x, Y, -k);
    }

    T ask(auto& f, int x, int y) { // 单点查询
        T ans = 0;
        for (int i = x; i; i -= i & -i) {
            for (int j = y; j; j -= j & -j) {
                ans += f[i][j];
            }
        }
        return ans;
    }
    T ask(int x, int y) {
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
