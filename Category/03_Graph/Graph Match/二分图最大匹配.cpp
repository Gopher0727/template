// 匈牙利算法/增广路算法  O(nm)  Hungarian

// 注：更推荐用 Dinic，可以达到 O(m√n) 的复杂度（而且这个复杂度一般不会跑满）
// - 超级源点 S 连左部，右部连超级汇点 T，所有边的容量均为 1，那么 S 到 T 的最大流即为二分图的最大匹配

// 有关随机贪心(匹配)预处理的 hack https://bzoj.blog.uoj.ac/blog/2427

// König 定理: 一个二分图中的最大匹配数等于这个图中的最小点覆盖数

// Problems:
// - https://leetcode.cn/problems/maximum-students-taking-exam
// - https://leetcode.cn/problems/broken-board-dominoes
// - https://codeforces.com/contest/1139/problem/E
// - https://codeforces.com/contest/1404/problem/E
// - https://codeforces.com/contest/1783/problem/F

// 写法 1
auto maxBipartiteMatchingHungarian(const vector<vector<int>>& g) {
    int n = g.size();
    vector<int> match(n, -1);
    vector<int> vis(n);
    int ts = 0;

    auto dfs = [&](this auto&& dfs, int u) -> bool {
        vis[u] = ts;
        for (auto v : g[u]) {
            int w = match[v];
            if (w == -1 || vis[w] != ts && dfs(w)) {
                match[u] = v;
                match[v] = u;
                return true;
            }
        }
        return false;
    };

    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (match[i] != -1) {
            continue;
        }
        ts++;
        if (dfs(i)) {
            cnt++;
            // cnt += 2;
        }
    }
    return pair {match, cnt};
}


// 写法 2
// 适用左右两侧节点有明确区分的情况，要求 g 中存储的是左侧到右侧的单向边
// 常见于棋盘放置 1x2 骨牌，或者一些排列型约束的题目
auto maxBipartiteMatchingHungarian(int nL, int nR, const vector<vector<int>>& g) {
    vector<int> matchL(nL, -1), matchR(nR, -1);
    vector<int> vis(nL);
    int ts = 0;

    auto dfs = [&](this auto&& dfs, int u) -> bool {
        vis[u] = ts;
        for (auto v : g[u]) {
            int lw = matchR[v];
            if (lw == -1 || vis[lw] != ts && dfs(lw)) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }
        return false;
    };

    int cnt = 0;
    for (int i = 0; i < g.size(); i++) {
        ts++;
        if (dfs(i)) {
            cnt++;
            // cnt += 2;
        }
    }
    return pair {matchL, cnt};
}


// HopcroftKarp 算法（HK算法、基于最大流模型）
// 实际运行复杂度较低
struct HopcroftKarp {
    vector<vector<int>> g;
    vector<int> pa, pb, vis;
    int n, m, dfn, res;

public:
    HopcroftKarp(int _n, int _m) : n(_n + 1), m(_m + 1) {
        assert(0 <= n && 0 <= m);
        pa.assign(n, -1);
        pb.assign(m, -1);
        vis.resize(n);
        g.resize(n);
        res = 0;
        dfn = 0;
    }

    void add(int x, int y) {
        assert(0 <= x && x < n && 0 <= y && y < m);
        g[x].push_back(y);
    }

    bool dfs(int v) {
        vis[v] = dfn;
        for (int u : g[v]) {
            if (pb[u] == -1) {
                pb[u] = v;
                pa[v] = u;
                return true;
            }
        }
        for (int u : g[v]) {
            if (vis[pb[u]] != dfn && dfs(pb[u])) {
                pa[v] = u;
                pb[u] = v;
                return true;
            }
        }
        return false;
    }

    int work() {
        while (1) {
            dfn++;
            int cnt = 0;
            for (int i = 0; i < n; i++) {
                if (pa[i] == -1 && dfs(i)) {
                    cnt++;
                }
            }
            if (cnt == 0) {
                break;
            }
            res += cnt;
        }
        return res;
    }
};
