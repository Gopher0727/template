// 【彻底掌握直径 DP！从二叉树到一般树！】(https://leetcode.cn/problems/diameter-of-binary-tree/solutions/2227017/shi-pin-che-di-zhang-wo-zhi-jing-dpcong-taqma/)
// https://oi-wiki.org/graph/

struct Tree {
    int n; // 边数
    vector<vector<int>> g;

public:
    Tree(int n) : n(n), g(n) {}

    void addEdge(int x, int y) {
        g[x].push_back(y);
        g[y].push_back(x);
    }

    // 获取子树大小（包括当前节点并以当前节点为根）
    vector<int> subTreeSize(int x) {
        vector<int> _size(n, 1);
        vector<int> vis(n);
        stack<pair<int, int>> stk;
        stk.emplace(x, -1); // 子节点，父节点
        while (!stk.empty()) {
            auto [x, pa] = stk.top();
            if (vis[x] == 0) {
                vis[x] = 1;
                for (int y : g[x]) {
                    if (y != pa && vis[y] == 0) {
                        stk.emplace(y, x);
                    }
                }
            } else { // 当前节点及其子树已经完全访问，处理子树大小
                for (int y : g[x]) {
                    if (y != pa) {
                        _size[x] += _size[y];
                    }
                }
                stk.pop();
            }
        }
        return _size;
    }

    // DFS: 树上两点路径
    auto path(int st, int end) {
        vector<int> path;
        auto f = [&](auto&& f, int u, int pa) {
            if (u == st) {
                path.push_back(u);
                return true;
            }
            for (int v : g[u]) {
                if (v != pa && f(f, v, u)) {
                    path.push_back(u);
                    return true;
                }
            }
            return false;
        };
        f(f, end, -1);
        return path;
    }

    // 预处理从 v 到 w 走一步的节点 move[v][w]
    // 定义：v 到 v 走一步的节点为 v
    auto move() {
        vector move(n, vector<int>(n));
        for (int i = 0; i < n; ++i) {
            auto build = [&](auto&& build, int u, int pa) -> void {
                move[u][i] = pa;
                for (int v : g[u]) {
                    if (v != pa) {
                        build(build, v, u);
                    }
                }
            };
            build(build, i, i);
        }
        return move;
    }
};
// 默认：无向图，点下标从 0 开始


/*

树的直径：树上任意两点间的最大距离，也就是最长的简单路径。

1> 两次 DFS：考虑边权时，要求无负权边

    首先从任意节点 y 开始进行第一次 DFS，到达距离其最远的节点，记为 z，然后再从 z 开始做第二次 DFS，到达距离 z 最远的节点，
    记为 z'，则 \delta(z,z') 即为树的直径。

(※) 在一棵树上，从任意节点 y 开始进行一次 DFS，到达的距离其最远的节点 z 必为直径的一端。

如果需要求出一条直径上所有的节点，则可以在第二次 DFS 的过程中，记录每个点的前序节点，即可从直径的一端一路向前，遍历直径上所有的节点。

2> 树形 DP：可以在存在负权边的情况下求解出树的直径

    记录当 1 为树的根时，每个节点作为子树的根向下，所能延伸的最长路径长度 d1 与次长路径（与最长路径无公共边）长度 d2，
    那么直径就是对于每一个点，该点 d1 + d2 能取到的值中的最大值。

如果需要求出一条直径上所有的节点，则可以在 DP 的过程中，记录下每个节点能向下延伸的最长路径与次长路径（定义同上）所对应的子节点，
在求 d 的同时记下对应的节点 u，使得 d = d1[u] + d2[u]，即可分别沿着从 u 开始的最长路径的次长路径对应的子节点一路向某个方向
（对于无根树，虽然这里指定了 1 为树的根，但仍需记录每点跳转的方向；对于有根树，一路向上跳即可），遍历直径上所有的节点。

我们定义 dp[u]：以 u 为根的子树中，从 u 出发的最长路径。那么容易得出转移方程：dp[u] = max(dp[u], dp[v] + w(u, v))，其中的 v
为 u 的子节点，w(u, v) 表示所经过边的权重。对于树的直径，实际上是可以通过枚举从某个节点出发不同的两条路径相加的最大值求出。
因此，在 DP 求解的过程中，我们只需要在更新 dp[u] 之前，计算 d = max(d, dp[u] + dp[v] + w(u, v)) 即可算出直径 d。


链：
   从子树中的叶子节点到当前节点的路径。把最长链的长度，作为 dfs 的返回值。根据这一定义，空节点的链长是 −1，叶子节点的链长是 0。
直径：
   等价于由两条（或者一条）链拼成的路径。我们枚举每个 node，假设直径在这里「拐弯」，也就是计算由左右两条从下面的叶子节点
   到 node 的链的节点值之和，去更新答案的最大值。

*/

