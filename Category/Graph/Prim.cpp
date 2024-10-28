// 无向边
//
void solve() {
    int n, m;
    cin >> n >> m;

    using PII = pair<int, int>;
    vector<vector<PII>> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        u--, v--;
        g[u].emplace_back(v, w);
    }

    ll sum = 0; // 最小生成树的边权和
    int cnt = 0; // 记录的点数
    bool ok = false; // 判断是否有最小生成树

    priority_queue<PII, vector<PII>, greater<PII>> pq; // w, v
    vector<int> vis(n);
    pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [w, v] = pq.top();
        pq.pop();
        if (vis[v] == 1) {
            continue;
        }
        vis[v] = 1;
        cnt++;
        sum += w;
        if (cnt == n) {
            ok = true;
            break;
        }
        for (auto& [to, w] : g[v]) {
            if (vis[to] == 1) {
                continue;
            }
            pq.emplace(w, to);
        }
    }

    if (ok) {
        cout << sum << "\n";
    } else {
        cout << "orz\n";
    }
}
// The node-index starts from 0
