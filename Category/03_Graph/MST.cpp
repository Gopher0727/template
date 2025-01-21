// 最小生成树（MST, Minimum spanning tree）
//

// kruskal 能够处理有向边或者无向边，适用于稀疏图
//
// 结构体存边，对边进行排序，并初始化一个并查集，当 "边数 = 点数 - 1" 时停止。
//
struct Tree {
    int n;
    using TLII = tuple<ll, int, int>;
    priority_queue<TLII, vector<TLII>, greater<>> pq;

public:
    Tree(int n) : n(n) {}

    void add(int u, int v, ll w) { pq.emplace(w, u, v); }

    auto kruskal() {
        DSU dsu(n);

        ll ans = 0;
        int cnt = 0;
        while (!pq.empty()) {
            auto [w, u, v] = pq.top();
            pq.pop();
            if (dsu.same(u, v)) {
                continue;
            }
            dsu.merge(u, v);
            ans += w;
            cnt++;
        }
        return pair(ans, cnt == n - 1);
    }
};


// Prim  无向边，适合于稠密图
//
struct Tree {
    int n;
    vector<vector<pair<ll, int>>> g;

public:
    Tree(int n) : n(n), g(n) {}

    void add(int u, int v, ll w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    auto prim() {
        ll ans = 0;
        int cnt = 0;
        vector<int> vis(n);
        using PLI = pair<ll, int>;
        priority_queue<PLI, vector<PLI>, greater<>> pq;
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
