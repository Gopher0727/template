// 带权图倍增查询 LCA 及两点间的距离
struct LCA {
    int n, LOG;
    vector<vector<int>> up;
    vector<int> dep;
    vector<i64> dis;
    vector<vector<pair<int, i64>>> adj;

    explicit LCA(int n) : n(n) {
        LOG = 1;
        while ((1 << LOG) <= n) {
            LOG++;
        }
        up.assign(LOG, vector<int>(n, -1));
        dep.assign(n, 0);
        dis.assign(n, 0);
        adj.assign(n, {});
    }

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void work(int root = 0) {
        dfs(root, -1);
        for (int k = 1; k < LOG; ++k) {
            for (int v = 0; v < n; ++v) {
                int mid = up[k - 1][v];
                up[k][v] = (mid < 0 ? -1 : up[k - 1][mid]);
            }
        }
    }

    void dfs(int v, int pa) {
        up[0][v] = pa;
        for (auto [to, w] : adj[v]) {
            if (to == pa) {
                continue;
            }
            dep[to] = dep[v] + 1;
            dis[to] = dis[v] + w;
            dfs(to, v);
        }
    }

    int get_kth_ancestor(int v, int k) {
        for (int i = 0; i < LOG; ++i) {
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
        for (int k = LOG - 1; k >= 0; --k) {
            if (up[k][u] != up[k][v]) {
                u = up[k][u];
                v = up[k][v];
            }
        }
        return up[0][u];
    }

    auto dist(int u, int v) {
        int w = lca(u, v);
        return pair {dep[u] + dep[v] - 2 * dep[w], dis[u] + dis[v] - 2 * dis[w]};
    }
};
