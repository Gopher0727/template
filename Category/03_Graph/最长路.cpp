// TopoSort + DP
//
// 计算一张 DAG 中的最长路径，在执行前可能需要使用 dijkstra 重构一张正确的 DAG
//
struct DAG {
    int n;
    vector<vector<pair<int, i64>>> g;
    vector<int> deg;
    vector<i64> dis;

public:
    DAG() = delete;
    DAG(int n) { init(n); }

    void init(int n) {
        this->n = n;
        g.assign(n, {});
        deg.assign(n, 0);
        dis.assign(n, -1E18);
    }

    void addEdge(int x, int y, i64 w) {
        g[x].push_back({y, w});
        ++deg[y];
    }

    auto TopoSort(int s, int t) {
        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (deg[i] == 0) {
                q.push(i);
            }
        }
        dis[s] = 0;
        while (!q.empty()) {
            int x = q.front();
            q.pop();
            for (auto [y, w] : g[x]) {
                dis[y] = max(dis[y], dis[x] + w);
                --deg[y];
                if (deg[y] == 0) {
                    q.push(y);
                }
            }
        }
        return dis[t];
    }
};
