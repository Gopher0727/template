struct BlockCutTree {
    int n;
    vector<vector<int>> adj;
    vector<int> dfn, low, stk;
    vector<pair<int, int>> edges;
    int cnt, cur;

    BlockCutTree() = delete;
    BlockCutTree(int n) { init(n); }

    void init(int n) {
        this->n = n;
        adj.assign(n, {});
        dfn.assign(n, -1);
        low.resize(n);
        stk.clear();
        edges.clear();
        cnt = cur = 0;
    }

    void addEdge(int u, int v) { adj[u].push_back(v); }

    void Tarjan(int x) {
        stk.push_back(x);
        dfn[x] = low[x] = cur++;
        for (auto y : adj[x]) {
            if (dfn[y] == -1) {
                Tarjan(y);
                low[x] = min(low[x], low[y]);
                if (low[y] == dfn[x]) {
                    int v;
                    do {
                        v = stk.back();
                        stk.pop_back();
                        edges.emplace_back(n + cnt, v);
                    } while (v != y);
                    edges.emplace_back(x, n + cnt);
                    cnt++;
                }
            } else {
                low[x] = min(low[x], dfn[y]);
            }
        }
    }

    auto work() {
        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) {
                stk.clear();
                Tarjan(i);
            }
        }
        return pair {cnt, edges};
    }
};
