// dijkstra
// 求解 `非负权重加权有向图` 上 `单源最短路`

/*
struct Node {
    int to, w;
};

void solve() {
    int n, m, s;
    cin >> n >> m >> s;

    vector<vector<Node>> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        u--, v--;
        g[u].emplace_back(v, w);
    }
}
*/

// 堆优化 稀疏图
vector<ll> dis(n + 1, LLONG_MAX / 2);
auto dijkstra = [&](int s = 0) {
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
// The node-index starts from 0

// 朴素 稠密图
vector<ll> dis(n + 1, LLONG_MAX / 2);
vector<int> vis(n + 1);
auto plain_dijkstra = [&](int s = 0) {
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
// The node-index starts from 0


// 0-1 BFS 本质是对 Dijkstra 算法的优化。
// 因为边权只有 0 和 1，我们可以把最小堆换成双端队列，遇到 0 边权就加入队首，遇到 1 边权就加入队尾，
// 这样可以保证队首总是最小的，就不需要最小堆了。
//
static constexpr int inf = INT_MAX / 2;

void solve() {
    int n = grid.size(), m = grid[0].size();

    vector dis(n, vector<int>(m, inf));
    dis[0][0] = grid[0][0];

    deque<pair<int, int>> q;
    q.emplace_front(0, 0);

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop_front();

        for (auto& [dx, dy] : dir) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                int g = grid[nx][ny];
                if (dis[x][y] + g < dis[nx][ny]) {
                    dis[nx][ny] = dis[x][y] + g;
                    g == 0 ? q.emplace_front(nx, ny) : q.emplace_back(nx, ny);
                }
            }
        }
    }
}
