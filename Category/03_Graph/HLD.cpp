// 树链剖分（Heavy-Light Decomposition，HLD）
//
// 1. 如果权值在边上的话，则先将权值压到深度大的点上，再建立线段树。
// 2. 节点 x 的子树对应的 'DFS序' 区间为 [in[x], in[x] + siz[x] - 1]。
// 3. seq 数组的作用是 将 DFS 序的时间戳（in[u] 的值）映射回原始节点编号。
//

struct HLD {
    int n;
    int cur;
    vector<vector<int>> g;
    vector<int> pa, siz, dep, top;
    vector<int> in, out;
    vector<int> seq;

    explicit HLD(int n) { init(n); }

    void init(int n) {
        this->n = n;
        this->cur = 0;
        g.assign(n, {});
        pa.resize(n);
        siz.resize(n);
        dep.resize(n);
        top.resize(n);
        in.resize(n);
        out.resize(n);
        seq.resize(n);
    }

    void addEdge(int u, int v) { g[u].push_back(v); }

    void work(int root = 0) {
        pa[root] = -1;
        dep[root] = 0;
        top[root] = root;
        dfs1(root);
        dfs2(root);
    }

    void dfs1(int u) {
        if (pa[u] != -1) {
            g[u].erase(find(g[u].begin(), g[u].end(), pa[u]));
        }
        siz[u] = 1;
        for (auto& v : g[u]) {
            pa[v] = u;
            dep[v] = dep[u] + 1;
            dfs1(v);
            siz[u] += siz[v];
            if (siz[g[u][0]] < siz[v]) {
                swap(g[u][0], v);
            }
        }
    }

    void dfs2(int u) {
        in[u] = this->cur++;
        seq[in[u]] = u;
        for (auto& v : g[u]) {
            top[v] = (v == g[u][0] ? top[u] : v);
            dfs2(v);
        }
        out[u] = this->cur;
    }

    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] > dep[top[v]]) {
                u = pa[top[u]];
            } else {
                v = pa[top[v]];
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
            u = pa[top[u]];
        }
        return seq[in[u] - dep[u] + d];
    }

    bool isAncester(int u, int v) { return in[u] <= in[v] && in[v] < out[u]; }

    int rootedParent(int u, int v) {
        if (u == v) {
            return v;
        }
        if (!isAncester(v, u)) {
            return pa[v];
        }
        return *prev(upper_bound(g[v].begin(), g[v].end(), u, [&](int x, int y) { return in[x] < in[y]; }));
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
