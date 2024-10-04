struct Node {};

vector<int> tree;
vector<int> vec;

const ll initVal = 0;
auto Op = [](int i, int j) {
    return max(i, j);
};

void make_tag() {}

void push() {}

void pull(int o) {
    tree[o] = Op(tree[o << 1], tree[o << 1 | 1]);
}

void modify(int id, int k, int o, int l, int r) {
    if (l == r) {
        tree[o] = k;
        return;
    }
    int m = l + (r - l) / 2;
    if (id <= m) {
        modify(id, k, o << 1, l, m);
    } else {
        modify(id, k, o << 1 | 1, m + 1, r);
    }
    pull(o);
}

ll query(int todol, int todor, int o, int l, int r) {
    if (todor < l || r < todol) {
        return initVal;
    }
    if (todol <= l && r <= todor) {
        return tree[o];
    }
    int m = l + (r - l) / 2;
    return Op(query(todol, todor, o << 1, l, m), query(todol, todor, o << 1 | 1, m + 1, r));
}

void build(int o, int l, int r) {
    if (l == r) {
        tree[o] = vec[l];
        return;
    }
    int m = l + (r - l) / 2;
    build(o << 1, l, m), build(o << 1 | 1, m + 1, r);
    pull(o);
}

void solve() {
    int n;
    cin >> n;

    vec.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> vec[i];
    }

    tree.resize(4 << __lg(n));
}
