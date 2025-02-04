// 最短路径树（Shortest Path Tree, SPT）
//
// 在一张无向带权联通图中，有这样一棵生成树：满足从根节点到任意点的路径都为原图中根到任意点的最短路径。
//
struct Graph {
    int n;
    vector<vector<pair<int, i64>>> g; // u -> (v, w)
    map<pair<int, int>, i64> edge; // (u, v) -> w
    vector<int> vis;
    vector<int> pre; // 记录前驱结点
    vector<i64> dis;
    i64 ans = 0; // 输出 SPT 的边权和

    explicit Graph(int n) { init(n); }

    void init(int n) {
        this->n = n;
        g.resize(n);
        dis.resize(n, inf);
        pre.resize(n);
        vis.resize(n);
    }

    void add(int x, int y, i64 w) {
        g[x].push_back({y, w});
        edge[{x, y}] = w;
    }

    void djikstra(int s) {
        priority_queue<pair<int, i64>, vector<pair<int, i64>>, greater<>> q;
        q.push({0, s});
        dis[s] = 0;
        vector<int> vis(n);
        while (!q.empty()) {
            int x = q.top().second;
            q.pop();
            if (vis[x]) {
                continue;
            }
            vis[x] = 1;
            for (auto [y, w] : g[x]) {
                if (dis[y] >= dis[x] + w) { // ！注意，SPT 这里修改为>=号
                    dis[y] = dis[x] + w;
                    pre[y] = x;
                    q.push({dis[y], y});
                }
            }
        }
    }

    void dfs(int x) {
        vis[x] = 1;
        for (auto [y, w] : g[x]) {
            if (vis[y]) {
                continue;
            }
            if (pre[y] == x) {
                ans += edge[{x, y}];
                dfs(y);
            }
        }
    }

    void work(int n) {
        djikstra(0); // 以 0 为根
        dfs(0); // 以 0 为根
        cout << ans << "\n";
    }
};
