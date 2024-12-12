// '二分图' 定义：
// 如果一张无向图的 N 个节点可以分成 A，B 两个不相交的非空集合，并且同一个集合内的点没有边相连，那么称该无向图为二分图。
//


// 代码采用邻接表存图，且点下标从 0 开始。


// '二分图' 判定：
// 染色法：由于，二分图不存在奇环。尝试用两种颜色标记图中的节点，当一个点被标记后，所有与它相邻的节点应该标记与它相反的颜色，
//       若标记过程中产生冲突，则说明图中存在奇环。
//
vector<int> color(n, -1);

auto dfs = [&](auto&& dfs, int u, int c = 0) -> bool {
    color[u] = c;
    for (int v : g[u]) {
        if (color[v] == -1) {
            if (dfs(dfs, v, c ^ 1)) {
                return true;
            }
        } else if (color[v] == c) {
            return true;
        }
    }
    return false;
};

bool flag = false;
for (int i = 0; i < n; ++i) {
    if (color[i] == -1) {
        if (dfs(i)) {
            flag = true; // 发现奇环
            break;
        }
    }
}




const int maxn = 1000 + 5; // 单侧顶点的最大数目

// 二分图最大基数匹配
struct BPM {
    int n, m; // 左右顶点个数
    vector<int> G[maxn]; // 邻接表
    int left[maxn]; // left[i]为右边第i个点的匹配点编号，-1表示不存在
    bool T[maxn]; // T[i]为右边第i个点是否已标记
    int right[maxn]; // 求最小覆盖用
    bool S[maxn]; // 求最小覆盖用
    void init(int n, int m) {
        this->n = n;
        this->m = m;
        for(int i = 0; i < n; i++) G[i].clear();
    }
    void AddEdge(int u, int v) {
        G[u].push_back(v);
    }
    bool match(int u){
        S[u] = true;
        for(int i = 0; i < G[u].size(); i++) {
            int v = G[u][i];
            if (!T[v]){
                T[v] = true;
                if (left[v] == -1 || match(left[v])){
                    left[v] = u;
                    right[u] = v;
                    return true;
                }
            }
        }
        return false;
    }
    // 求最大匹配
    int solve() {
        memset(left, -1, sizeof(left));
        memset(right, -1, sizeof(right));
        int ans = 0;
        for(int u = 0; u < n; u++) { // 从左边结点u开始增广
            memset(S, 0, sizeof(S));
            memset(T, 0, sizeof(T));
            if(match(u)) ans++;
        }
        return ans;
    }
    // 求最小覆盖。X和Y为最小覆盖中的点集（最大独立集与最小覆盖集互补）
    int mincover(vector<int>& X, vector<int>& Y) {
        int ans = solve();
        memset(S, 0, sizeof(S));
        memset(T, 0, sizeof(T));
        for(int u = 0; u < n; u++)
            if(right[u] == -1) match(u); // 从所有X未盖点出发增广
        for(int u = 0; u < n; u++)
            if(!S[u]) X.push_back(u); // X中的未标记点
        for(int v = 0; v < m; v++)
            if(T[v]) Y.push_back(v); // Y中的已标记点
     return ans;
    }
};
BPM solver;
int R, C, N;
int main(){
    int kase = 0;
    while(scanf("%d%d%d", &R, &C, &N) == 3 && R && C && N) {
        solver.init(R, C);
        for(int i = 0; i < N; i++) {
            int r, c;
            scanf("%d%d", &r, &c); r--; c--;
            solver.AddEdge(r, c);
        }
        vector<int> X, Y;
        int ans = solver.mincover(X, Y);
        printf("%d", ans);
        for(int i = 0; i < X.size(); i++) printf(" r%d", X[i]+1);
        for(int i = 0; i < Y.size(); i++) printf(" c%d", Y[i]+1);
        printf("\n");
    }
    return 0;
}
