/*
    权值线段树基于值域划分，往往需要离散化。

    作用:
    > 求一段区间的某个数字的出现次数;
    > 查询整体区间的第 k 大/小的值; （注意是`整体区间`，任意区间需要利用`主席树`）

    权值线段树一般 update 即是 build。
*/
class SegTree {
private:
    int n;
    vector<int> cnt;

public:
    SegTree() {}
    SegTree(int n) : n(n), cnt(n << 2) {}
    SegTree(vector<int>& vec) : SegTree(vec.size()) {
        for (int& v : vec) {
            update(1, 1, n, v);
        }
    }

private:
    // 单点修改：添加一个数字（增加 1）
    void update(int o, int l, int r, int x) {
        if (l == r) {
            cnt[o]++;
            return;
        }
        int mid = l + (r - l) / 2;
        if (x <= mid) {
            update(o << 1, l, mid, x);
        }
        if (x > mid) {
            update(o << 1 | 1, mid + 1, r, x);
        }
        cnt[o] = cnt[o << 1] + cnt[o << 1 | 1];
    }

    // 单点修改：增加 v
    void update(int o, int l, int r, int x, int v) {
        if (l == r) {
            cnt[o] += v;
            return;
        }
        int mid = l + (r - l) / 2;
        if (x <= mid) {
            update(o << 1, l, mid, x, v);
        }
        if (x > mid) {
            update(o << 1 | 1, mid + 1, r, x, v);
        }
        cnt[o] = cnt[o << 1] + cnt[o << 1 | 1];
    }

    // 单点查询：求某数的出现次数
    int query(int o, int l, int r, int x) {
        if (l == r) {
            return cnt[o];
        }
        int mid = l + (r - l) / 2;
        if (x <= mid) {
            return query(o << 1, l, mid, x);
        }
        return query(o << 1 | 1, mid + 1, r, x);
    }

    // 区间查询 [L, R]：查询一段区间中所有数字出现的总次数
    int query(int o, int l, int r, int L, int R) {
        if (L <= l && r <= R) {
            return cnt[o];
        }
        int ans = 0;
        int mid = l + (r - l) / 2;
        if (L <= mid) {
            ans += query(o << 1, l, mid, L, R);
        }
        if (R > mid) {
            ans += query(o << 1 | 1, mid + 1, r, L, R);
        }
        return ans;
    }

    // 查询整个值域中第 k 小的数
    int Kth_Min(int o, int l, int r, int k) {
        if (l == r) {
            return l;
        }
        int mid = l + (r - l) / 2;
        int cl = cnt[o << 1], cr = cnt[o << 1 | 1];
        if (k <= cl) {
            return Kth_Min(o << 1, l, mid, k);
        }
        return Kth_Min(o << 1 | 1, mid + 1, r, k - cl);
    }

    // 查询整个值域中第 k 大的数
    int Kth_Max(int o, int l, int r, int k) {
        if (l == r) {
            return l;
        }
        int mid = l + (r - l) / 2;
        int cl = cnt[o << 1], cr = cnt[o << 1 | 1];
        if (k <= cr) {
            return Kth_Max(o << 1, l, mid, k);
        }
        return Kth_Max(o << 1 | 1, mid + 1, r, k - cr);
    }

public:
    void update(int x) { return update(1, 1, n, x); }
    void update(int x, int v) { return update(1, 1, n, x, v); }
    int query(int x) { return query(1, 1, n, x); }
    int query(int l, int r) { return query(1, 1, n, l, r); }
    int Kth_Max(int k) { return Kth_Max(1, 1, n, k); }
    int Kth_Min(int k) { return Kth_Min(1, 1, n, k); }
};
// 注意下标从 1 开始。
