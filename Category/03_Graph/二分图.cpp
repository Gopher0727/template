// '二分图' 定义：
// 如果一张无向图的 N 个节点可以分成 A，B 两个不相交的非空集合，并且同一个集合内的点没有边相连，那么称该无向图为二分图。
//
// 代码采用邻接表存图，且点下标从 0 开始。
//
// '二分图' 染色判定：
// 由于二分图不存在奇环，尝试用两种颜色标记图中的节点，当一个点被标记后，所有与它相邻的节点应该标记与它相反的颜色，
// 若标记过程中产生冲突，则说明图中存在奇环。
//
vector<int> col(n, -1);
auto dfs = [&](auto&& dfs, int u, int c = 0) -> bool {
    col[u] = c;
    for (int v : g[u]) {
        if (col[v] == -1 && dfs(dfs, v, c ^ 1) || col[v] == c) {
            return true;
        }
    }
    return false;
};

bool flag = true;
for (int i = 0; i < n; ++i) {
    // 发现奇环，不是二分图
    if (col[i] == -1 && dfs(dfs, i)) {
        flag = false;
        break;
    }
}


// 2-Sat
struct TwoSat {
    int n;
    vector<vector<int>> e;
    vector<bool> ans;

    explicit TwoSat(int n) : n(n), e(2 * n), ans(n) {}

    void addEdge(int u, bool f, int v, bool g) {
        e[2 * u + !f].push_back(2 * v + g);
        e[2 * v + !g].push_back(2 * u + f);
    }

    bool work() {
        vector<int> id(2 * n, -1), dfn(2 * n, -1), low(2 * n, -1);
        vector<int> stk;
        int now = 0, cnt = 0;
        auto tarjan = [&](auto&& self, int u) -> void {
            stk.push_back(u);
            dfn[u] = low[u] = now++;
            for (auto v : e[u]) {
                if (dfn[v] == -1) {
                    self(self, v);
                    low[u] = min(low[u], low[v]);
                } else if (id[v] == -1) {
                    low[u] = min(low[u], dfn[v]);
                }
            }
            if (dfn[u] == low[u]) {
                int v;
                do {
                    v = stk.back();
                    stk.pop_back();
                    id[v] = cnt;
                } while (v != u);
                ++cnt;
            }
        };
        for (int i = 0; i < 2 * n; ++i) {
            if (dfn[i] == -1) {
                tarjan(tarjan, i);
            }
        }
        for (int i = 0; i < n; ++i) {
            if (id[2 * i] == id[2 * i + 1]) {
                return false;
            }
            ans[i] = id[2 * i] > id[2 * i + 1];
        }
        return true;
    }
};

//// 如果需要“答案不唯一时不输出”

// 结构体中增加
int check(int x, int y) {
    vector<int> vis(2 * n);
    auto dfs = [&](auto self, int x) -> void {
        vis[x] = 1;
        for (auto y : e[x]) {
            if (vis[y]) continue;
            self(self, y);
        }
    };
    dfs(dfs, x);
    return vis[y];
}
// 主函数中增加
for (int i = 0; i < n; i++) {
    if (sat.check(2 * i, 2 * i + 1)) {
        cout << 1 << " ";
    } else if (sat.check(2 * i + 1, 2 * i)) {
        cout << 0 << " ";
    } else {
        cout << "?" << " ";
    }
}
