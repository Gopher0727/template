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
