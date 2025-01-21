// SPT 问题
//
// 在一张无向带权联通图中，有这样一棵生成树：满足从根节点到任意点的路径都为原图中
// 根到任意点的最短路径。
//
// 性质：记根节点 root 到某一结点 x 的最短距离 dis，在 SPT 上这两点之间的距离为 len，则两者长度相等。
//
static constexpr int inf = numeric_limits<int>::max() / 2;

using PII = pair<int, int>;
map<PII, int> id;
namespace G {
    vector<vector<PII>> ver;
    map<PII, int> edge;
    vector<int> v, d, pre, vis;
    int ans = 0;

    auto init = []() {
        ver.resize(N + 1);
        v.resize(N + 1);
        d.resize(N + 1, inf);
        pre.resize(N + 1);
        vis.resize(N + 1);
        return 0;
    }();

    void add(int x, int y, int w) {
        ver[x].push_back({y, w});
        edge[{x, y}] = edge[{y, x}] = w;
    }

    void djikstra(int s) { // ！注意，该 djikstra 并非原版，多加了一个等于号
        priority_queue<PII, vector<PII>, greater<PII>> q;
        q.push({0, s});
        d[s] = 0;
        while (!q.empty()) {
            int x = q.top().second;
            q.pop();
            if (v[x]) {
                continue;
            }
            v[x] = 1;
            for (auto [y, w] : ver[x]) {
                if (d[y] >= d[x] + w) { // ！注意，SPT 这里修改为>=号
                    d[y] = d[x] + w;
                    pre[y] = x; // 记录前驱结点
                    q.push({d[y], y});
                }
            }
        }
    }

    void dfs(int x) {
        vis[x] = 1;
        for (auto [y, w] : ver[x]) {
            if (vis[y]) {
                continue;
            }
            if (pre[y] == x) {
                cout << id[{x, y}] << " "; // 输出SPT所使用的边编号
                ans += edge[{x, y}];
                dfs(y);
            }
        }
    }

    void solve(int n) {
        djikstra(1); // 以 1 为根
        dfs(1); // 以 1 为根
        cout << endl << ans; // 输出SPT的边权和
    }
} // namespace G

void Solve() {
    int n, m;
    cin >> n >> m;

    for (int i = 1; i <= m; ++i) {
        int x, y, w;
        cin >> x >> y >> w;
        G::add(x, y, w), G::add(y, x, w);
        id[{x, y}] = id[{y, x}] = i;
    }
    G::solve(n);
}
