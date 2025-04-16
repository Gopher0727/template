// 权值线段树基于值域划分，往往需要离散化。
//
// 作用:
// > 查询整个区间中某个数字的出现次数
// > 查询一段区间中所有数字出现的总次数
// > 查询整个区间的第 k 大/小的值
//
// 权值线段树一般 modify 即是 build。

class SegTree {
    int n;
    vector<int> cnt;

public:
    SegTree() = delete;
    SegTree(int n) : n(n), cnt(n << 2) {}

    // 单点修改：增加 v
    void modify(int x, int v, int o, int l, int r) {
        if (l == r) {
            cnt[o] += v;
            return;
        }
        int mid = l + (r - l) / 2;
        if (x <= mid) {
            modify(x, v, o << 1, l, mid);
        }
        if (x > mid) {
            modify(x, v, o << 1 | 1, mid + 1, r);
        }
        cnt[o] = cnt[o << 1] + cnt[o << 1 | 1];
    }
    void modify(int x, int v = 1) { modify(x, v, 1, 0, n - 1); }

    // 单点查询：求某数的出现次数
    int query(int x, int o, int l, int r) {
        if (l == r) {
            return cnt[o];
        }
        int mid = l + (r - l) / 2;
        if (x <= mid) {
            return query(x, o << 1, l, mid);
        }
        return query(x, o << 1 | 1, mid + 1, r);
    }
    int query(int x) { return query(x, 1, 0, n - 1); }

    // 区间查询 [L, R]：查询一段区间中所有数字出现的总次数
    int query(int L, int R, int o, int l, int r) {
        if (L <= l && r <= R) {
            return cnt[o];
        }
        int ans = 0;
        int mid = l + (r - l) / 2;
        if (L <= mid) {
            ans += query(L, R, o << 1, l, mid);
        }
        if (R > mid) {
            ans += query(L, R, o << 1 | 1, mid + 1, r);
        }
        return ans;
    }
    int query(int L, int R) { return query(L, R, 1, 0, n - 1); }

    // 查询整个值域中第 k 小的数
    int Kth_Min(int k, int o, int l, int r) {
        if (l == r) {
            return l;
        }
        int mid = l + (r - l) / 2;
        int cl = cnt[o << 1], cr = cnt[o << 1 | 1];
        if (k <= cl) {
            return Kth_Min(k, o << 1, l, mid);
        }
        return Kth_Min(k - cl, o << 1 | 1, mid + 1, r);
    }
    int Kth_Min(int k) { return Kth_Min(k, 1, 0, n - 1); }

    // 查询整个值域中第 k 大的数
    int Kth_Max(int k, int o, int l, int r) {
        if (l == r) {
            return l;
        }
        int mid = l + (r - l) / 2;
        int cl = cnt[o << 1], cr = cnt[o << 1 | 1];
        if (k <= cr) {
            return Kth_Max(k, o << 1, l, mid);
        }
        return Kth_Max(k - cr, o << 1 | 1, mid + 1, r);
    }
    int Kth_Max(int k) { return Kth_Max(k, 1, 0, n - 1); }
};
