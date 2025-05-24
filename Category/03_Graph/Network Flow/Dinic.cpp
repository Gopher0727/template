struct Edge {
    int to, rev;
    int cap;
};

struct Dinic {
    int n;
    vector<vector<Edge>> graph;
    vector<int> level, it;

    explicit Dinic(int n) : n(n) {
        graph.assign(n, {});
        level.assign(n, 0);
        it.assign(n, 0);
    }

    void addEdge(int s, int t, int cap) {
        Edge a = {t, (int) graph[t].size(), cap};
        Edge b = {s, (int) graph[s].size(), 0};
        graph[s].push_back(a);
        graph[t].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const Edge& e : graph[u]) {
                if (level[e.to] < 0 && e.cap > 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    int dfs(int u, int t, int flow) {
        if (flow == 0) {
            return 0;
        }
        if (u == t) {
            return flow;
        }
        for (int& i = it[u]; i < graph[u].size(); i++) {
            Edge& e = graph[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                int pushed = dfs(e.to, t, min(flow, e.cap));
                if (pushed) {
                    e.cap -= pushed;
                    graph[e.to][e.rev].cap += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    int maxFlow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (int pushed = dfs(s, t, 1e9)) {
                flow += pushed;
            }
        }
        return flow;
    }
};
