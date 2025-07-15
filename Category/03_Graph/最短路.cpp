// dijkstra
// 求解 `非负权重加权有向图` 上 `单源最短路`
// ⚠ 注意存入优先队列的元素顺序


// 0-1 BFS 本质是对 Dijkstra 算法的优化。
// 因为边权只有 0 和 1，我们可以把最小堆换成双端队列，遇到 0 边权就加入队首，遇到 1 边权就加入队尾，
// 这样可以保证队首总是最小的，就不需要最小堆了。


// BellmanFord  可以处理有负权，但不能有负环
//
// 时间复杂度 O(nm)


// SPFA
// 对于 Bellman-Ford 的过程来说，只有上一次被松弛过的节点，所连接的边才可能引起下一次松弛，
// 可以利用队列维护需要进行松弛的节点。

void spfa(int s) {
    vector<int> dis(n, 1E9);
    dis[s] = 0;
    vector<bool> inq(n);
    inq[s] = true;
    vector<int> cnt(n);
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = false;
        for (auto [v, w] : g[u]) {
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                if (inq[v]) {
                    continue;
                }
                q.push(v);
                inq[v] = true;
                if (++cnt[v] > n) { // 有负环
                    return false;
                }
            }
        }
    }
    return true;
}


// Floyd-Warshall
// 求解图中任意两点之间的最短路径
// 适用于任何图，不管有向无向，边权正负，但是最短路必须存在（不能有负环）。
//
// 时间复杂度 O(n^3) 空间复杂度 O(n^2)

auto FloydWarshall(int n, vector<array<int, 3>>& edges) { // 顶点数、边列表
    vector g(n, vector<int>(n, inf));
    for (int i = 0; i < n; i++) {
        g[i][i] = 0;
    }
    for (auto& [u, v, d] : edges) {
        g[u][v] = g[v][u] = min(g[u][v], d);
    }

    // g[k][i][j] 表示「经过若干个编号不超过 k 的中间节点」时，从 i 到 j 的最短路长度，其中第一维可以压缩掉
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (g[i][k] == inf) {
                continue;
            }
            for (int j = 0; j < n; j++) {
                g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
            }
        }
    }

    // 如果出现 g[i][i] < 0 则说明有负环

    return g;
}

// 动态加边
auto addEdge(int from, int to, int dis, vector<vector<int>>& g) {
    int n = g.size();
    if (dis >= g[from][to]) { // 无法让任何最短路变短
        return;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g[i][j] = min(g[i][j], g[i][from] + dis + g[to][j]);
        }
    }
}