// 二叉树的直径
int diameterOfBinaryTree(TreeNode* root) {
    int ans = 0;
    // ⚠注意：dfs 返回的是链的长度，不是直径的长度。
    // ⚠注意：dfs 返回的是当前子树的最大链长（也可以理解为子树的高度），不包含当前节点和其父节点的这条边。
    auto dfs = [&](this auto&& dfs, TreeNode* node) {
        if (node == nullptr) {
            return -1;
        }
        int L = dfs(node->left) + 1;
        int R = dfs(node->right) + 1;
        ans = max(ans, L + R);
        return max(L, R);
    };
    dfs(root);
    return ans;
}

//
// 性质：
//    1> 若树上所有边边权均为正，则树的所有直径中点重合。
//



// 树的直径、重心、中心
struct Tree {
    int n;
    vector<vector<pair<int, int>>> e;
    vector<int> dep, parent, maxdep, d1, d2, s1, s2, up;

    Tree(int n) {
        this->n = n;
        e.resize(n + 1);
        dep.resize(n + 1);
        parent.resize(n + 1);
        maxdep.resize(n + 1);
        d1.resize(n + 1);
        d2.resize(n + 1);
        s1.resize(n + 1);
        s2.resize(n + 1);
        up.resize(n + 1);
    }

    void add(int u, int v, int w) {
        e[u].push_back({w, v});
        e[v].push_back({w, u});
    }

    void dfs(int u, int pa) {
        maxdep[u] = dep[u];
        for (auto [w, v] : e[u]) {
            if (v == pa) {
                continue;
            }
            dep[v] = dep[u] + 1;
            parent[v] = u;
            dfs(v, u);
            maxdep[u] = max(maxdep[u], maxdep[v]);
        }
    }
    void dfs1(int u, int pa) {
        for (auto [w, v] : e[u]) {
            if (v == pa) {
                continue;
            }
            dfs1(v, u);
            int x = d1[v] + w;
            if (x > d1[u]) {
                d2[u] = d1[u], s2[u] = s1[u];
                d1[u] = x, s1[u] = v;
            } else if (x > d2[u]) {
                d2[u] = x, s2[u] = v;
            }
        }
    }
    void dfs2(int u, int pa) {
        for (auto [w, v] : e[u]) {
            if (v == pa) {
                continue;
            }
            if (s1[u] == v) {
                up[v] = max(up[u], d2[u]) + w;
            } else {
                up[v] = max(up[u], d1[u]) + w;
            }
            dfs2(v, u);
        }
    }

    int radius, center, diam;
    void getCenter() {
        center = 1; // 中心
        for (int i = 1; i <= n; i++) {
            if (max(d1[i], up[i]) < max(d1[center], up[center])) {
                center = i;
            }
        }
        radius = max(d1[center], up[center]); // 距离最远点的距离的最小值
        diam = d1[center] + up[center] + 1; // 直径
    }

