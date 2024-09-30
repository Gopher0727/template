#define MAX 200001

int BFS_TopoSort() {
    int n, m; // 点数、边数
    cin >> n >> m;

    vector<int> adjlist[MAX]; // 邻接表
    int inDegree[MAX] {}; // 入度

    //* Data processing
    int a, b;
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        adjlist[a].emplace_back(b);
        inDegree[b]++;
    }

    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (inDegree[i] == 0) { // 入度为 0 的点入队
            q.emplace(i);
        }
    }

    int cnt = 0; // 用于判断图中是否存在环的计数器
    vector<int> res;
    while (!q.empty()) {
        int u = q.front();
        res.emplace_back(u);
        q.pop();

        for (int i = 0; i < adjlist[u].size(); i++) {
            int v = adjlist[u][i];
            if (--inDegree[v] == 0) { // 若入度减为0，则入队
                q.emplace(v);
            }
        }

        cnt++;
    }

    if (cnt != n) {
        cout << -1;
        return 0;
    }

    // 图无环，存在拓扑序列
    for (int i = 0; i < res.size(); i++) {
        cout << res[i] << " \n"[i == res.size() - 1];
    }

    return 0;
}
