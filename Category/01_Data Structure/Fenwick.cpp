template <typename T>
struct Fenwick {
    int n;
    vector<T> tree;

    Fenwick(int n_ = 0) : n(n_), tree(n, T {}) {}

    void add(int x, const T& v) {
        for (int i = x + 1; i <= n; i += i & -i) {
            tree[i - 1] += v;
        }
    }
    T query(int x) {
        T ans {};
        for (int i = x; i > 0; i -= i & -i) { // i &= i-1
            ans = ans + tree[i - 1];
        }
        return ans;
    }
    T query(int l, int r) { return query(r) - query(l); }

    int select(const T& k) {
        T cur {};
        int x = 0;
        for (int i = 1 << __lg(n); i; i /= 2) {
            if (x + i <= n && cur + tree[x + i - 1] <= k) {
                x += i;
                cur += tree[x - 1];
            }
        }
        return x;
    }
};
// The Fenwick is aligned with the input array 0-index.


template <typename T>
class Fenwick {
private:
    int n;
    vector<T> tree;

    void update(int id, T val) {
        for (int i = id + 1; i <= n; i += i & -i) {
            tree[i - 1] += val;
        }
    }

public:
    vector<T> nums;

public:
    Fenwick(int n = 0) : n(n), nums(n), tree(n) {}
    template <typename U>
    Fenwick(const vector<U>& vec) : Fenwick(vec.size()) {
        for (int i = 0; i < n; ++i) {
            this->nums[i] = vec[i];
            update(i, vec[i]);
        }
    }

    void add(int id, T val) {
        update(id, val);
        nums[id] += val;
    }
    void modify(int id, T x) {
        add(id, x - nums[id]);
        nums[id] = x;
    }
    T query(int id) {
        T ans = 0;
        for (int i = id + 1; i > 0; i &= i - 1) {
            ans += tree[i - 1];
        }
        return ans;
    }
    T rangeQuery(int l, int r) { return query(r) - query(l); }
};
// The Fenwick is aligned with the input array 0-index and the modifications are not independent.


template <typename T>
class Fenwick {
private:
    int n;
    vector<T> f;

public:
    Fenwick(int n = 0) : n(n), f(n, 0) {}
    template <typename U>
    Fenwick(vector<U>& vec) : f(vec.size()) {
        for (int i = 1; i <= vec.size(); ++i) {
            add(i, vec[i]);
        }
    }

public:
    void add(int id, T val) {
        for (int i = id; i <= n; i += i & -i) {
            f[i] += val;
        }
    }
    T query(int id) {
        T ans = 0;
        for (int i = id; i > 0; i &= i - 1) {
            ans += f[i];
        }
        return ans;
    }
    T query(int l, int r) { return query(r) - query(l - 1); }

    T kthMax(int k) {
        T ans = 0;
        for (int i = __lg(n); i >= 0; i--) {
            int val = ans + (1 << i);
            if (val < n && f[val] < k) {
                k -= f[val];
                ans = val;
            }
        }
        return ans + 1;
    }
};
// The Fenwick is aligned with the input array 1-index.


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

    T preSum(int id) {
        T s0 = 0, s1 = 0;
        for (int i = id; i > 0; i &= i - 1) {
            s0 += fd[i][0];
            s1 += fd[i][1];
        }
        return (id + 1) * s0 - s1;
    }

    T rangeSum(int l, int r) { return preSum(r) - preSum(l - 1); }
};
// The Fenwick is aligned with the input array 1-index.


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
