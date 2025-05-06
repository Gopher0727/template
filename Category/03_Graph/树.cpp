struct Tree {
    int n; // 边数
    vector<vector<int>> g;

public:
    explicit Tree(int n) : n(n), g(n) {}

    void addEdge(int x, int y) {
        g[x].push_back(y);
        g[y].push_back(x);
    }

    // 获取子树大小（包括当前节点并以当前节点为根）
    vector<int> subTreeSize(int x) {
        vector<int> siz(n, 1);
        vector<int> vis(n);
        stack<pair<int, int>> stk;
        stk.push({x, -1}); // 子节点，父节点
        while (!stk.empty()) {
            auto [x, pa] = stk.top();
            if (vis[x] == 0) {
                vis[x] = 1;
                for (int y : g[x]) {
                    if (y != pa && vis[y] == 0) {
                        stk.push({y, x});
                    }
                }
            } else { // 当前节点及其子树已经完全访问，处理子树大小
                for (int y : g[x]) {
                    if (y != pa) {
                        siz[x] += siz[y];
                    }
                }
                stk.pop();
            }
        }
        return siz;
    }

    // DFS: 树上两点路径
    auto path(int x, int y) {
        vector<int> path;
        auto f = [&](this auto&& f, int u, int pa) -> bool {
            if (u == x) {
                path.push_back(u);
                return true;
            }
            for (int v : g[u]) {
                if (v != pa && f(v, u)) {
                    path.push_back(u);
                    return true;
                }
            }
            return false;
        };
        f(y, -1);
        return path;
    }

    // 预处理从 v 到 w 走一步的节点 move[v][w]
    // 定义：v 到 v 走一步的节点为 v
    auto move() {
        vector move(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            auto build = [&](this auto&& build, int u, int pa) -> void {
                move[u][i] = pa;
                for (int v : g[u]) {
                    if (v != pa) {
                        build(v, u);
                    }
                }
            };
            build(i, i);
        }
        return move;
    }
};
// 默认：无向图，点下标从 0 开始



//// 树的直径：树上任意两点间的最大距离，也就是最长的简单路径。
//
// > 两次 DFS / BFS：
// （※）考虑边权时，要求无负权边
// 首先从任意节点 y 开始，到达距离其最远的节点，记为 z，
// 然后再从 z 开始，到达距离 z 最远的节点，记为 z'，则 \delta(z,z') 即为树的直径。
//
// 如果需要求出一条直径上所有的节点，则可以记录每个点的前序节点，从直径的一端一路向前，遍历直径上所有的节点。
//
// 【Note】在一棵树上，从任意节点 y 开始，到达的距离其最远的节点 z 必为直径的一端。
// 【Note】若树上所有边边权均为正，则树的所有直径中点重合。

auto work = [&](int u) {
    vector<int> dis(n), pa(n);
    int far = u;

    auto dfs = [&](this auto&& dfs, int u) -> void {
        for (int v : g[u]) {
            if (v == pa[u]) {
                continue;
            }
            pa[v] = u;
            dis[v] = dis[u] + 1;
            dfs(v);

            if (dis[v] > dis[far]) {
                far = v;
            }
        }
    };

    dis[u] = 0;
    pa[u] = -1;
    dfs(u);

    int r = far;

    dis[r] = 0;
    pa[r] = -1;
    dfs(r);

    int s = far;

    vector<int> path;
    for (int i = s; i != -1; i = pa[i]) {
        path.push_back(i);
    }

    return array {r, s};
};


// > 树形 DP：
// （※）可以在存在负权边的情况下求解出树的直径（只得到直径的长度）
// 记录当 1 为树的根时，每个节点作为子树的根向下，所能延伸的最长路径长度 d1 与次长路径（与最长路径无公共边）长度 d2，
// 那么直径就是对于每一个点，该点 d1 + d2 能取到的值中的最大值。
// 如果需要求出一条直径上所有的节点，则可以在 DP 的过程中，记录下每个节点能向下延伸的最长路径与次长路径所对应的子节点，
// 在求 d 的同时记下对应的节点 u，使得 d = d1[u] + d2[u]，即可分别沿着从 u 开始的最长路径的次长路径对应的子节点一路向某个方向
// （对于无根树，虽然这里指定了 1 为树的根，但仍需记录每点跳转的方向；对于有根树，一路向上跳即可），遍历直径上所有的节点。
//
// 我们定义 dp[u]：以 u 为根的子树中，从 u 出发的最长路径。那么容易得出转移方程：dp[u] = max(dp[u], dp[v]+w(u,v))，其中的 v
// 为 u 的子节点，w(u, v) 表示所经过边的权重。对于树的直径，实际上是可以通过枚举从某个节点出发不同的两条路径相加的最大值求出。
// 因此，在 DP 求解的过程中，我们只需要在更新 dp[u] 之前，计算 d = max(d, dp[u]+dp[v]+w(u,v)) 即可算出直径 d。



