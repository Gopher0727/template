// 求连通分量、割点
//
// 引入：DFS 生成树（无向图）
//     性质：
//         1> T 的根节点是一个割点，当且仅当根节点有两个或更多的子节点
//         2> T 的非根节点 u 是一个割点，当且仅当 u 存在一个子节点 v，且 v 和它的后代都没有回退边连向 u 的祖先
//
void solve() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> g(n + 1);
    for (int i = 1; i <= m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }

    vector<int> low(n + 1), dfn(n + 1);
    vector<int> isCut(n + 1);
    int cnt = 0, root;
    auto tarjan = [&](auto&& self, int u, int pa) -> void {
        low[u] = dfn[u] = ++cnt;
        int child = 0;
        for (int v : g[u]) {
            if (!dfn[v]) {
                child++;
                self(self, v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u] && u != root) {
                    isCut[u] = 1;
                }
            } else if (dfn[v] < dfn[u] && v != pa) {
                low[u] = min(low[u], dfn[v]);
            }

            if (u == root && child >= 2) {
                isCut[root] = 1;
            }
        }
    };

    for (root = 1; root <= n; ++root) {
        if (!dfn[root]) {
            tarjan(tarjan, root, -1);
        }
    }

    vector<int> ans;
    for (int i = 1; i <= n; ++i) {
        if (isCut[i]) {
            ans.emplace_back(i);
        }
    }

    cout << ans.size() << "\n";
    for (int node : ans) {
        cout << node << " ";
    }
}
