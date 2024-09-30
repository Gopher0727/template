/*
    求解 `非负权重加权有向图` 上 `单源最短路`

    n: 点的个数，m: 有向边个数，s: 出发点编号 (点的编号从 1 开始)
*/

// 堆优化 稀疏图
vector<ll> dis(n + 1, LLONG_MAX / 2);
auto dijkstra = [&](int s = 1) {
    dis[s] = 0;

    using PLL = pair<ll, ll>;
    priority_queue<PLL, vector<PLL>, greater<PLL>> pq;
    pq.emplace(0, s); // dis[k], k
    while (!pq.empty()) {
        auto [d, cur] = pq.top();
        pq.pop();

        if (d > dis[cur]) {
            continue;
        }
        for (auto& [to, w] : g[cur]) {
            if (dis[to] > d + w) {
                dis[to] = d + w;
                pq.emplace(dis[to], to);
            }
        }
    }
    return 0;
}();

// 朴素 稠密图
vector<ll> dis(n + 1, LLONG_MAX / 2);
vector<int> vis(n + 1);
auto plain_dijkstra = [&](int s = 1) {
    dis[s] = 0;
    for (int i = 1; i < n; ++i) {
        int cur = 0;
        for (int j = 1; j <= n; ++j) {
            if (vis[j] == 0 && dis[j] < dis[cur]) {
                cur = j;
            }
        }
        vis[cur] = 1;

        for (auto& [to, w] : g[cur]) {
            if (dis[to] > dis[cur] + w) {
                dis[to] = dis[cur] + w;
            }
        }
    }
    return 0;
}();

/*
struct Node {
    int to, w;
};

void solve() {
    int n, m, s;
    cin >> n >> m >> s;

    vector<vector<Node>> g(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
    }

    // plain_dijkstra(s);
    dijkstra(s);

    for (int i = 1; i <= n; ++i) {
        cout << dis[i] << " ";
    }
}
*/
