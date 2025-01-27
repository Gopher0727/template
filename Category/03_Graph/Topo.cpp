struct Topo {
    int n;
    vector<vector<int>> g;
    vector<int> inDeg;

    Topo() {}
    Topo(int n) { init(n); }

    void init(int n) {
        g.assign(n, {});
        inDeg.assign(n, 0);
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        inDeg[v]++;
    }

    auto work() {
        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (inDeg[i] == 0) {
                q.emplace(i);
            }
        }

        int cnt = 0; // 用于判断图中是否存在环的计数器
        vector<int> res;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            res.push_back(u);
            for (int& v : g[u]) {
                if (--inDeg[v] == 0) {
                    q.emplace(v);
                }
            }
            cnt++;
        }
        if (cnt != n) {
            cout << -1;
            return;
        }
        // 图无环，存在拓扑序
        int _ = res.size();
        for (int i = 0; i < _; ++i) {
            cout << res[i] << " \n"[i == _ - 1];
        }
    }
};
