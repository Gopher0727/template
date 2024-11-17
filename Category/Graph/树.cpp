struct Tree {
    int n;
    vector<vector<int>> g;

public:
    Tree(int n) : n(n), g(n + 1) {}

    void addEdge(int x, int y) {
        g[x].push_back(y);
        // g[y].push_back(x);
    }

    int getlen(int root) { // 获取x所在树的直径
        map<int, int> dep;
        auto dfs = [&](auto&& dfs, int x, int pa = 0) -> void {
            for (auto y : g[x]) {
                if (y == pa) {
                    continue;
                }
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
        vector<int> _size(n, 1);
        auto dfs = [&](auto&& dfs, int x, int pa = -1) -> void {
            for (int y : g[x]) {
                if (y == pa) {
                    continue;
                }
                dfs(dfs, y, x);
                _size[x] += _size[y];
            }
        };
        dfs(dfs, 0);
        return _size;
    }
};
