/*
    每次新插入一个元素，修改的线段树只与插入的元素有关（只有添加的值到根节点的一条链发生了变化）。

    作用: 查询任意区间第 k 大/小的值。
*/
struct Node {
    int l, r, cnt;
};

vector<Node> f;
int rk = 0;

auto clone(int x) {
    return f[++rk] = f[x], f[rk].cnt++, rk;
}

auto build(int& x, int l, int r) {
    x = ++rk;
    if (l == r) {
        return;
    }
    int mid = l + (r - l) / 2;
    build(f[x].l, l, mid);
    build(f[x].r, mid + 1, r);
}

auto update(int& x, int l, int r, int k) {
    x = clone(x);
    if (l == r) {
        return;
    }
    int mid = l + (r - l) / 2;
    if (k <= mid) {
        update(f[x].l, l, mid, k);
    } else {
        update(f[x].r, mid + 1, r, k);
    }
}

// root[l-1], root[r], 1, mx/size, k
auto query(int pre, int cur, int l, int r, int k) {
    if (l == r) {
        return l;
    }
    // 通过两个时刻同一位置的 cnt 值相减，得到位于 [l, r] 子区间内的元素数量
    int l1 = f[cur].l;
    int l2 = f[pre].l;
    int cnt = f[l1].cnt - f[l2].cnt;
    int mid = l + (r - l) / 2;
    if (cnt >= k) {
        return query(f[pre].l, f[cur].l, l, mid, k);
    }
    return query(f[pre].r, f[cur].r, mid + 1, r, k - cnt);
}

auto query(int pre, int cur, int l, int r, int L, int R) -> bool {
    if (f[cur].cnt - f[pre].cnt < 1) {
        return false;
    }
    if (L <= l && r <= R) {
        return true;
    }

    int tmp = 0;
    int mid = l + (r - l) / 2;
    if (L <= mid) {
        tmp += query(f[pre].l, f[cur].l, l, mid, L, R);
    }
    if (R > mid) {
        tmp += query(f[pre].r, f[cur].r, mid + 1, r, L, R);
    }
    return tmp;
}
// 注意下标从 1 开始。

void solve() {
    int n;
    cin >> n;

    vector<int> vec(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> vec[i];
    }

    vector<int> root(n + 1);
    f.resize(n << 5); // 基于值域划分
    int mx = ranges::max(vec);
    build(root[0], 1, mx);
    for (int i = 1; i <= n; i++) {
        root[i] = root[i - 1];
        update(root[i], 1, mx, vec[i]);
    }

    // query(root[l-1], root[r], 1, mx, k);
    // query(root[l-1], root[r], 1, mx, L, R);
}

// 主席树
template <class T>
class ChairmanTree {
private:
    struct Node {
        int val = 0;
        int ls = 0, rs = 0;
    };

    vector<int> root;
    vector<Node> f;
    int n = 0, len = 0, pos = 0;
    map<T, int> key_val;
    map<int, T> val_key;

    int build(int l, int r) {
        int root = ++pos;
        if (l == r) {
            return root;
        }
        int mid = l + (r - l) / 2;
        f[root].ls = build(l, mid);
        f[root].rs = build(mid + 1, r);
        return root;
    }

public:
    void init(int n, const vector<T>& arr) {
        vector<T> line;
        for (int i = 1; i <= n; ++i) {
            line.push_back(arr[i]);
        }
        sort(line.begin(), line.end());
        line.erase(unique(line.begin(), line.end()), line.end());
        len = line.size();
        for (int i = 0; i < line.size(); ++i) {
            key_val[line[i]] = i + 1;
            val_key[i + 1] = line[i];
        }
        this->n = n;
        root.resize(n + 10);
        f.resize((n + 10) << 5);
        root[0] = build(1, len);
        for (int i = 1; i <= n; ++i) {
            root[i] = modify(key_val[arr[i]], 1, len, root[i - 1]);
        }
    }

    int modify(int k, int l, int r, int rt) {
        int nrt = ++pos;
        f[nrt] = f[rt];
        f[nrt].val += 1;
        if (l == r) {
            return nrt;
        }
        int mid = l + (r - l) / 2;
        if (k <= mid) {
            f[nrt].ls = modify(k, l, mid, f[rt].ls);
        }
        if (mid < k) {
            f[nrt].rs = modify(k, mid + 1, r, f[rt].rs);
        }
        return nrt;
    }

    int query(int u, int v, int l, int r, int k) {
        if (l == r) {
            return l;
        }
        int mid = l + (r - l) / 2;
        int x = f[f[v].ls].val - f[f[u].ls].val;
        if (k <= x) {
            return query(f[u].ls, f[v].ls, l, mid, k);
        } else {
            return query(f[u].rs, f[v].rs, mid + 1, r, k - x);
        }
    }
    T query(int l, int r, int k) { return val_key[query(root[l - 1], root[r], 1, len, k)]; }
};
// 注意下标从 1 开始。
