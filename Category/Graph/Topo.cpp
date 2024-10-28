int TopoSort() {
    int n, m; // 点数、边数
    cin >> n >> m;

    vector<vector<int>> g(n);
    vector<int> inDeg(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        g[a].push_back(b);
        inDeg[b]++;
    }

    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (inDeg[i] == 0) { // 入度为 0 的点入队
            q.emplace(i);
        }
    }

    int cnt = 0; // 用于判断图中是否存在环的计数器
    vector<int> res;
    while (!q.empty()) {
        int u = q.front();
        res.push_back(u);
        q.pop();
        for (int i = 0; i < g[u].size(); ++i) {
            int v = g[u][i];
            if (--inDeg[v] == 0) { // 若入度减为0，则入队
                q.emplace(v);
            }
        }
        cnt++;
    }
    if (cnt != n) {
        cout << -1;
        return 0;
    }
    // 图无环，存在拓扑序列
    for (int i = 0; i < res.size(); ++i) {
        cout << res[i] << " \n"[i == res.size() - 1];
    }

    return 0;
}
// The node-index starts from 0
