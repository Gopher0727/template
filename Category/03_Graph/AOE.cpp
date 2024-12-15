int main() {
    int n, m;
    cin >> n >> m;

    // 下标从 0 开始
    vector<vector<pair<int, int>>> g(n);
    vector<vector<pair<int, int>>> rg(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
        rg[v].emplace_back(u, w);
    }

    vector<int> inDeg(n), outDeg(n);
    for (int u = 0; u < n; ++u) {
        for (auto& [v, _] : g[u]) {
            inDeg[v]++;
            outDeg[u]++;
        }
    }

    vector<int> pa(n, -1);
    vector<int> est(n, numeric_limits<int>::min()); // Longest path distances
    est[0] = 0;

    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (inDeg[i] == 0) {
            q.push(i);
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto& [v, w] : g[u]) {
            if (est[u] != numeric_limits<int>::min() && est[u] + w > est[v]) {
                est[v] = est[u] + w;
                pa[v] = u;
            }
            if (--inDeg[v] == 0) {
                q.push(v);
            }
        }
    }

    int maxDistance = *max_element(est.begin(), est.end());
    int endNode = max_element(est.begin(), est.end()) - est.begin();

    vector<int> lst(n, maxDistance);
    queue<int> rq;
    for (int i = 0; i < n; ++i) {
        if (outDeg[i] == 0) {
            rq.push(i);
        }
    }
    while (!rq.empty()) {
        int u = rq.front();
        rq.pop();
        for (auto& [v, w] : rg[u]) {
            lst[v] = min(lst[v], lst[u] - w);
            if (--outDeg[v] == 0) {
                rq.push(v);
            }
        }
    }

    // Output:
    cout << "The longest path length is: " << maxDistance << endl;
    cout << "The critical path is: ";
    vector<int> path;
    for (int cur = endNode; cur >= 0; cur = pa[cur]) {
        path.push_back(cur);
        if (cur == 0) {
            break;
        }
    }
    ranges::reverse(path);
    for (int v : path) {
        cout << v << " ";
    }
    cout << endl;

    cout << "Earliest Start Times (EST): ";
    for (int i = 0; i < n; ++i) {
        cout << est[i] << " \n"[i == n - 1];
    }

    cout << "Latest Start Times (LST): ";
    for (int i = 0; i < n; ++i) {
        cout << lst[i] << " \n"[i == n - 1];
    }

    return 0;
}


/*

测试数据：
6 10

0 1 2
0 2 5
1 2 1
1 3 3
2 3 3
2 5 1
2 4 4
3 4 1
4 5 1
3 5 4

输出：
The longest path length is: 12
The critical path is: 0 2 3 5
Earliest Start Times (EST): 0 2 5 8 9 12
Latest Start Times (LST): 0 4 5 8 11 12

*/
