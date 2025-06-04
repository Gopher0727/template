// 在线查询 LCA 及两点距离
// 倍增
struct LCA {
    int n, LOG;
    vector<vector<pair<int, i64>>> g;
    vector<vector<int>> up;
    vector<int> dep;
    vector<i64> dis;

    explicit LCA(int n) { init(n); }

    void init(int n) {
        this->n = n;
        LOG = bit_width(static_cast<unsigned>(n));
        // LOG = 1;
        // while ((1 << LOG) <= n) {
        //     LOG++;
        // }
        up.assign(LOG, vector(n, -1));
        g.assign(n, {});
        dep.assign(n, {});
        dis.assign(n, {});
    }

    void addEdge(int u, int v, int w = 1) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    void work(int root = 0) {
        dfs(root, -1);
        for (int k = 1; k < LOG; k++) {
            for (int v = 0; v < n; v++) {
                int mid = up[k - 1][v];
                up[k][v] = (mid < 0 ? -1 : up[k - 1][mid]);
            }
        }
    }

    void dfs(int u, int pa) {
        up[0][u] = pa;
        for (auto [v, w] : g[u]) {
            if (v == pa) {
                continue;
            }
            dep[v] = dep[u] + 1;
            dis[v] = dis[u] + w;
            dfs(v, u);
        }
    }

    int get_kth_ancestor(int v, int k) {
        for (int i = 0; i < LOG; i++) {
            if (k >> i & 1) {
                v = up[i][v];
                if (v == -1) {
                    break;
                }
            }
        }
        return v;
    }

    int lca(int u, int v) {
        if (dep[u] < dep[v]) {
            swap(u, v);
        }
        u = get_kth_ancestor(u, dep[u] - dep[v]);
        if (u == v) {
            return u;
        }
        for (int k = LOG - 1; k >= 0; k--) {
            if (up[k][u] != up[k][v]) {
                u = up[k][u];
                v = up[k][v];
            }
        }
        return up[0][u];
    }

    // [无权距离，加权距离]
    auto dist(int u, int v) {
        int w = lca(u, v);
        return pair {dep[u] + dep[v] - 2 * dep[w], dis[u] + dis[v] - 2 * dis[w]};
    }
};


// 离线查询 LCA（考虑存储查询的空间开销）
// Tarjan LCA + DSU
struct LCA {
    int n;
    int m;
    vector<vector<int>> g;
    vector<vector<array<int, 2>>> Qs;
    vector<int> pa;
    vector<int> vis;
    vector<int> ans;

    explicit LCA(int n, int m) { init(n, m); }

    void init(int n, int m) {
        this->n = n;
        this->m = m;
        g.assign(n, {});
        pa.assign(n, {});
        Qs.assign(m, {});
        vis.assign(n, {});
        ans.assign(m, {});
        iota(pa.begin(), pa.end(), 0);
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void addQuery(int u, int v, int id) {
        Qs[u].push_back({v, id});
        Qs[v].push_back({u, id});
    }

    void work(int root = 0) {
        tarjan(root, -1);
    }

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

    // Tarjan 算法中，合并方向 (u <- v) 是由 DFS 决定的，不需要额外优化
    bool merge(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) {
            return false;
        }
        pa[px] = py;
        return true;
    }

    void tarjan(int u, int pa) {
        // 如果需要区分状态，可以标记“正在访问”

        for (int v : g[u]) {
            if (v == pa) {
                continue;
            }
            tarjan(v, u);
            merge(u, v);
        }
        // 处理查询
        for (auto& [v, id] : Qs[u]) {
            if (vis[v]) {
                ans[id] = find(v);
            }
        }
        vis[u] = true;
    }
};


// 动态 LCA
// Link-Cut Tree

// 基本思路：
// 1. 判断连通性 find(u) == find(v)
// 2. 执行 access(u), access(v)
// access(v) 的返回值即为 LCA(u, v)
