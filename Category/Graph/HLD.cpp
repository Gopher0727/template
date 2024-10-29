// 树链剖分

// 1.  如果权值在边上的话，则先将权值压到深度大的点上，再建立线段树。
// 2.  结点x的子树对应的DFS序区间为[id[x], id[x] + siz[x] - 1]。

#define lc t[p].lchild
#define rc t[p].rchild

const int maxn = 110000;
int cur, root, dfn, dep[maxn], siz[maxn], pa[maxn], id[maxn], son[maxn], top[maxn];
int val[maxn], A[maxn], ans;
vector<int> G[maxn];

struct segment {
    int l, r, lchild, rchild;
    int sum, add;
}t[maxn * 4];

inline void maintain(int p) {
    t[p].sum = t[lc].sum + t[rc].sum;
}
inline void mark(int p, int addv) { //给结点打标记
    if (addv) {
        t[p].add += addv;
        t[p].sum += addv * (t[p].r - t[p].l + 1);
    }
}
inline void pushdown(int p) { //pushdown将标记传递给子结点，不影响当前结点的信息。
    mark(lc, t[p].add);
    mark(rc, t[p].add);
    t[p].add = 0;
}
int build(int L, int R) {
    int p = ++cur;
    t[p].l = L;
    t[p].r = R;
    t[p].add = 0;
    if (L == R) {
        mark(p, val[L]);
    }
    else {
        int mid = (L + R) >> 1;
        lc = build(L, mid);
        rc = build(mid + 1, R);
        maintain(p);
    }
    return p;
}
void update(int p, int L, int R, int v) {
    if (L <= t[p].l && R >= t[p].r) {
        mark(p, v);
    }
    else {
        pushdown(p); //如果没有pushdown只需要在最后调用一次maintain即可。
        int mid = (t[p].l + t[p].r) >> 1;
        if (L <= mid) 
            update(lc, L, R, v);
        if (R > mid)
            update(rc, L, R, v);
        maintain(p);
    }
}
void query(int p, int L, int R) {
    if (L <= t[p].l && R >= t[p].r) {
        ans += t[p].sum;
    }
    else {
        pushdown(p);
        int mid = (t[p].l + t[p].r) >> 1;
        if (L <= mid) 
            query(lc, L, R);
        if (R > mid) 
            query(rc, L, R);
    }
}
void dfs1(int x, int fa, int d) {
    dep[x] = d;
    siz[x] = 1;
    son[x] = 0;
    pa[x] = fa;
    for (auto y : G[x]) if (y != fa) {
        dfs1(y, x, d + 1);
        siz[x] += siz[y];
        if (siz[son[x]] < siz[y])
            son[x] = y;
    }
}
void dfs2(int x, int tp) {
    top[x] = tp;
    id[x] = ++dfn; //id[x]表示结点x的DFS序编号
    if (son[x])
        dfs2(son[x], tp);
    for (auto y : G[x])
        if (y != pa[x] && y != son[x])
            dfs2(y, y);
}
int ask(int x, int y) {
    ::ans = 0;
    while (top[x] != top[y]) {
        if (dep[top[x]] < dep[top[y]])
            swap(x, y);
        query(root, id[top[x]], id[x]);
        x = pa[top[x]];
    }
    //如果权值在边上则加上：if (x == y) return ans;
    if (dep[x] > dep[y]) swap(x, y);
    query(root, id[x], id[y]); //如果权值在边上则查询id[son[x]]
    return ans;
}
void add(int x, int y, int v) {
    while (top[x] != top[y]) {
        if (dep[top[x]] < dep[top[y]])
            swap(x, y);
        update(root, id[top[x]], id[x], v);
        x = pa[top[x]];
    }
    //如果权值在边上则加上：if (x == y) return ans;
    if (dep[x] > dep[y]) swap(x, y);
    update(root, id[x], id[y], v); //如果权值在边上则改为id[son[x]]
}
void init(int n) { //调用该函数之前应当先完成建图
    cur = dfn = siz[0] = 0;
    dfs1(1, -1, 1); //根结点为s的话，改为：dfs1(s, -1, 1), dfs2(s, s);
    dfs2(1, 1);
    for (int i = 1; i <= n; ++i)
        val[id[i]] = A[i]; //A[i]表示结点i的权值，将其复制到DFS序上。
    ::root = build(1, n);
}
int main() {
    return 0;
}


// jiangly template
/**   树链剖分（HLD）
 *    2023-08-31: https://codeforces.com/contest/1863/submission/221214363
**/
struct HLD {
    int n;
    vector<int> siz, top, dep, parent, in, out, seq;
    vector< vector<int>> adj;
    int cur;

    HLD() {}
    HLD(int n) {
        init(n);
    }
    void init(int n) {
        this->n = n;
        siz.resize(n);
        top.resize(n);
        dep.resize(n);
        parent.resize(n);
        in.resize(n);
        out.resize(n);
        seq.resize(n);
        cur = 0;
        adj.assign(n, {});
    }
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void work(int root = 0) {
        top[root] = root;
        dep[root] = 0;
        parent[root] = -1;
        dfs1(root);
        dfs2(root);
    }
    void dfs1(int u) {
        if (parent[u] != -1) {
            adj[u].erase( find(adj[u].begin(), adj[u].end(), parent[u]));
        }

        siz[u] = 1;
        for (auto &v : adj[u]) {
            parent[v] = u;
            dep[v] = dep[u] + 1;
            dfs1(v);
            siz[u] += siz[v];
            if (siz[v] > siz[adj[u][0]]) {
                 swap(v, adj[u][0]);
            }
        }
    }
    void dfs2(int u) {
        in[u] = cur++;
        seq[in[u]] = u;
        for (auto v : adj[u]) {
            top[v] = v == adj[u][0] ? top[u] : v;
            dfs2(v);
        }
        out[u] = cur;
    }
    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] > dep[top[v]]) {
                u = parent[top[u]];
            } else {
                v = parent[top[v]];
            }
        }
        return dep[u] < dep[v] ? u : v;
    }

    int dist(int u, int v) {
        return dep[u] + dep[v] - 2 * dep[lca(u, v)];
    }

    int jump(int u, int k) {
        if (dep[u] < k) {
            return -1;
        }

        int d = dep[u] - k;

        while (dep[top[u]] > d) {
            u = parent[top[u]];
        }

        return seq[in[u] - dep[u] + d];
    }

    bool isAncester(int u, int v) {
        return in[u] <= in[v] && in[v] < out[u];
    }

    int rootedParent(int u, int v) {
         swap(u, v);
        if (u == v) {
            return u;
        }
        if (!isAncester(u, v)) {
            return parent[u];
        }
        auto it =  upper_bound(adj[u].begin(), adj[u].end(), v, [&](int x, int y) {
            return in[x] < in[y];
        }) - 1;
        return *it;
    }

    int rootedSize(int u, int v) {
        if (u == v) {
            return n;
        }
        if (!isAncester(v, u)) {
            return siz[v];
        }
        return n - siz[rootedParent(u, v)];
    }

    int rootedLca(int a, int b, int c) {
        return lca(a, b) ^ lca(b, c) ^ lca(c, a);
    }
};
