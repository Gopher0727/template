struct Topo {
    vector<vector<int>> adj;
    vector<int> inDeg;

    Topo() = delete;
    Topo(int n) { init(n); }

    void init(int n) {
        adj.assign(n, {});
        inDeg.assign(n, 0);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        inDeg[v]++;
    }

    auto work() {
        queue<int> q;
        for (int i = 0; i < adj.size(); ++i) {
            if (inDeg[i] == 0) {
                q.emplace(i);
            }
        }

        vector<int> res;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            res.push_back(u);
            for (int& v : adj[u]) {
                if (--inDeg[v] == 0) {
                    q.emplace(v);
                }
            }
        }
        return pair {res, res.size() == adj.size()};
    }
};
