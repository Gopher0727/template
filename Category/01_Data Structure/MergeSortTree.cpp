// 归并排序树：支持区间查询，统计区间 [ql,qr] 内值在 [L, R] 内的个数
struct MergeSortTree {
    int n;
    vector<vector<int>> seg;

    MergeSortTree(const vector<int>& arr) {
        n = arr.size();
        seg.resize(4 * n);
        build(arr, 1, 0, n - 1);
    }

    void build(const vector<int>& arr, int idx, int l, int r) {
        if (l == r) {
            seg[idx] = {arr[l]};
            return;
        }
        int mid = (l + r) >> 1;
        build(arr, idx << 1, l, mid);
        build(arr, idx << 1 | 1, mid + 1, r);
        seg[idx].resize(seg[idx << 1].size() + seg[idx << 1 | 1].size());
        merge(seg[idx << 1].begin(), seg[idx << 1].end(), seg[idx << 1 | 1].begin(), seg[idx << 1 | 1].end(), seg[idx].begin());
    }

    // query in interval [ql, qr] counting numbers in [Lval, Rval]
    int query(int idx, int l, int r, int ql, int qr, int Lval, int Rval) {
        if (ql > r || qr < l) {
            return 0;
        }
        if (ql <= l && r <= qr) {
            // 在 seg[idx] 中计数
            auto& vec = seg[idx];
            int cnt = (upper_bound(vec.begin(), vec.end(), Rval) - lower_bound(vec.begin(), vec.end(), Lval));
            return cnt;
        }
        int mid = (l + r) >> 1;
        return query(idx << 1, l, mid, ql, qr, Lval, Rval) + query(idx << 1 | 1, mid + 1, r, ql, qr, Lval, Rval);
    }

    int query(int ql, int qr, int Lval, int Rval) {
        if (ql > qr) {
            return 0;
        }
        return query(1, 0, n - 1, ql, qr, Lval, Rval);
    }
};