    int rem; // 删除重心后剩余连通块体积的最小值
    int cog; // 重心
    vector<bool> vis;
    void getCog() {
        vis.resize(n);
        rem = INT_MAX;
        cog = 1;
        dfsCog(1);
    }
    int dfsCog(int u) {
        vis[u] = true;
        int s = 1, res = 0;
        for (auto [w, v] : e[u]) {
            if (vis[v]) continue;
            int t = dfsCog(v);
            res = max(res, t);
            s += t;
        }
        res = max(res, n - s);
        if (res < rem) {
            rem = res;
            cog = u;
        }
        return s;
    }
};


// 重心：
// 删除树上的某一个点，会得到若干棵子树；删除某点后，得到的最大子树最小，这个点称为重心。
// 我们假设某个点是重心，记录此时最大子树的最小值，遍历完所有点后取最大值即可。
// 【Note】重心最多有两个（此时重心相邻）
//
// 点分治的一般过程是：
// 取重心为新树的根，随后使用 dfs 处理当前这棵树，灵活运用 child 和 pre 两个数组
// 分别计算通过根节点、不通过根节点的路径信息，根据需要进行答案的更新；
// 再对子树分治，寻找子树的重心。
//
int root = 0, MaxTree = 1e18; // 分别代表重心下标、最大子树大小
vector<int> vis(n + 1), siz(n + 1);
auto get = [&](auto self, int x, int fa, int n) -> void { // 获取树的重心
    siz[x] = 1;
    int val = 0;
    for (auto [y, w] : ver[x]) {
        if (y == fa || vis[y]) continue;
        self(self, y, x, n);
        siz[x] += siz[y];
        val = max(val, siz[y]);
    }
    val = max(val, n - siz[x]);
    if (val < MaxTree) {
        MaxTree = val;
        root = x;
    }
};
auto calc = [&](int x) -> void { // 以 x 为新的根，维护询问
    set<int> pre = {0}; // 记录到根节点 x 距离为 i 的路径是否存在
    vector<int> dis(n + 1);
    for (auto [y, w] : ver[x]) {
        if (vis[y]) continue;
        vector<int> child; // 记录 x 的子树节点的深度信息
        auto dfs = [&](auto self, int x, int fa) -> void {
            child.push_back(dis[x]);
            for (auto [y, w] : ver[x]) {
                if (y == fa || vis[y]) continue;
                dis[y] = dis[x] + w;
                self(self, y, x);
            }
        };
        dis[y] = w;
        dfs(dfs, y, x);
        for (auto it : child) {
            for (int i = 1; i <= m; i++) { // 根据询问更新值
                if (q[i] < it || !pre.count(q[i] - it)) continue;
                ans[i] = 1;
            }
        }
        pre.insert(child.begin(), child.end());
    }
};
auto dfz = [&](auto self, int x, int fa) -> void { // 点分治
    vis[x] = 1; // 标记已经被更新过的旧重心，确保只对子树分治
    calc(x);
    for (auto [y, w] : ver[x]) {
        if (y == fa || vis[y]) continue;
        MaxTree = 1e18;
        get(get, y, x, siz[y]);
        self(self, root, x);
    }
};
get(get, 1, 0, n);
dfz(dfz, root, 0);


