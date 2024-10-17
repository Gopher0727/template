/*
    BellmanFord  可以处理有负权，但不能有负环

    时间复杂度 O(nm)
*/
static constexpr int inf = INT_MAX / 2;

void solve() {
    int n, m, s;
    cin >> n >> m >> s;

    vector<int> x(m + 1), y(m + 1), w(m + 1);
    for (int i = 1; i <= m; ++i) {
        cin >> x[i] >> y[i] >> w[i];
    }

    vector<int> d(n + 1, inf);
    auto BellmanFord = [&]() {
        d[s] = 0;
        for (int k = 1; k < n; ++k) {
            for (int i = 1; i <= m; ++i) {
                d[y[i]] = min(d[y[i]], d[x[i]] + w[i]);
            }
        }
        return 0;
    }();

    for (int i = 1; i <= n; ++i) {
        cout << d[i] << " ";
    }
    cout << "\n";
}


/*
    SPFA

    对于 Bellman-Ford 的过程来说，只有上一次被松弛过的节点，所连接的边才可能引起下一次松弛，
    可以利用队列维护需要进行松弛的节点。
*/
static constexpr int inf = INT_MAX / 2;

void solve {
    int n, m, s;
    cin >> n >> m >> s;

    vector<vector<pair<int, int>>> g(n + 1);
    for (int i = 1; i <= m; ++i) {
        int x, y, w;
        cin >> x >> y >> w;
        g[x].emplace_back(y, w);
    }

    vector<int> inq(n + 1), cnt(n + 1), dis(n + 1, inf);
    auto spfa = [&]() {
        queue<int> q;
        q.emplace(s);
        dis[s] = 0, inq[s] = 1;
        while (!q.empty()) {
            int x = q.front();
            q.pop();
            inq[x] = 0;
            for (auto [y, w] : g[x]) {
                if (dis[y] > dis[x] + w) {
                    dis[y] = dis[x] + w;
                    if (inq[y]) {
                        continue;
                    }
                    q.emplace(y);
                    inq[y] = 1;
                    if (++cnt[y] > n) {
                        return false;
                    }
                }
            }
        }
        return true;
    }();

    for (int i = 1; i <= n; ++i) {
        cout << dis[i] << " ";
    }
    cout << "\n";
}
