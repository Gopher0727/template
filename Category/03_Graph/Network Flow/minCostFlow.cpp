// 题目：
// https://leetcode.cn/problems/minimum-time-to-break-locks-i/
// https://leetcode.cn/problems/select-cells-in-grid-with-maximum-score/



// 定义超级源点与超级汇点
// ...

struct neighbor {
    int to, rid, cap, cost;
};
vector<vector<neighbor>> g(T + 1);
auto addEdge = [&](int from, int to, int cap, int cost) {
    g[from].emplace_back(to, g[to].size(), cap, cost);
    g[to].emplace_back(from, g[from].size() - 1, 0, -cost);
};

// 左部向右部连边，超级源点向左部连边，右部向超级汇点连边
// ...

// template
vector<pair<int, int>> pa(T + 1); // int v, i;
vector<int> dis(T + 1), inQ(T + 1);
auto spfa = [&]() {
    fill(dis.begin(), dis.end(), numeric_limits<int>::max());
    dis[S] = 0, inQ[S] = 1;
    queue<int> q;
    q.emplace(S);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        inQ[v] = 0;
        for (int i = 0; i < g[v].size(); ++i) {
            if (g[v][i].cap == 0) {
                continue;
            }
            int w = g[v][i].to;
            int nd = dis[v] + g[v][i].cost;
            if (nd < dis[w]) {
                dis[w] = nd;
                pa[w] = {v, i};
                if (inQ[w] == 0) {
                    inQ[w] = 1;
                    q.emplace(w);
                }
            }
        }
    }
    return dis[T] < numeric_limits<int>::max();
};

int minCost = 0;
while (spfa()) {
    int minF = numeric_limits<int>::max();
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
return minCost;

