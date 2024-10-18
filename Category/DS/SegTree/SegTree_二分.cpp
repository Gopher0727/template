vector<int> tree;

void build(int o, int l, int r, vector<int>& heights) {
    if (l == r) {
        tree[o] = heights[l];
        return;
    }
    int mid = l + (r - l) / 2;
    build(o << 1, l, mid, heights);
    build(o << 1 | 1, mid + 1, r, heights);
    tree[o] = max(tree[o << 1], tree[o << 1 | 1]);
}

int query(int o, int l, int r, int L, int v) { // 查询 [L, n-1] 中第一个大于 v 的下标
    if (tree[o] <= v) {
        return -1;
    }
    if (l == r) {
        return l;
    }
    int mid = l + (r - l) / 2;
    if (L <= mid) {
        int pos = query(o << 1, l, mid, L, v);
        if (pos >= 0) {
            return pos;
        }
    }
    return query(o << 1 | 1, mid + 1, r, L, v);
}

int findLeft(int R, int val, int o, int l, int r) { // 返回区间 [1,R] 中 <= val 的最靠左的位置，不存在时返回 -1
    if (mn[o] > val) {
        return -1;
    }
    if (l == r) {
        return l;
    }

    int m = l + (r - l) / 2;
    if (mn[o << 1] <= val) {
        return findLeft(R, val, o << 1, l, m);
    }
    if (R > m) {
        return findLeft(R, val, o << 1 | 1, m + 1, r);
    }
    return -1;
}
