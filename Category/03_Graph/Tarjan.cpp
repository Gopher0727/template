// 求割点、割边、连通分量（强连通分量 SCC，点双连通分量 VBCC，边双连通分量 EBCC）
//
// 引入：DFS 生成树（无向图）
//     1> T 的根节点是一个割点，当且仅当根节点有两个或更多的子节点
//     2> T 的非根节点 u 是一个割点，当且仅当 u 存在一个子节点 v，且 v 和它的后代都没有回退边连向 u 的祖先
//
// dfn[x]：时间戳，节点 x 第一次被访问的顺序
// low[x]：追溯值，从节点 x 出发，所能访问到的最早时间戳

struct Cut {
    int n;
    int clk;
    int root;
    vector<vector<int>> g;
    vector<int> dfn;
    vector<int> low;
    vector<bool> isCut;

    explicit Cut(int n) { init(n); }

    void init(int n) {
        this->n = n;
        clk = 0;
        g.assign(n, {});
        low.assign(n, 0);
        dfn.assign(n, -1);
        isCut.assign(n, false);
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void Tarjan(int u, int pa) {
        dfn[u] = low[u] = clk++;
        int son = 0;
        for (int v : g[u]) {
            if (dfn[v] == -1) {
                son++;
                Tarjan(v, u);
                low[u] = min(low[u], low[v]);
                // 求割边去掉等于的情况即可
                if (u != root && dfn[u] <= low[v]) {
                    isCut[u] = true;
                }
            } else if (v != pa && dfn[v] < dfn[u]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (u == root && son >= 2) {
            isCut[u] = true;
        }
    }

    void work() {
        for (int i = 0; i < n; i++) {
            root = i;
            if (dfn[i] == -1) {
                Tarjan(i, -1);
            }
        }
    }
};


// （有向图）强连通分量（SCC, Strongly Connected Components）缩点
//
// 有向有环图 --> 有向无环图（DAG、拓扑图）
//     1> 缩点之后，观察新图中点的入度、出度情况去构造答案
//     2> 缩点之后，重建拓扑图，递推
//
// col[x]：即 color，记录所属的连通块

struct SCC {
    int n;
    int clk;
    int cnt;
    vector<vector<int>> g;
    vector<int> dfn;
    vector<int> low;
    vector<int> col;
    vector<int> stk;

    explicit SCC(int n) { init(n); }

    void init(int n) {
        this->n = n;
        clk = 0;
        cnt = 0;
        g.assign(n, {});
        dfn.assign(n, -1);
        low.assign(n, {});
        col.assign(n, -1);
        stk.clear();
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void Tarjan(int u) {
        dfn[u] = low[u] = clk++;
        stk.push_back(u);
        for (auto v : g[u]) {
            if (dfn[v] == -1) {
                Tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (col[v] == -1) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            int pre;
            do {
                pre = stk.back();
                col[pre] = cnt;
                stk.pop_back();
            } while (pre != u);
            cnt++;
        }
    }

    auto work() { // [cnt 新图的顶点数量]
        for (int i = 0; i < n; i++) { // 避免图不连通
            if (dfn[i] == -1) {
                Tarjan(i);
            }
        }

        vector<vector<int>> adj(cnt);
        vector<int> siz(cnt); // siz 每个 scc 中点的数量
        for (int i = 0; i < n; i++) {
            int u = col[i];
            siz[u]++;
            for (auto j : g[i]) {
                int v = col[j];
                if (v != u) {
                    adj[u].push_back(v);
                }
            }
        }
        return tuple {cnt, adj, col, siz};
    }
};


// （无向图）割边与割边缩点
//
// 求解图中全部割边、划分边双（颜色相同的点位于同一个边双连通分量中）  O(N+M)
// > 对于一个边双，删去任意边后依旧联通；
// > 对于边双中的任意两点，一定存在两条不相交的路径连接这两个点（路径上可以有公共点，但是没有公共边）。

struct EBCC {
    int n;
    int clk;
    int cnt;
    vector<vector<int>> g;
    vector<int> dfn;
    vector<int> low;
    vector<int> col;
    vector<int> stk;
    set<array<int, 2>> bridge;

    explicit EBCC(int n) { init(n); }

    void init(int n) {
        this->n = n;
        clk = 0;
        cnt = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, -1);
        low.assign(n + 1, 0);
        col.assign(n + 1, -1);
        stk.clear();
        bridge.clear();
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void Tarjan(int u, int pa) {
        dfn[u] = low[u] = clk++;
        stk.push_back(u);
        for (auto v : g[u]) {
            if (v == pa) {
                continue;
            }
            if (dfn[v] == -1) {
                bridge.insert({u, v});
                Tarjan(v, u);
                low[u] = min(low[u], low[v]);
            } else if (col[v] == -1 && dfn[v] < dfn[u]) {
                bridge.insert({u, v});
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            cnt++;
            int pre;
            do {
                pre = stk.back();
                col[pre] = cnt;
                stk.pop_back();
            } while (pre != u);
        }
    }

    auto work() { // [cnt 新图的顶点数量, bridge 全部割边]
        for (int i = 1; i <= n; i++) { // 避免图不连通
            if (dfn[i] == -1) {
                Tarjan(i, 0);
            }
        }

        vector<vector<int>> adj(cnt + 1);
        vector<int> siz(cnt + 1); // siz 每个 scc 中点的数量
        for (int i = 1; i <= n; i++) {
            siz[col[i]]++;
            for (auto j : g[i]) {
                int u = col[i], v = col[j];
                if (u != v) {
                    adj[u].push_back(v);
                }
            }
        }
        return tuple {cnt, adj, col, siz};
    }
};


set<pair<int, int>> E;
struct EBCC {
    int n;
    int clk;
    int cnt;
    vector<vector<int>> adj;
    vector<int> dfn;
    vector<int> low;
    vector<int> col;
    vector<int> stk;

    explicit EBCC(int n) { init(n); }

    void init(int n) {
        this->n = n;
        clk = 0;
        cnt = 0;
        adj.assign(n, {});
        dfn.assign(n, -1);
        low.assign(n, {});
        col.assign(n, -1);
        stk.clear();
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void Tarjan(int x, int p) {
        dfn[x] = low[x] = clk++;
        stk.push_back(x);
        for (auto y : adj[x]) {
            if (y == p) {
                continue;
            }
            if (dfn[y] == -1) {
                E.emplace(x, y);
                Tarjan(y, x);
                low[x] = min(low[x], low[y]);
            } else if (col[y] == -1 && dfn[y] < dfn[x]) {
                E.emplace(x, y);
                low[x] = min(low[x], dfn[y]);
            }
        }
        if (dfn[x] == low[x]) {
            int y;
            do {
                y = stk.back();
                col[y] = cnt;
                stk.pop_back();
            } while (y != x);
            cnt++;
        }
    }

    vector<int> work() {
        Tarjan(0, -1);
        return col;
    }

    struct Graph {
        int n;
        vector<pair<int, int>> edges;
        vector<int> siz;
        vector<int> cnte;
    };

    Graph compress() {
        Graph g;
        g.n = cnt;
        g.siz.resize(cnt);
        g.cnte.resize(cnt);
        for (int i = 0; i < n; i++) {
            g.siz[col[i]]++;
            for (auto j : adj[i]) {
                if (col[i] < col[j]) {
                    g.edges.push_back({col[i], col[j]});
                } else if (i < j) {
                    g.cnte[col[i]]++;
                }
            }
        }
        return g;
    }
};


// （无向图）割点与割点缩点
//
// 求解图中全部割点、划分点双（颜色相同的点位于同一个点双连通分量中）  O(N+M)
// > 每一个割点至少属于两个点双

struct VBCC {
    int n;
    int clk;
    int cnt;
    vector<vector<int>> g;
    vector<vector<int>> col;
    vector<int> dfn;
    vector<int> low;
    vector<int> stk;
    vector<bool> isCut; // 记录是否为割点

    explicit VBCC(int n) { init(n); }

    void init(int n) {
        this->n = n;
        cnt = 0;
        clk = 0;
        g.resize(n + 1);
        col.resize(2 * n + 1);
        dfn.resize(n + 1);
        low.resize(n + 1);
        stk.clear();
        isCut.resize(n + 1);
    }

    void addEdge(int x, int y) {
        g[x].push_back(y);
        g[y].push_back(x);
    }

    void Tarjan(int x, int root) {
        low[x] = dfn[x] = clk++;
        stk.push_back(x);
        if (x == root && !g[x].size()) { // 特判孤立点
            ++cnt;
            col[cnt].push_back(x);
            return;
        }
        int flag = 0;
        for (auto y : g[x]) {
            if (!dfn[y]) {
                Tarjan(y, root);
                low[x] = min(low[x], low[y]);
                if (dfn[x] <= low[y]) {
                    flag++;
                    if (x != root || flag > 1) {
                        isCut[x] = true; // 标记为割点
                    }
                    int pre = 0;
                    cnt++;
                    do {
                        pre = stk.back();
                        col[cnt].push_back(pre);
                        stk.pop_back();
                    } while (pre != y);
                    col[cnt].push_back(x);
                }
            } else {
                low[x] = min(low[x], dfn[y]);
            }
        }
    }

    void work() {
        for (int i = 1; i <= n; ++i) { // 避免图不连通
            if (!dfn[i]) {
                Tarjan(i, i);
            }
        }
    }

    pair<int, vector<vector<int>>> rebuild() { // [新图的顶点数量, 新图]
        work();
        vector<vector<int>> adj(cnt + 1);
        for (int i = 1; i <= cnt; i++) {
            if (!col[i].size()) { // 注意，孤立点也是 VBCC
                continue;
            }
            for (auto j : col[i]) {
                if (isCut[j]) {
                    adj[i].push_back(isCut[j]);
                    adj[isCut[j]].push_back(i);
                }
            }
        }
        return {cnt, adj};
    }
};
