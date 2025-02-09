// 左偏树便于进行可持久化
// 支持两个堆的合并过程，时间复杂度 O(log n)
//
// 堆结构的平衡性由 dist 值来维护
// dist 值表示：离开该节点后，至少要经过几个节点到空
// dist[0] = -1
// 合并后，左儿子 dist 值 < 右儿子 dist 值时就交换左右子树
// 节点 dist 值 = 右儿子 dist 值 + 1
//
// 左偏树的平衡性只是方便了合并的过程，树上依然可能出现很长的链，所以任何节点找堆顶的过程（即查询）需要结合并查集
//
// Link：
// https://www.luogu.com.cn/problem/P3377
// https://www.luogu.com.cn/problem/P2713


const int MAXN = 100001;

vector<int> num;
vector<int> ls;
vector<int> rs;
vector<int> dist;
vector<int> fa;

auto init = []() {
    num.resize(MAXN);
    ls.resize(MAXN);
    rs.resize(MAXN);
    dist.resize(MAXN);
    dist[0] = -1;
    fa.resize(MAXN);
    iota(fa.begin(), fa.end(), 0);
    return 0;
}();

int find(int i) {
    return i == fa[i] ? i : fa[i] = find(fa[i]);
}

int merge(int i, int j) {
    if (i == 0 || j == 0) {
        return i + j;
    }
    if (num[i] > num[j] || num[i] == num[j] && i > j) {
        swap(i, j);
    }
    rs[i] = merge(rs[i], j);
    if (dist[ls[i]] < dist[rs[i]]) {
        swap(ls[i], rs[i]);
    }
    dist[i] = dist[rs[i]] + 1;
    fa[ls[i]] = fa[rs[i]] = i;
    return i;
}

int pop(int i) {
    fa[ls[i]] = ls[i], fa[rs[i]] = rs[i];
    fa[i] = merge(ls[i], rs[i]); // 查询压缩的过程需要把 i 的 fa 改成 merge 后的节点
    ls[i] = rs[i] = dist[i] = 0;
    return fa[i];
}

void solve() {
    int n, m;
    cin >> n >> m;

    for (int i = 1; i <= n; i++) {
        cin >> num[i];
    }
    for (int i = 1; i <= m; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            int x, y;
            cin >> x >> y;
            if (num[x] != -1 && num[y] != -1) {
                int l = find(x), r = find(y);
                if (l != r) {
                    merge(l, r);
                }
            }
        } else {
            int x;
            cin >> x;
            if (num[x] == -1) {
                cout << -1 << "\n";
            } else {
                int ans = find(x);
                cout << num[ans] << "\n";
                pop(ans);
                num[ans] = -1;
            }
        }
    }
}
