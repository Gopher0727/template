// vector<vector<int>> g(n);
// vector<bool> in(n);
auto dfs = [&] (this auto&& dfs, int u, int pa) -> void {
    for (int v : g[u]) {
        if (v == pa) {
            continue;
        }
        dfs(v, u);
    }
    bool can = true;
    for (int v : g[u]) {
        if (v == pa) {
            continue;
        }
        if (in[v]) {
            can = false;
            break;
        }
    }
    if (!can) {
        in[u] = true;
    }
};
