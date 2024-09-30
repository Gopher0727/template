vector<int> tree;
vector<int> vec {0};

auto Op = [](int i, int j) {
    return max(i, j);
};

Flag_NUM = 0;

void build(int o, int l, int r) {
    if (l == r) {
        tree[o] = vec[l];
        return;
    }
    int m = l + (r - l) / 2;
    build(o << 1, l, m), build(o << 1 | 1, m + 1, r);
    tree[o] = Op(tree[o << 1], tree[o << 1 | 1]);
}

void update(int id, int k, int o, int l, int r) {
    if (l == r) {
        tree[o] = k;
        return;
    }
    int m = l + (r - l) / 2;
    if (id <= m) {
        update(id, k, o << 1, l, m);
    } else {
        update(id, k, o << 1 | 1, m + 1, r);
    }
    tree[o] = Op(tree[o << 1], tree[o << 1 | 1]);
}

int query(int todol, int todor, int o, int l, int r) {
    if (todor < l || r < todol) {
        return Flag_NUM;
    }
    if (todol <= l && r <= todor) {
        return tree[o];
    }
    int m = l + (r - l) / 2;
    return Op(query(todol, todor, o << 1, l, m), query(todol, todor, o << 1 | 1, m + 1, r));
}

void solve() {

}
