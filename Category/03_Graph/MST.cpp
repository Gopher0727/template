// 最小生成树（MST, Minimum spanning tree）
//
// 无向边

// kruskal 适用于稀疏图

struct Tree {
    int n;
    vector<tuple<i64, int, int>> edges;

    explicit Tree(int n) : n(n) { edges.reserve(n); }

    void addEdge(int u, int v, i64 w) { edges.push_back({w, u, v}); }

    auto kruskal() {
        DSU dsu(n);
        i64 ans = 0;
        int cnt = 0;
        ranges::sort(edges);
        for (auto& [w, u, v] : edges) {
            if (dsu.same(u, v)) {
                continue;
            }
            dsu.merge(u, v);
            ans += w;
            cnt++;
        }
        return pair {ans, cnt == n - 1};
    }
};


// Prim 适合于稠密图

struct Tree {
    int n;
    vector<vector<pair<int, i64>>> g;

    Tree() = delete;
    Tree(int n) : n(n), g(n) {}

    void addEdge(int u, int v, i64 w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    auto prim() {
        i64 ans = 0;
        int cnt = 0;
        vector<int> vis(n);
        priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<>> pq;
        pq.push({0, 0});
        while (!pq.empty()) {
            auto [w, v] = pq.top();
            pq.pop();
            if (vis[v] == 1) {
                continue;
            }
            vis[v] = 1;
            ans += w;
            cnt++;
            for (auto& [to, w] : g[v]) {
                if (vis[to] == 1) {
                    continue;
                }
                pq.push({w, to});
            }
        }
        return pair(ans, cnt == n);
    }
};
