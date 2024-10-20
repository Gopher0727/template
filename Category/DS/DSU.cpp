/*
    无向图
*/

struct DSU { // Implement (union by size) + (path compression)
    vector<int> pa, _size;

    DSU(int n) : pa(n), _size(n, 1) { iota(pa.begin(), pa.end(), 0); }

    int find(int x) { return x == pa[x] ? x : pa[x] = find(pa[x]); }

    void merge(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) {
            return;
        }
        if (_size[px] > _size[py]) {
            swap(px, py);
        }
        pa[px] = py;
        _size[py] += _size[px];
    }

    bool same(int x, int y) { return find(x) == find(y); }

    int size(int x) { return _size[find(x)]; }
};

// DSU++
struct DSU {
    vector<int> pa, p, e, f;

    DSU(int n) : pa(n + 1), p(n + 1, 1), e(n + 1, 1), f(n + 1, 1) { iota(pa.begin(), pa.end(), 0); }

    // int find(int x) { return x == pa[x] ? x : pa[x] = find(pa[x]); }
    int find(int x) {
        while (x != pa[x]) {
            x = pa[x] = pa[pa[x]];
        }
        return x;
    }
    bool merge(int x, int y) { // 设 x 是 y 的祖先
        if (x == y) {
            f[find(x)] = 1;
        }
        x = find(x), y = find(y);
        e[x]++;
        if (x == y) {
            return false;
        }
        if (x < y) {
            swap(x, y);
        }
        pa[y] = x;
        f[x] |= f[y];
        p[x] += p[y];
        e[x] += e[y];
        return true;
    }
    bool same(int x, int y) { return find(x) == find(y); }

    bool F(int x) { // 判断连通块内是否存在自环
        return f[find(x)];
    }
    int size(int x) { // 输出连通块中点的数量
        return p[find(x)];
    }
    int E(int x) { // 输出连通块中边的数量
        return e[find(x)];
    }
};

// 可撤销并查集（DSU With Rollback）
struct DSU {
    vector<int> size;
    vector<int> pa;
    vector<array<int, 2>> his;

    DSU(int n) : size(n + 1, 1), pa(n + 1) { iota(pa.begin(), pa.end(), 0); }

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
