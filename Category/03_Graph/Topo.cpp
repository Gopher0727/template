// Problems:
//
//
struct Topo {
    vector<vector<int>> g;
    vector<int> inDeg;

    Topo() = delete;
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
        for (int i = 0; i < g.size(); ++i) {
            if (inDeg[i] == 0) {
                q.emplace(i);
            }
        }

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
        }
        return pair {res, res.size() == g.size()};
    }
};
