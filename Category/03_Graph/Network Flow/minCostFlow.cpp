// 定义超级源点与超级汇点
// ...
int S;
int T;

struct neighbor {
    int to, rid;
    i64 cap, cost;
};
vector<vector<neighbor>> g(T + 1);
auto addEdge = [&](int from, int to, i64 cap, i64 cost) {
    g[from].push_back({to, (int) g[to].size(), cap, cost});
    g[to].push_back({from, (int) g[from].size() - 1, 0, -cost});
};

// 左部向右部连边，超级源点向左部连边，右部向超级汇点连边
// ...

// template
vector<pair<int, int>> pa(T + 1); // int v, i;
vector<i64> dis(T + 1);
vector<int> inQ(T + 1);
auto spfa = [&]() {
    fill(dis.begin(), dis.end(), numeric_limits<i64>::max());
    dis[S] = 0, inQ[S] = 1;
    queue<int> q;
    q.push(S);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        inQ[v] = 0;
        for (int i = 0; i < g[v].size(); i++) {
            if (g[v][i].cap == 0) {
                continue;
            }
            int w = g[v][i].to;
            if (i64 nd = dis[v] + g[v][i].cost; nd < dis[w]) {
                dis[w] = nd;
                pa[w] = {v, i};
                if (inQ[w] == 0) {
                    inQ[w] = 1;
                    q.push(w);
                }
            }
        }
    }
    return dis[T] < numeric_limits<int>::max(); // todo
};

i64 minCost = 0;
while (spfa()) {
    i64 minF = numeric_limits<i64>::max();
    for (int v = T; v != S;) {
        auto& [pv, pi] = pa[v];
        minF = min(minF, g[pv][pi].cap);
        v = pv;
    }
    for (int v = T; v != S;) {
        auto& [pv, pi] = pa[v];
        auto& e = g[pv][pi];
        e.cap -= minF;
        g[v][e.rid].cap += minF;
        v = pv;
    }
    minCost += dis[T] * minF;
}