//// 重心（centroid / cog，Center of gravity）
//
// > 以重心节点为根，最大子树的节点数最少
// > 以重心节点为根，每个子树的节点数不超过总节点数的一半
// > 以重心节点为根，所有节点都走向重心的总边数最少
//
// 【Note】重心最多有两个（此时重心相邻）
// 【Note】拥有奇数个节点的树只有一个重心
// 【Note】如果树上增加一个叶节点，转移后的重心最多移动一条边
// 【Note】如果把两棵树连起来，新树的重心一定在原来两棵树重心的路径上
// 【Note】如果树上边权非负，不论边权如何分布，所有节点都走向重心的总距离和最小
// 【Note】树的重心一定在根节点的重链上
// 【Note】树的重心一定在它重儿子的重心到根节点的路径上


// 获取树的重心（1）
// · 返回某一个重心
vector<int> siz(n);
int cog = -1, maxSize = 1E9;
auto dfs = [&](this auto&& dfs, int u, int pa) -> void {
    siz[u] = 1;
    int res = 0;
    for (int v : g[u]) {
        if (v == pa) {
            continue;
        }
        dfs(v, u);
        siz[u] += siz[v];
        res = max(res, siz[v]);
    }
    res = max(res, n - siz[u]);
    if (res < maxSize) {
        maxSize = res;
        cog = u;
    }
};
dfs(0, -1);


// 获取树的重心（2）
// · 返回所有重心
vector<int> siz(n), maxSize(n), cog;
auto dfs = [&](this auto&& dfs, int u, int pa) -> void {
    siz[u] = 1;
    int& res = maxSize[u];
    for (int v : g[u]) {
        if (v == pa) {
            continue;
        }
        dfs(v, u);
        siz[u] += siz[v];
        res = max(res, siz[v]);
    }
    res = max(res, n - siz[u]);
};
dfs(0, -1);
for (int i = 0; i < n; i++) {
    if (maxSize[i] <= n / 2) {
        cog.push_back(i);
    }
}

// 也可以用下面的方法查找单个重心，此时 dfs 只需要统计 siz 信息
auto find = [&](this auto&& find, int u, int pa) -> int {
    for (auto v : g[u]) {
        if (v != pa && 2 * siz[v] >= n) {
            return find(v, u);
        }
    }
    return u;
};


// 点分治的一般过程是：
// 取重心为新树的根，随后使用 dfs 处理当前这棵树，灵活运用 child 和 pre 两个数组
// 分别计算通过根节点、不通过根节点的路径信息，根据需要进行答案的更新；
// 再对子树分治，寻找子树的重心。


// 树上倍增
// 基础，无权图
struct Tree {
    int n;
    vector<vector<int>> g, st;
    vector<int> lg, dep;

    Tree(int n) {
        this->n = n;
        g.resize(n + 1);
        st.resize(n + 1, vector<int>(30));
        lg.resize(n + 1);
        dep.resize(n + 1);
        for (int i = 1; i <= n; i++) { // 预处理 log
            lg[i] = lg[i - 1] + (1 << lg[i - 1] == i);
        }
    }

    void add(int x, int y) { // 建立双向边
        g[x].push_back(y);
        g[y].push_back(x);
    }

    void dfs(int x, int fa) {
        st[x][0] = fa; // 储存 x 的父节点
        dep[x] = dep[fa] + 1;
        for (int i = 1; i <= lg[dep[x]]; i++) {
            st[x][i] = st[st[x][i - 1]][i - 1];
        }
        for (auto y : g[x]) {
            if (y == fa) {
                continue;
            }
            dfs(y, x);
        }
    }

