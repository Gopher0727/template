// TopoSort + DP
// 计算一张 DAG 中的最长路径，在执行前可能需要使用 dijkstra 重构一张正确的 DAG
//
struct DAG {
    int n;
    vector<vector<pair<int, int>>> g;
    vector<int> deg;
    vector<i64> dis;

public:
    DAG(int n) : n(n), g(n + 1), deg(n + 1), dis(n + 1, -1E18) {}

    void add(int x, int y, int w) {
        g[x].push_back({y, w});
        ++deg[y];
    }

    auto TopoSort(int s, int t) {
        queue<int> q;
        for (int i = 1; i <= n; i++) {
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
