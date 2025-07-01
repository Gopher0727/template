struct Edge {
    int to, rev;
    i64 cap;
};

struct Dinic {
    int n;
    vector<vector<Edge>> g;
    vector<int> level;
    vector<int> it;

    explicit Dinic(int n) : n(n) {
        g.assign(n, {});
        level.assign(n, 0);
        it.assign(n, 0);
    }

    void addEdge(int s, int t, i64 cap) {
        g[s].push_back({t, (int) g[t].size(), cap});
        g[t].push_back({s, (int) g[s].size() - 1, 0});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const Edge& e : g[u]) {
                if (level[e.to] < 0 && e.cap > 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    i64 dfs(int u, int t, i64 flow) {
        if (flow == 0) {
            return 0;
        }
        if (u == t) {
            return flow;
        }
        for (int& i = it[u]; i < g[u].size(); i++) {
            Edge& e = g[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                if (i64 pushed = dfs(e.to, t, min(flow, e.cap))) {
                    e.cap -= pushed;
                    g[e.to][e.rev].cap += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    i64 maxFlow(int s, int t) {
        i64 flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (i64 pushed = dfs(s, t, 1e9)) {
                flow += pushed;
            }
        }
        return flow;
    }
};
