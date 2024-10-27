struct Tree {
    int n;
    vector<vector<int>> g;

    Tree(int n) : n(n), g(n + 1) {}

    void add(int x, int y) {
        g[x].push_back(y);
        // g[y].push_back(x);
    }

    int getlen(int root) { // 获取x所在树的直径
        map<int, int> dep;
        auto dfs = [&](auto&& dfs, int x, int fa = 0) -> void {
            for (auto y : g[x]) {
                if (y == fa) continue;
                dep[y] = dep[x] + 1;
                dfs(dfs, y, x);
            }
            if (dep[x] > dep[root]) {
                root = x;
            }
        };
        dfs(dfs, root);
        dep.clear();
        dfs(dfs, root);
        return dep[root];
    }

    vector<int> subTreeSize() {
        vector<int> res(n, 1);
        auto dfs = [&](auto&& dfs, int x) -> void {
            for (int y : g[x]) {
                if (y != -1) {
                    dfs(dfs, y);
                    res[x] += res[y];
                }
            }
        };
        dfs(dfs, 0);
        return res;
    }
};
