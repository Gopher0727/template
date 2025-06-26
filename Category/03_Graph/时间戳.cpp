// 根据 DFS 的性质，当我们递归以 u 为根的子树时，设 v 是 u 的子孙节点，我们必须先递归完以 v 为根的子树，
// 之后才能递归完以 u 为根的子树。
// 从时间戳上看，如果 v 是 u 的子孙节点，那么区间 [in[v], out[v]] 必然被区间 [in[u], out[u]] 所包含。
// 反之，如果区间 [in[v], out[v]] 被区间 [in[u], out[u]] 所包含，那么 v 必然是 u 的子孙节点（也即 u 是 v 的祖先节点）。
// 因此我们可以通过 in[u] < in[v] ≤ out[v] ≤ out[u] 来判断 u 是否为 v 的祖先节点。
// 由于 in[v] ≤ out[v] 恒成立，上式可以简化为 in[u] < in[v] ≤ out[u]。

vector<int> in(n), out(n);
int clk = 0;
auto dfs = [&](this auto&& dfs, int u = 0, int pa = -1) -> void {
    in[u] = clk;
    for (int v : g[u]) {
        if (v != pa) {
            dfs(v, u);
        }
    }
    out[u] = ++clk;
};
dfs();
