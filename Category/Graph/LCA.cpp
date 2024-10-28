class TreeAncestor {
    vector<int> depth;
    vector<vector<int>> parent;

public:
    explicit TreeAncestor(vector<pair<int, int>>& edges, int s = 0) {
        int n = edges.size() + 1;
        int m = bit_width(n * 1ull); // int m = 32 - __builtin_clz(n);

        vector<vector<int>> g(n);
        for (auto [x, y] : edges) {
            g[x].push_back(y);
            g[y].push_back(x);
        }

        depth.resize(n);
        parent.resize(n, vector<int>(m, -1));

        auto dfs = [&](auto&& dfs, int x, int pa) -> void {
            parent[x][0] = pa;
            for (int y : g[x]) {
                if (y != pa) {
                    depth[y] = depth[x] + 1;
                    dfs(dfs, y, x);
                }
            }
        };
        dfs(dfs, s, -1);

        for (int i = 0; i < m - 1; i++) {
            for (int x = 0; x < n; x++) {
                int p = parent[x][i];
                if (p != -1) {
                    parent[x][i + 1] = parent[p][i];
                }
            }
        }
    }

    int get_kth_ancestor(int node, int k) {
        for (; k; k &= k - 1) {
            node = parent[node][__builtin_ctz(k)];
        }
        return node;
    }

    int get_lca(int x, int y) {
        if (depth[x] > depth[y]) {
            swap(x, y);
        }
        y = get_kth_ancestor(y, depth[y] - depth[x]);
        if (y == x) {
            return x;
        }
        for (int i = parent[x].size() - 1; i >= 0; i--) {
            int px = parent[x][i], py = parent[y][i];
            if (px != py) {
                x = px;
                y = py;
            }
        }
        return parent[x][0];
    }
};
// The node-index starts from 0
