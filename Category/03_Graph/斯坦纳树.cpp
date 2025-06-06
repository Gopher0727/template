// 在带权无向图中，寻找连接图中“关键点”的权值最小的子树
// 与 MST 相似，不过斯坦纳树只关心指定的关键点子集，可以自由选择非关键点作为中间辅助连接点（斯坦纳点）

// 一般图上的斯坦纳树问题是 NP hard 的，但关键点数量 k 较小的情况可以通过状压 DP 求解

// dp[S][v]：S 表示关键点子集，且构造的斯坦纳树包含顶点 v 时，所需的最小总代价

// G = (V, E)  N≈100, k≈10

struct Steiner {
    int n, k;
    vector<vector<pair<int, i64>>> g;
    vector<vector<i64>> dp;
    vector<int> id; // 存储K个关键点的原始编号

    explicit Steiner(int n, int k) : n(n), k(k) {
        g.assign(n, {});
        dp.assign(1 << k, vector(n, inf));
    }

    void addEdge(int u, int v, int w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    void addPoint(int u) {
        int i = id.size();
        id.push_back(u);
        dp[1 << i][u] = 0;
    }

    void spfa(int S) { // 对固定的关键点集合S，跑SPFA进行扩展
        vector<bool> inq(n);
        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (dp[S][i] != inf) { // 从已有点进行扩展
                q.push(i);
                inq[i] = true;
            }
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = false;
            for (auto [v, w] : g[u]) {
                if (dp[S][v] > dp[S][u] + w) {
                    dp[S][v] = dp[S][u] + w;
                    if (inq[v]) {
                        continue;
                    }
                    q.push(v);
                    inq[v] = true;
                }
            }
        }
    }

    void dijkstra(int S) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        for (int i = 0; i < n; ++i) {
            if (dp[S][i] != inf) {
                pq.push({dp[S][i], i});
            }
        }
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dp[S][u]) {
                continue;
            }
            for (auto& [v, w] : g[u]) {
                if (dp[S][u] + w < dp[S][v]) {
                    dp[S][v] = dp[S][u] + w;
                    pq.push({dp[S][v], v});
                }
            }
        }
    }

    void work() {
        // 外层循环：枚举关键点集合 S
        for (int S = 1; S < (1 << k); S++) {
            // 类型一转移：合并子问题
            // S_sub 从 (S-1)&S 开始枚举，确保 S_sub 是 S 的真子集
            for (int s_sub = (S - 1) & S; s_sub; s_sub = (s_sub - 1) & S) {
                int s_oth = S ^ s_sub; // S \ s_sub

                // 可选优化：避免 (s_sub, s_oth) 和 (s_oth, s_sub) 重复
                // 但这里s_sub固定，s_oth随之确定，不会重复
                // if (s_oth < s_sub) {
                //     continue;
                // }

                for (int v = 0; v < n; v++) { // 枚举汇合点 v
                    if (dp[s_sub][v] != inf && dp[s_oth][v] != inf) {
                        dp[S][v] = min(dp[S][v], dp[s_sub][v] + dp[s_oth][v]);
                    }
                }
            }

            // 类型二转移：扩展连通分量 (对当前集合S，用 SPFA/Dijkstra 松弛)
            // spfa(S);
            dijkstra(S);
        }
    }

    i64 get() { return ranges::min(dp[(1 << k) - 1]); }
};
// 默认：无向图，点下标从 0 开始
