int n, m;
vector<bool> vis;
vector<int> head, nxt, to;

/**

    head[v] -> e    点-边
    nxt[e] -> e     边-边
    to[e] -> v      边-点

加边：
    u1 -(e1)- u2
    u1 -(e2)- u3 -(e1)- u2

        nxt[++cnt] = head[u]; head[u] = cnt; to[cnt] = v;
    or: nxt[cnt] = head[u]; to[cnt] = v; head[u] = cnt++;

*/

void add(int u, int v) {
    nxt.push_back(head[u]);
    head[u] = to.size();
    to.push_back(v);
}

bool find_edge(int u, int v) {
    for (int i = head[u]; ~i; i = nxt[i]) { // ~i 表示 i != -1
        if (to[i] == v) {
            return true;
        }
    }
    return false;
}

void dfs(int u) {
    if (vis[u]) {
        return;
    }
    vis[u] = true;
    for (int i = head[u]; ~i; i = nxt[i]) {
        dfs(to[i]);
    }
}

int main() {
    cin >> n >> m;

    vis.resize(n + 1, false);
    head.resize(n + 1, -1);

    for (int i = 1; i <= m; ++i) {
        int u, v;
        cin >> u >> v;
        add(u, v);
    }

    return 0;
}
