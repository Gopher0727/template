// 一般图最大匹配（Graph 带花树算法）
//
// 与二分图匹配的差别在于图中可能存在奇环，时间复杂度与边的数量无关  O(N^3)
//
struct Graph {
    int n;
    vector<vector<int>> e;

public:
    Graph() = delete;
    Graph(int n) : n(n), e(n) {}

    void addEdge(int u, int v) {
        e[u].push_back(v);
        e[v].push_back(u);
    }

    vector<int> work(int m, const auto& init) {
        vector<int> match(n, -1), vis(n), link(n), f(n), dep(n);
        for (auto [x, y] : init) {
            match[x] = y;
            match[y] = x;
        }

        auto find = [&](int u) {
            while (f[u] != u) {
                u = f[u] = f[f[u]];
            }
            return u;
        };

        auto lca = [&](int u, int v) {
            u = find(u);
            v = find(v);
            while (u != v) {
                if (dep[u] < dep[v]) swap(u, v);
                u = find(link[match[u]]);
            }
            return u;
        };

        queue<int> q;
        auto blossom = [&](int u, int v, int p) {
            while (find(u) != p) {
                link[u] = v;
                v = match[u];
                if (vis[v] == 0) {
                    vis[v] = 1;
                    q.push(v);
                }
                f[u] = f[v] = p;
                u = link[v];
            }
        };

        // find an augmenting path starting from u and augment (if exist)
        auto augment = [&](int u) {
            while (!q.empty()) {
                q.pop();
            }

            iota(f.begin(), f.end(), 0);

            // vis = 0 corresponds to inner vertices
            // vis = 1 corresponds to outer vertices
            fill(vis.begin(), vis.end(), -1);

            q.push(u);
            vis[u] = 1;
            dep[u] = 0;
            int y = -1;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                if (u >= m) {
                    y = u;
                }
                for (auto v : e[u]) {
                    if (vis[v] == -1) {
                        vis[v] = 0;
                        link[v] = u;
                        dep[v] = dep[u] + 1;
                        // found an augmenting path
                        if (match[v] == -1) {
                            for (int x = v, y = u, tmp; y != -1; x = tmp, y = x == -1 ? -1 : link[x]) {
                                tmp = match[y];
                                match[x] = y;
                                match[y] = x;
                            }
                            return;
                        }
                        vis[match[v]] = 1;
                        dep[match[v]] = dep[u] + 2;
                        q.push(match[v]);
                    } else if (vis[v] == 1 && find(v) != find(u)) {
                        // found a blossom
                        int p = lca(u, v);
                        blossom(u, v, p);
                        blossom(v, u, p);
                    }
                }
            }
            if (y != -1) {
                for (int x = -1, tmp; y != -1; x = tmp, y = x == -1 ? -1 : link[x]) {
                    tmp = match[y];
                    if (x != -1) {
                        match[x] = y;
                    }
                    match[y] = x;
                }
            }
        };

        // find a maximal matching greedily (decrease constant)
        auto greedy = [&]() {
            for (int u = 0; u < n; ++u) {
                if (match[u] != -1) continue;
                for (auto v : e[u]) {
                    if (match[v] == -1) {
                        match[u] = v;
                        match[v] = u;
                        break;
                    }
                }
            }
        };
        // greedy();

        for (int u = 0; u < m; ++u) {
            if (match[u] == -1) {
                augment(u);
            }
        }
        return match;
    }
};
