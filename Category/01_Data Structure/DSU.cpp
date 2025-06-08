// 并查集 （适用于无向图）
//
// Link：
// 【RMQ 标准算法和线性树上并查集】(https://ljt12138.blog.uoj.ac/blog/4874)

struct DSU {
    vector<int> pa, siz;
    int block;

    explicit DSU(int n) : pa(n), siz(n, 1), block(n) { iota(pa.begin(), pa.end(), 0); }

    // int find(int x) { return x == pa[x] ? x : pa[x] = find(pa[x]); }
    int find(int x) {
        int root = x;
        while (pa[root] != root) {
            root = pa[root];
        }
        while (pa[x] != root) {
            int tmp = pa[x];
            pa[x] = root;
            x = tmp;
        }
        return root;
    }

    bool merge(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) {
            return false;
        }
        if (siz[px] > siz[py]) {
            swap(px, py);
        }
        pa[px] = py;
        siz[py] += siz[px];
        block--;
        return true;
    }

    bool same(int x, int y) { return find(x) == find(y); }

    int size(int x) { return siz[find(x)]; }
};


// DSU++
struct DSU {
    vector<int> pa;
    vector<int> siz;
    vector<int> _edges;
    vector<int> _loop;
    int block;

    explicit DSU(int n) : pa(n), siz(n, 1), _edges(n), _loop(n), block(n) { iota(pa.begin(), pa.end(), 0); }

    // int find(int x) { return x == pa[x] ? x : pa[x] = find(pa[x]); }
    int find(int x) {
        int root = x;
        while (pa[root] != root) {
            root = pa[root];
        }
        while (pa[x] != root) {
            int tmp = pa[x];
            pa[x] = root;
            x = tmp;
        }
        return root;
    }

    bool merge(int x, int y) {
        if (x == y) {
            _loop[find(x)] = 1;
        }
        x = find(x), y = find(y);
        _edges[x]++;
        if (x == y) {
            return false;
        }
        if (x < y) {
            swap(x, y);
        }
        pa[y] = x;
        _loop[x] |= _loop[y];
        siz[x] += siz[y];
        _edges[x] += _edges[y];
        block--;
        return true;
    }

    bool same(int x, int y) { return find(x) == find(y); }

    int size(int x) { return siz[find(x)]; }

    bool self_loop(int x) { return _loop[find(x)]; }

    bool loop(int x) { return siz[find(x)] == _edges[find(x)]; }

    int edges(int x) { return _edges[find(x)]; }

    vector<vector<int>> groups() {
        int n = pa.size();
        vector<int> cnt(n);
        for (int i = 0; i < n; ++i) {
            cnt[find(i)]++;
        }

        vector<vector<int>> res;
        res.reserve(block);
        vector<int> index(n, -1);
        for (int i = 0; i < n; ++i) {
            if (pa[i] == i) {
                index[i] = res.size();
                res.emplace_back();
                res.back().reserve(cnt[i]);
            }
        }
        for (int i = 0; i < n; ++i) {
            res[index[pa[i]]].push_back(i);
        }
        return res;
    }
};


// 可撤销并查集（DSU With Rollback）
struct DSU {
    vector<int> size;
    vector<int> pa;
    vector<array<int, 2>> his;

    explicit DSU(int n) : size(n + 1, 1), pa(n + 1) { iota(pa.begin(), pa.end(), 0); }

    int find(int x) {
        while (x != pa[x]) {
            x = pa[x];
        }
        return x;
    }

    bool merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) {
            return false;
        }
        if (size[x] < size[y]) {
            swap(x, y);
        }
        his.push_back({x, y});
        size[x] += size[y];
        pa[y] = x;
        return true;
    }

    int time() { return his.size(); }

    void revert(int time) {
        while (his.size() > time) {
            auto [x, y] = his.back();
            his.pop_back();
            pa[y] = y;
            size[x] -= size[y];
        }
    }
};

struct DSU {
    vector<pair<int&, int>> his;

    int n;
    vector<int> f, g, bip;

    DSU(int n_) : n(n_), f(n, -1), g(n), bip(n, 1) {}

    pair<int, int> find(int x) {
        if (f[x] < 0) {
            return {x, 0};
        }
        auto [u, v] = find(f[x]);
        return {u, v ^ g[x]};
    }

    void set(int& a, int b) {
        his.emplace_back(a, a);
        a = b;
    }

    void merge(int a, int b, int& ans) {
        auto [u, xa] = find(a);
        auto [v, xb] = find(b);
        int w = xa ^ xb ^ 1;
        if (u == v) {
            if (bip[u] && w) {
                set(bip[u], 0);
                ans--;
            }
            return;
        }
        if (f[u] > f[v]) {
            std::swap(u, v);
        }
        ans -= bip[u];
        ans -= bip[v];
        set(bip[u], bip[u] && bip[v]);
        set(f[u], f[u] + f[v]);
        set(f[v], u);
        set(g[v], w);
        ans += bip[u];
    }

    int timeStamp() { return his.size(); }

    void rollback(int t) {
        while (his.size() > t) {
            auto [x, y] = his.back();
            x = y;
            his.pop_back();
        }
    }
};
