// 树链剖分（HLD）
//
// 1.  如果权值在边上的话，则先将权值压到深度大的点上，再建立线段树。
// 2.  结点x的子树对应的DFS序区间为[id[x], id[x] + siz[x] - 1]。
//
struct HLD {
    int n;
    vector<int> siz, top, dep, parent, in, out, seq;
    vector<vector<int>> g;
    int cur;

    HLD() {}
    HLD(int n) { init(n); }

    void init(int n) {
        this->n = n;
        siz.resize(n);
        top.resize(n);
        dep.resize(n);
        parent.resize(n);
        in.resize(n);
        out.resize(n);
        seq.resize(n);
        cur = 0;
        g.assign(n, {});
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void work(int root = 0) {
        top[root] = root;
        dep[root] = 0;
        parent[root] = -1;
        dfs1(root);
        dfs2(root);
    }

    void dfs1(int u) {
        if (parent[u] != -1) {
            g[u].erase(find(g[u].begin(), g[u].end(), parent[u]));
        }
        siz[u] = 1;
        for (auto& v : g[u]) {
            parent[v] = u;
            dep[v] = dep[u] + 1;
            dfs1(v);
            siz[u] += siz[v];
            if (siz[v] > siz[g[u][0]]) {
                swap(v, g[u][0]);
            }
        }
    }

    void dfs2(int u) {
        in[u] = cur++;
        seq[in[u]] = u;
        for (auto v : g[u]) {
            top[v] = v == g[u][0] ? top[u] : v;
            dfs2(v);
        }
        out[u] = cur;
    }

    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] > dep[top[v]]) {
                u = parent[top[u]];
            } else {
                v = parent[top[v]];
            }
        }
        return dep[u] < dep[v] ? u : v;
    }

    int dist(int u, int v) { return dep[u] + dep[v] - 2 * dep[lca(u, v)]; }

    int jump(int u, int k) {
        if (dep[u] < k) {
            return -1;
        }
        int d = dep[u] - k;
        while (dep[top[u]] > d) {
            u = parent[top[u]];
        }
        return seq[in[u] - dep[u] + d];
    }

    bool isAncester(int u, int v) { return in[u] <= in[v] && in[v] < out[u]; }

    int rootedParent(int u, int v) {
        swap(u, v);
        if (u == v) {
            return u;
        }
        if (!isAncester(u, v)) {
            return parent[u];
        }
        auto it = upper_bound(g[u].begin(), g[u].end(), v, [&](int x, int y) { return in[x] < in[y]; }) - 1;
        return *it;
    }

    int rootedSize(int u, int v) {
        if (u == v) {
            return n;
        }
        if (!isAncester(v, u)) {
            return siz[v];
        }
        return n - siz[rootedParent(u, v)];
    }

    int rootedLca(int a, int b, int c) { return lca(a, b) ^ lca(b, c) ^ lca(c, a); }
};


template <typename SegTree>
struct HLD {
    int n, idx;
    vector<vector<int>> ver;
    vector<int> siz, dep;
    vector<int> top, son, parent;
    vector<int> in, id, val;
    SegTree seg;

    HLD(int n) {
        this->n = n;
        idx = 0;
        ver.resize(n + 1);
        siz.resize(n + 1);
        dep.resize(n + 1);
        top.resize(n + 1);
        son.resize(n + 1);
        parent.resize(n + 1);
        in.resize(n + 1);
        id.resize(n + 1);
        val.resize(n + 1);
    }

    void addEdge(int x, int y) { // 建立双向边
        ver[x].push_back(y);
        ver[y].push_back(x);
    }

    void dfs1(int x) {
        siz[x] = 1;
        dep[x] = dep[parent[x]] + 1;
        for (auto y : ver[x]) {
            if (y == parent[x]) {
                continue;
            }
            parent[y] = x;
            dfs1(y);
            siz[x] += siz[y];
            if (siz[y] > siz[son[x]]) {
                son[x] = y;
            }
        }
    }

    void dfs2(int x, int up) {
        id[x] = ++idx;
        val[idx] = in[x]; // 建立编号
        top[x] = up;
        if (son[x]) {
            dfs2(son[x], up);
        }
        for (auto y : ver[x]) {
            if (y == parent[x] || y == son[x]) {
                continue;
            }
            dfs2(y, y);
        }
    }

    // 链上修改
    void modify(int l, int r, int val) {
        while (top[l] != top[r]) {
            if (dep[top[l]] < dep[top[r]]) {
                swap(l, r);
            }
            seg.modify(id[top[l]], id[l], val);
            l = parent[top[l]];
        }
        if (dep[l] > dep[r]) {
            swap(l, r);
        }
        seg.modify(id[l], id[r], val);
    }
    // 子树修改
    void modify(int root, int val) { seg.modify(id[root], id[root] + siz[root] - 1, val); }

    // 链上查询
    int ask(int l, int r) {
        int ans = 0;
        while (top[l] != top[r]) {
            if (dep[top[l]] < dep[top[r]]) {
                swap(l, r);
            }
            ans += seg.ask(id[top[l]], id[l]);
            l = parent[top[l]];
        }
        if (dep[l] > dep[r]) {
            swap(l, r);
        }
        return ans + seg.ask(id[l], id[r]);
    }
    // 子树查询
    int ask(int root) { return seg.ask(id[root], id[root] + siz[root] - 1); }

    void work(auto in, int root = 1) { // 在此初始化
        assert(in.size() == n + 1);
        this->in = in;
        dfs1(root);
        dfs2(root, root);
        seg.init(val); // 建立线段树
    }

    void work(int root = 1) { // 在此初始化
        dfs1(root);
        dfs2(root, root);
        seg.init(val); // 建立线段树
    }
};