    int lca(int x, int y) {
        if (dep[x] < dep[y]) {
            swap(x, y);
        }
        while (dep[x] > dep[y]) {
            x = st[x][lg[dep[x] - dep[y]] - 1];
        }
        if (x == y) {
            return x;
        }
        for (int k = lg[dep[x]] - 1; k >= 0; k--) {
            if (st[x][k] == st[y][k]) {
                continue;
            }
            x = st[x][k];
            y = st[y][k];
        }
        return st[x][0];
    }

    int calc(int x, int y) { // 倍增查询两点间距离
        return dep[x] + dep[y] - 2 * dep[lca(x, y)];
    }

    void work(int root = 1) { // 在此初始化
        dfs(root, 0);
    }

    // 树上路径交
    // 计算两条路径的交点数量，直接载入任意 LCA 封装即可。
    int intersection(int x, int y, int u, int v) {
        vector<int> t = {lca(x, u), lca(x, v), lca(y, u), lca(y, v)};
        sort(t.begin(), t.end());
        int p0 = lca(x, y), p1 = lca(u, v);
        if (dep[t[0]] < min(dep[p0], dep[p1]) || dep[t[2]] < max(dep[p0], dep[p1])) {
            return 0;
        }
        return 1 + calc(t[2], t[3]);
    }
};


// 扩展，带权图，支持“倍增查询两点路径上的最大边权”
struct Tree {
    int n;
    vector<vector<int>> st, Max;
    vector<vector<pair<int, int>>> g;
    vector<int> lg, dep;

    Tree(int n) {
        this->n = n;
        g.resize(n + 1);
        st.resize(n + 1, vector<int>(30));
        Max.resize(n + 1, vector<int>(30));
        lg.resize(n + 1);
        dep.resize(n + 1);
        for (int i = 1; i <= n; i++) { // 预处理 log
            lg[i] = lg[i - 1] + (1 << lg[i - 1] == i);
        }
    }

    void add(int x, int y, int w) { // 建立双向边
        g[x].push_back({y, w});
        g[y].push_back({x, w});
    }

    void dfs(int x, int fa) {
        st[x][0] = fa;
        dep[x] = dep[fa] + 1;
        for (int i = 1; i <= lg[dep[x]]; i++) {
            st[x][i] = st[st[x][i - 1]][i - 1];
            Max[x][i] = max(Max[x][i - 1], Max[st[x][i - 1]][i - 1]);
        }
        for (auto [y, w] : g[x]) {
            if (y == fa) {
                continue;
            }
            Max[y][0] = w;
            dfs(y, x);
        }
    }

    int lca(int x, int y) {
        if (dep[x] < dep[y]) {
            swap(x, y);
        }
        while (dep[x] > dep[y]) {
            x = st[x][lg[dep[x] - dep[y]] - 1];
        }
        if (x == y) {
            return x;
        }
        for (int k = lg[dep[x]] - 1; k >= 0; k--) {
            if (st[x][k] == st[y][k]) {
                continue;
            }
            x = st[x][k];
            y = st[y][k];
        }
        return st[x][0];
    }

    int calc(int x, int y) { // 倍增查询两点间距离
        return dep[x] + dep[y] - 2 * dep[lca(x, y)];
    }

    int query(int x, int y) { // 倍增查询两点路径上的最大边权（带权图）
        auto get = [&](int x, int y) -> int {
            int ans = 0;
            if (x == y) {
                return ans;
            }
            for (int i = lg[dep[x]]; i >= 0; i--) {
                if (dep[st[x][i]] > dep[y]) {
                    ans = max(ans, Max[x][i]);
                    x = st[x][i];
                }
            }
            ans = max(ans, Max[x][0]);
            return ans;
        };
        int fa = lca(x, y);
        return max(get(x, fa), get(y, fa));
    }

    void work(int root = 1) { // 在此初始化
        dfs(root, 0);
    }

    // 树上路径交
    // 计算两条路径的交点数量，直接载入任意 LCA 封装即可。
    int intersection(int x, int y, int u, int v) {
        vector<int> t = {lca(x, u), lca(x, v), lca(y, u), lca(y, v)};
        sort(t.begin(), t.end());
        int p0 = lca(x, y), p1 = lca(u, v);
        if (dep[t[0]] < min(dep[p0], dep[p1]) || dep[t[2]] < max(dep[p0], dep[p1])) {
            return 0;
        }
        return 1 + calc(t[2], t[3]);
    }
};