// 树上倍增
// 基础，无权图
struct Tree {
    int n;
    vector<vector<int>> ver, val;
    vector<int> lg, dep;
    Tree(int n) {
        this->n = n;
        ver.resize(n + 1);
        val.resize(n + 1, vector<int>(30));
        lg.resize(n + 1);
        dep.resize(n + 1);
        for (int i = 1; i <= n; i++) { // 预处理 log
            lg[i] = lg[i - 1] + (1 << lg[i - 1] == i);
        }
    }
    void add(int x, int y) { // 建立双向边
        ver[x].push_back(y);
        ver[y].push_back(x);
    }
    void dfs(int x, int fa) {
        val[x][0] = fa; // 储存 x 的父节点
        dep[x] = dep[fa] + 1;
        for (int i = 1; i <= lg[dep[x]]; i++) {
            val[x][i] = val[val[x][i - 1]][i - 1];
        }
        for (auto y : ver[x]) {
            if (y == fa) continue;
            dfs(y, x);
        }
    }
    int lca(int x, int y) {
        if (dep[x] < dep[y]) swap(x, y);
        while (dep[x] > dep[y]) {
            x = val[x][lg[dep[x] - dep[y]] - 1];
        }
        if (x == y) return x;
        for (int k = lg[dep[x]] - 1; k >= 0; k--) {
            if (val[x][k] == val[y][k]) continue;
            x = val[x][k];
            y = val[y][k];
        }
        return val[x][0];
    }
    int calc(int x, int y) { // 倍增查询两点间距离
        return dep[x] + dep[y] - 2 * dep[lca(x, y)];
    }
    void work(int root = 1) { // 在此初始化
        dfs(root, 0);
    }
};

// 扩展，带权图，支持“倍增查询两点路径上的最大边权”
struct Tree {
    int n;
    vector<vector<int>> val, Max;
    vector<vector<pair<int, int>>> ver;
    vector<int> lg, dep;
    Tree(int n) {
        this->n = n;
        ver.resize(n + 1);
        val.resize(n + 1, vector<int>(30));
        Max.resize(n + 1, vector<int>(30));
        lg.resize(n + 1);
        dep.resize(n + 1);
        for (int i = 1; i <= n; i++) { // 预处理 log
            lg[i] = lg[i - 1] + (1 << lg[i - 1] == i);
        }
    }
    void add(int x, int y, int w) { // 建立双向边
        ver[x].push_back({y, w});
        ver[y].push_back({x, w});
    }
    void dfs(int x, int fa) {
        val[x][0] = fa;
        dep[x] = dep[fa] + 1;
        for (int i = 1; i <= lg[dep[x]]; i++) {
            val[x][i] = val[val[x][i - 1]][i - 1];
            Max[x][i] = max(Max[x][i - 1], Max[val[x][i - 1]][i - 1]);
        }
        for (auto [y, w] : ver[x]) {
            if (y == fa) continue;
            Max[y][0] = w;
            dfs(y, x);
        }
    }
    int lca(int x, int y) {
        if (dep[x] < dep[y]) swap(x, y);
        while (dep[x] > dep[y]) {
            x = val[x][lg[dep[x] - dep[y]] - 1];
        }
        if (x == y) return x;
        for (int k = lg[dep[x]] - 1; k >= 0; k--) {
            if (val[x][k] == val[y][k]) continue;
            x = val[x][k];
            y = val[y][k];
        }
        return val[x][0];
    }
    int calc(int x, int y) { // 倍增查询两点间距离
        return dep[x] + dep[y] - 2 * dep[lca(x, y)];
    }
    int query(int x, int y) { // 倍增查询两点路径上的最大边权（带权图）
        auto get = [&](int x, int y) -> int {
            int ans = 0;
            if (x == y) return ans;
            for (int i = lg[dep[x]]; i >= 0; i--) {
                if (dep[val[x][i]] > dep[y]) {
                    ans = max(ans, Max[x][i]);
                    x = val[x][i];
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
};


// 树上路径交
// 计算两条路径的交点数量，直接载入任意 LCA 封装即可。
int intersection(int x, int y, int X, int Y) {
    vector<int> t = {lca(x, X), lca(x, Y), lca(y, X), lca(y, Y)};
    sort(t.begin(), t.end());
    int r = lca(x, y), R = lca(X, Y);
    if (dep[t[0]] < min(dep[r], dep[R]) || dep[t[2]] < max(dep[r], dep[R])) {
        return 0;
    }
    return 1 + clac(t[2], t[3]);
}
